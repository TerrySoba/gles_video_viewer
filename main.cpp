#include "gl_handler.h"

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <memory>
#include <chrono>



static const GLuint WIDTH = 1920 * 2;
static const GLuint HEIGHT = 1080 * 2;

static const GLuint RENDER_WIDTH = WIDTH * 1.0;
static const GLuint RENDER_HEIGHT = HEIGHT * 1.0;

static const GLfloat vertices[] = {
    -1.0,  1.0, 0.0,
     1.0,  1.0, 0.0,
    -1.0, -1.0, 0.0,
     1.0, -1.0, 0.0,
};

static const GLfloat textureCoordinates[] = {
     0.0,  0.0,
     1.0,  0.0,
     0.0,  1.0,
     1.0,  1.0,
};

cv::Mat getField(const cv::Mat& mat, int field)
{
    cv::Mat frame(mat.rows / 2, mat.cols, mat.type());

    for (int y = field; y < mat.rows; y += 2)
    {
        mat.row(y).copyTo(frame.row(y / 2));
    }

    return frame;
}



int main(void) {
    try
    {
        // cv::VideoCapture capture("/home/yoshi252/Downloads/VID_20170629_221511.avi");
        cv::VideoCapture capture("../gles_video_viewer/pix/mario_world_cap.avi");
        // cv::VideoCapture capture(1);

        // capture.set(CV_CAP_PROP_FRAME_WIDTH,480);
        // capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);

        cv::Mat image = cv::imread("../gles_video_viewer/pix/test2.png");
        capture >> image;

        GlHandler gl(WIDTH, HEIGHT, __FILE__);

        GLFWwindow* window;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
        glfwMakeContextCurrent(window);

        printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
        printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );


        /// setup framebuffer
        // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
        GLuint FramebufferName = 0;
        glGenFramebuffers(1, &FramebufferName);
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

        // The texture we're going to render to
        GLuint renderedTexture;
        glGenTextures(1, &renderedTexture);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, renderedTexture);

        // Give an empty image to OpenGL ( the last "0" )
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, RENDER_WIDTH, RENDER_HEIGHT, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

        // Poor filtering. Needed !
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Set "renderedTexture" as our colour attachement #0
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

        // Always check that our framebuffer is ok
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("glCheckFramebufferStatus returned false.");
        }


        /// load shader programs
        auto crtProgram = gl.compileAndLinkShaderFiles(
                    "../gles_video_viewer/shader.vert",
                    "../gles_video_viewer/shader.frag");

        auto passthroughProgram = gl.compileAndLinkShaderFiles(
                    "../gles_video_viewer/passthrough.vert",
                    "../gles_video_viewer/passthrough.frag");

        auto blurProgram = gl.compileAndLinkShaderFiles(
                    "../gles_video_viewer/blur.vert",
                    "../gles_video_viewer/blur.frag");

        auto pos = 0;
        auto texCoord = 1;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


        auto vbo = gl.createVertexBufferObject(vertices, sizeof(vertices), 3, pos);
        auto texCoordBuffer = gl.createVertexBufferObject(textureCoordinates, sizeof(textureCoordinates), 2, texCoord);

        // auto shadowmask = gl.loadTexture("../gles_video_viewer/pix/subpixels.png");
        auto shadowmask = gl.loadTexture("../gles_video_viewer/pix/subpixles_4k.png");
        auto scanline = gl.loadTexture("../gles_video_viewer/pix/scanline_mono.png");
        // auto texture = gl.loadTexture("../gles_video_viewer/pix/test.png");
        auto texture = gl.loadTexture(image);

        auto subpixelSampler = 0;
        auto imageSampler = 1;
        auto scanlineSampler = 2;
        auto inputRes = 3;

        glUseProgram(*crtProgram);
        glUniform1i(subpixelSampler, 0);
        glUniform1i(imageSampler, 1);
        glUniform1i(scanlineSampler, 2);


        glfwSwapInterval(1); // enable vsync

        size_t frameCounter = 0;
        auto startTime = std::chrono::high_resolution_clock::now();


        int inputX = 320;
        int inputY = 240;

        while (!glfwWindowShouldClose(window)) {

            glfwPollEvents();

            // Render to our framebuffer
            glUseProgram(*crtProgram);
            glUniform2i(inputRes, inputX, inputY);

            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
            glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGHT);

            glClear(GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, *shadowmask);

            glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
            glBindTexture(GL_TEXTURE_2D, *texture);

            glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 2
            glBindTexture(GL_TEXTURE_2D, *scanline);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


            // now render to screen
            glUseProgram(*passthroughProgram);
            // glUseProgram(*blurProgram);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, WIDTH, HEIGHT);

            glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, renderedTexture);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glfwSwapBuffers(window);
            ++frameCounter;

            // output fps
            if (frameCounter == 300)
            {

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end-startTime;
                std::cout << "fps: " << ((double)frameCounter / diff.count()) << "\n";
                startTime = std::chrono::high_resolution_clock::now();
                frameCounter = 0;
            }

            int field = frameCounter & 1;

            if (field)
                capture >> image;

            auto fieldImage = getField(image, field);
            texture = gl.loadTexture(fieldImage);

            inputX = fieldImage.cols;
            inputY = fieldImage.rows;

            glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
            glBindTexture(GL_TEXTURE_2D, *texture);
        }

        glfwTerminate();


        return EXIT_SUCCESS;

    }
    catch (std::exception& ex)
    {
        std::cerr << "Caught std::exception. what() \"" << ex.what() << "\"" << std::endl;
    }
    catch (...)
    {
        std::cerr << "Caught unknown exception." << std::endl;
    }

    return EXIT_FAILURE;
}
