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
}