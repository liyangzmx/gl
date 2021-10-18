#include "Utils.h"

#include <iostream>
#include <fstream>

using namespace std;

void Utils::printShaderLog(GLuint shader) {
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

void Utils::printProgramLog(GLuint prog) {
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

bool Utils::checkOpenGLError() {
    bool foundError = false;
    GLenum glError = glGetError();
    if(glError != GL_NO_ERROR) {
        cout << "glError: " << glError << endl;
        foundError = true;
        glError = glGetError();
    }

    return foundError;
}

string Utils::readShaderSource(const char *filePath) {
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

GLuint Utils::createShaderProgram() {
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