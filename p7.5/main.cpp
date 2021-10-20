#include <iostream>
#include <stack>

// sudo apt install libgles2-mesa-dev
#define GLFW_INCLUDE_ES3

// sudo apt install libglfw3-dev
#include <GLFW/glfw3.h>

// sudo apt install libglew-dev
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "Sphere.h"
#include "Torus.h"

Sphere mySphere(48);
Torus myTorus(1.0f, 0.5f, 48);

using namespace std;

#define numVAOs 1
#define numVBOs 4

float cameraX, cameraY, cameraZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint mvLoc;
GLuint mLoc, vLoc;
GLuint projLoc, tfLoc, nLoc;
int width, height;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc; 
float aspect;
glm::mat4 pMat, vMat, mMat, tMat, rMat, mvMat, invTrMat;
float scale = 1.0;
glm::vec3 currentLightPos, lightPosV;
float lightPos[3];

glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);

float* matAmb = Utils::goldAmbient();
float* matDif = Utils::goldDiffuse();
float* matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();

stack<glm::mat4> mvStack;
GLuint brickTexture = GL_NONE;

float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

void setupVerties(void) {
    float vertexPositions[108] = {
      -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
   };
   float pyramidPositions[54] = {
        -1.0f, -1.0f, 1.0, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
   };
   float pyrTexCoords[36] =
    { 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // 前侧面、右侧面
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // 后侧面、左侧面
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }; // 底面的两个三角形

    std::vector<int> ind = myTorus.getIndices();
    std::vector<glm::vec3> vert = myTorus.getVertices();
    std::vector<glm::vec2> tex = myTorus.getTexCoords();
    std::vector<glm::vec3> norm = myTorus.getNormals();

    std::vector<float> pvalues;     // 顶点位置
    std::vector<float> tvalues;     // 纹理坐标
    std::vector<float> nvalues;     // 法向量

    int numIndices = myTorus.getNumIndices();
    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back((vert[ind[i]]).x);
        pvalues.push_back((vert[ind[i]]).y);
        pvalues.push_back((vert[ind[i]]).z);

        tvalues.push_back((tex[ind[i]]).s);
        tvalues.push_back((tex[ind[i]]).t);

        nvalues.push_back((norm[ind[i]]).x);
        nvalues.push_back((norm[ind[i]]).y);
        nvalues.push_back((norm[ind[i]]).z);
    }

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    // 把顶点放入缓冲区 #0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);

    // 把纹理坐标放入缓冲区 #1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);

    // 把法向量放入缓冲区 #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);

    // 把索引放入缓冲区 #3
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size()*4, &ind[0], GL_STATIC_DRAW);
}

void init(GLFWwindow *window){
    renderingProgram = Utils::createShaderProgram();
    
    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 8.0f;

    setupVerties();

    brickTexture = Utils::loadTexture("../qq.jpg");
    cout << "brickTexture: " << brickTexture << endl;
}

void installLights(glm::mat4 vMatrix) { 
   // 将光源位置转换为视图空间坐标，并存入浮点数组
   lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0)); 
   lightPos[0] = lightPosV.x; 
   lightPos[1] = lightPosV.y; 
   lightPos[2] = lightPosV.z; 

   // 在着色器中获取光源位置和材质属性
   globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient"); 
   ambLoc = glGetUniformLocation(renderingProgram, "light.ambient"); 
   diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse"); 
   specLoc = glGetUniformLocation(renderingProgram, "light.specular"); 
   posLoc = glGetUniformLocation(renderingProgram, "light.position"); 
   mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient"); 
   mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse"); 
   mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular"); 
   mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess"); 

   // 在着色器中为光源与材质统一变量赋值
   glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient); 
   glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient); 
   glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse); 
   glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular); 
   glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos); 
   glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb); 
   glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif); 
   glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe); 
   glProgramUniform1f(renderingProgram, mShiLoc, matShi);
}

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void display(GLFWwindow *window, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderingProgram);
    // mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
    vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
    tfLoc = glGetUniformLocation(renderingProgram, "tf");
    
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glfwGetFramebufferSize(window, &width, &height);
    // cout << "width: " << width << ", height: " << height << endl;
    aspect = (float)width / (float)height;
    // cout << "apsect: " << aspect << endl;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);

    // 旋转环面以便更容易看到
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
    mMat = glm::scale(mMat, glm::vec3(scale, scale, scale));

    installLights(vMat);
    mvMat = vMat * mMat;

    invTrMat = glm::transpose(glm::inverse(mvMat));

    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    // glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
    
    float timeFactor = ((float)currentTime / 10);
    glUniform1f(tfLoc, (float)timeFactor);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(2); 

    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    if(glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
    } else {
        cout << "Not support GL_EXT_texture_filter_anisotropic." << endl;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

void window_reshape_callback(GLFWwindow *window, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void window_scroll_callback(GLFWwindow *window, double x, double y) {
    cout << "x: " << x << ", y: " << y << endl;
    scale += (0.1f * y);
}

int main(int, char**) {
    if(!glfwInit()) { 
        cout << "glfwInit() failed" << endl;
        exit(EXIT_FAILURE); 
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(640, 360, "P2.2", NULL, NULL);
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) { 
        cout << "glewInit() failed" << endl;
        exit(EXIT_FAILURE); 
    }
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, window_reshape_callback);
    glfwSetScrollCallback(window, window_scroll_callback);

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
