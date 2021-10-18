#version 430
out vec4 vColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main() {
    vColor = vec4(1.0, 0.0, 0.0, 1.0);
}