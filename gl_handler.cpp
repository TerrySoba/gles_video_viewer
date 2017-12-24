#include "gl_handler.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>
#include <exception>

GlHandler::GlHandler(int width, int height, const std::string& title)
{

}


std::string getFileContents(const std::string& path)
{
  std::FILE *fp = std::fopen(path.c_str(), "rb");
  if (fp)
  {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return(contents);
  }
  throw(errno);
}

std::shared_ptr<GLuint> GlHandler::compileAndLinkShaderFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    return compileAndLinkShaders(
                getFileContents(vertexShaderPath),
                getFileContents(fragmentShaderPath));
}

std::shared_ptr<GLuint> GlHandler::compileAndLinkShaders(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    enum Consts {INFOLOG_LEN = 512};
    GLchar infoLog[INFOLOG_LEN];
    GLint fragment_shader;
    GLint shader_program;
    GLint success;
    GLint vertex_shader;

    /* Vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto vertexSource = vertexShaderSource.c_str();
    glShaderSource(vertex_shader, 1, &vertexSource, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
        std::stringstream message;
        message << "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n" << infoLog;
        throw std::runtime_error(message.str());
    }

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentSource = fragmentShaderSource.c_str();
    glShaderSource(fragment_shader, 1, &fragmentSource, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
        std::stringstream message;
        message << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n" << infoLog;
        throw std::runtime_error(message.str());
    }

    /* Link shaders */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
        std::stringstream message;
        message << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << infoLog;
        throw std::runtime_error(message.str());
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return std::shared_ptr<GLuint>(new GLuint(shader_program), [](auto ptr) {
        glDeleteProgram(*ptr);
        delete ptr;
    });
}

std::shared_ptr<GLuint> GlHandler::loadTexture(const std::string& path)
{
    cv::Mat mat = cv::imread(path);
    cv::cvtColor(mat, mat, CV_RGB2BGR);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return std::shared_ptr<GLuint>(
                new GLuint(textureID),
                [](auto ptr) {
                    glDeleteTextures(1, ptr);
                    delete ptr;
                }
    );
}

std::shared_ptr<GLuint> GlHandler::createVertexBufferObject(const void* data, std::size_t dataSize, std::size_t vectorSize, GLuint index)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glVertexAttribPointer(index, vectorSize, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return std::shared_ptr<GLuint>(
                new GLuint(vbo),
                [](auto ptr) {
                    glDeleteBuffers(1, ptr);
                    delete ptr;
                }
    );
}
