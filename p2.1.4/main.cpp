#include <iostream>

// sudo apt install libgles2-mesa-dev
#define GLFW_INCLUDE_ES3

// sudo apt install libglfw3-dev
#include <GLFW/glfw3.h>

// sudo apt install libglew-dev
#include <GL/glew.h>

using namespace std;

#define numVaos 1

GLuint renderingProgram;
GLuint vao[numVaos];

GLuint createShaderProgram() {
    const char *vShaderSrc = "#version 430\n"
    "void main(void)\n"
    "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const char *fShaderSrc = "#version 430\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{ // color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "    if(gl_FragCoord.x < 200) \n"
    "      color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "    else \n"
    "      color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);

    return vfProgram;
}

void init(GLFWwindow *window){
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVaos, vao);
    glBindVertexArray(vao[0]);
    glPointSize(30.0);
}

void display(GLFWwindow *windows, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    glDrawArrays(GL_POINTS, 0, 1);
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
