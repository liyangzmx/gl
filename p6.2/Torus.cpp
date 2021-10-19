#include <cmath>
#include <vector>
#include <iostream>
#include "Torus.h"
using namespace std;

Torus::Torus() {
   prec = 48;
   inner = 0.5f;
   outer = 0.2f;
   init();
}

Torus::Torus(float innerRadius, float outerRadius, int precIn) {
   prec = precIn;
   inner = innerRadius;
   outer = outerRadius;
   init();
}

float Torus::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Torus::init() {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { sTangents.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { tTangents.push_back(glm::vec3()); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    // 计算第一个环
    for (int i = 0; i < prec + 1; i++) {
        float amt = toRadians(i*360.0f / prec);
        // 绕原点旋转点，形成环，然后将它们向外移动
        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 initPos(rMat * glm::vec4(outer, 0.0f, 0.0f, 1.0f));
        vertices[i] = glm::vec3(initPos + glm::vec3(inner, 0.0f, 0.0f));

        // 为环上的每个顶点计算纹理坐标
        texCoords[i] = glm::vec2(0.0f, ((float)i / (float)prec));

        // 计算切向量和法向量，第一个切向量是绕Z轴旋转的Y轴
        rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
        sTangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));   // 第二个切向量是 -Z 轴
        normals[i] = glm::cross(tTangents[i], sTangents[i]);      // 它们的叉乘积就是法向量
    }
    // 绕Y轴旋转最初的那个环，形成其他的环
    for (int i = 0; i < prec + 1; i++) {
        for (int ring = 1; ring < prec + 1; ring++) {
            for (int vert = 0; vert < prec + 1; vert++) {

                // 绕Y轴旋转最初那个环的顶点坐标
                float amt = (float)( toRadians(ring * 360.0f / prec));
                glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
                vertices[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

                // 计算新环顶点的纹理坐标
                texCoords[ring*(prec + 1) + vert] = glm::vec2((float)ring*2.0f / (float)prec, texCoords  
                        [vert].t);

                if (texCoords[ring*(prec + 1) + i].s > 1.0) texCoords[ring*(prec+1)+i].s -= 1.0f;

                // 绕Y轴旋转切向量和副切向量
                rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
                sTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents[i], 1.0f));
                rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
                tTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents[i], 1.0f));

                // 绕Y轴旋转法向量
                rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
                normals[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(normals[i], 1.0f));
            }
        }
    }

  // 按照逐个顶点的两个三角形，计算三角形索引
    for (int ring = 0; ring < prec; ring++) {
        for (int vert = 0; vert < prec; vert++) {
            indices[((ring*prec + vert) * 2) * 3 + 0] = ring*(prec + 1) + vert;
            indices[((ring*prec + vert) * 2) * 3 + 1] = (ring + 1)*(prec + 1) + vert;
            indices[((ring*prec + vert) * 2) * 3 + 2] = ring*(prec + 1) + vert + 1;
            indices[((ring*prec + vert) * 2 + 1) * 3 + 0] = ring*(prec + 1) + vert + 1;
            indices[((ring*prec + vert) * 2 + 1) * 3 + 1] = (ring + 1)*(prec + 1) + vert;
            indices[((ring*prec + vert) * 2 + 1) * 3 + 2] = (ring + 1)*(prec + 1) + vert + 1;
        }
    }
}

// 环面索引和顶点的访问函数
int Torus::getNumVertices() { return numVertices; }
int Torus::getNumIndices() { return numIndices; }
std::vector<int> Torus::getIndices() { return indices; }
std::vector<glm::vec3> Torus::getVertices() { return vertices; }
std::vector<glm::vec2> Torus::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Torus::getNormals() { return normals; }
std::vector<glm::vec3> Torus::getStangents() { return sTangents; }
std::vector<glm::vec3> Torus::getTtangents() { return tTangents; }