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




static const GLuint WIDTH = 1920 * 2;
static const GLuint HEIGHT = 1080 * 2;

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


int main(void) {
    try
    {
        // cv::VideoCapture capture("/home/yoshi252/Downloads/VID_20170629_221511.avi");
        // cv::VideoCapture capture("/home/yoshi252/Downloads/5_Exceed_-_Heaven7_(Win32).avi");
        cv::VideoCapture capture(0);

        capture.set(CV_CAP_PROP_FRAME_WIDTH,1920);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT,1080);

        cv::Mat image;
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

        auto shaderProgram = gl.compileAndLinkShaderFiles(
                    "../gles_video_viewer/shader.vert",
                    "../gles_video_viewer/shader.frag");

        auto pos = 0;
        auto texCoord = 1;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, WIDTH, HEIGHT);

        auto vbo = gl.createVertexBufferObject(vertices, sizeof(vertices), 3, pos);
        auto texCoordBuffer = gl.createVertexBufferObject(textureCoordinates, sizeof(textureCoordinates), 2, texCoord);

        auto shadowmask = gl.loadTexture("../gles_video_viewer/pix/subpixels.png");
        // auto texture = gl.loadTexture("../gles_video_viewer/pix/test.png");
        auto texture = gl.loadTexture(image);

        auto subpixelSampler = 0;
        auto imageSampler = 1;

        glUseProgram(*shaderProgram);
        glUniform1i(subpixelSampler, 0);
        glUniform1i(imageSampler, 1);

        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, *shadowmask);

        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, *texture);

        glfwSwapInterval(1); // enable vsync
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(*shaderProgram);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glfwSwapBuffers(window);

            capture >> image;
            texture = gl.loadTexture(image);

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
