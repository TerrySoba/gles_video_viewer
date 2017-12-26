#pragma once

#include <opencv2/core/core.hpp>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include <memory>

class GlHandler
{
public:
    GlHandler(int width, int height, const std::string& title);

    std::shared_ptr<GLuint> compileAndLinkShaderFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    std::shared_ptr<GLuint> compileAndLinkShaders(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    std::shared_ptr<GLuint> loadTexture(const std::string& path);
    std::shared_ptr<GLuint> loadTexture(const cv::Mat& path);
    std::shared_ptr<GLuint> createVertexBufferObject(const void* data, std::size_t dataSize, std::size_t vectorSize, GLuint index);

};


