// sudo apt install libglew-dev
#include <GL/glew.h>
#include <string>

#include "SOIL2.h"

namespace Utils {
    using namespace std;

    void printShaderLog(GLuint shader);
    void printProgramLog(GLuint prog);
    bool checkOpenGLError();
    string readShaderSource(const char *filePath);
    GLuint createShaderProgram();
    GLuint loadTexture(const char *textImagePath);

    // 黄金材质 — 环境光、漫反射、镜面反射和光泽
    float *goldAmbient();
    float *goldDiffuse();
    float *goldSpecular();
    float goldShininess();

    // 白银材质 — 环境光、漫反射、镜面反射和光泽
    float *silverAmbient();
    float *silverDiffuse();
    float *silverSpecular();
    float silverShininess();

    // 青铜材质 — 环境光、漫反射、镜面反射和光泽
    float *bronzeAmbient();
    float *bronzeDiffuse();
    float *bronzeSpecular();
    float bronzeShininess();
}