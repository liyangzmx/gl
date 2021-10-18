#include <iostream>

// sudo apt install libgles2-mesa-dev
#define GLFW_INCLUDE_ES3

// sudo apt install libglfw3-dev
#include <GLFW/glfw3.h>

// sudo apt install libglew-dev
#include <GL/glew.h>

#include <string>
#include <fstream>

using namespace std;

#define numVaos 1

GLuint renderingProgram;
GLuint vao[numVaos];

void printShaderLog(GLuint shader) {
    int len;
    int chWrittn = 0;
    char *log = NULL;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = (char *)malloc(sizeof(char) * len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader info log: " << log << endl;
        free(log);
    }
}

void printProgramLog(GLuint prog) {
    int len;
    int chWritten = 0;
    char *log = NULL;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = (char *)malloc(sizeof(char) * len);
        glGetProgramInfoLog(prog, len, &chWritten, log);
        cout << "Program info log: " << log << endl;
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError = false;
    GLenum glError = glGetError();
    if(glError != GL_NO_ERROR) {
        cout << "glError: " << glError << endl;
        foundError = true;
        glError = glGetError();
    }

    return foundError;
}

string readShaderSource(const char *filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();

    return content;
}

GLuint createShaderProgram() {
    // const char *vShaderSrc = "#version 430\n"
    // "void main(void)\n"
    // "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    // const char *fShaderSrc = "#version 430\n"
    // "out vec4 color;\n"
    // "void main(void)\n"
    // "{ // color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    // "    if(gl_FragCoord.x < 200) \n"
    // "      color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    // "    else \n"
    // "      color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    // "}";
    GLint vertCompiled = false;
    GLint fragCompiled = false;
    GLint linked = false;

    string vertShaderStr = readShaderSource("../vertShader.glsl");
    string fragShaderStr = readShaderSource("../fragShader.glsl");
    const char *vShaderSrc = vertShaderStr.c_str();
    const char *fShaderSrc = fragShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if(vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }
    
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if(fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    checkOpenGLError();

    glLinkProgram(vfProgram);
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void init(GLFWwindow *window){
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVaos, vao);
    glBindVertexArray(vao[0]);
}

GLfloat x = 0.0;
GLfloat inc = 0.01;

void display(GLFWwindow *windows, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    x += inc;
    if(x > 1.0) inc = -0.01f;
    if(x < -1.0f) inc = 0.01f;
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    glProgramUniform1f(renderingProgram, offsetLoc, x);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int, char**) {
    if(!glfwInit()) { 
        cout << "glfwInit() failed" << endl;
        exit(EXIT_FAILURE); 
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(400, 200, "P2.2", NULL, NULL);
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) { 
        cout << "glewInit() failed" << endl;
        exit(EXIT_FAILURE); 
    }
    glfwSwapInterval(1);

    init(window);
    while(!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}
