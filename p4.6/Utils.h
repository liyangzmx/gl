// sudo apt install libglew-dev
#include <GL/glew.h>
#include <string>

namespace Utils {
    using namespace std;

    void printShaderLog(GLuint shader);
    void printProgramLog(GLuint prog);
    bool checkOpenGLError();
    string readShaderSource(const char *filePath);
    GLuint createShaderProgram();
}