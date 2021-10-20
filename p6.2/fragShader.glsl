#version 430

out vec4 vColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding = 0) uniform sampler2D samp;

in vec2 tc;

void main() {
    vColor = texture(samp, tc);
}
