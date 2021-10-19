#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding = 0) uniform sampler2D samp;
uniform float tf;

out vec2 tc;

mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(  1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 
                          x,   y,   z, 1.0 );

    return trans;
}

mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(   1.0, 0.0, 0.0, 0.0,
                        0.0, cos(rad), -sin(rad), 0.0, 
                        0.0, sin(rad),  cos(rad), 0.0,
                        0.0, 0.0, 0.0, 1.0 );

    return xrot;
}

mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(    cos(rad), 0.0, sin(rad), 0.0,
                        0.0, 1.0, 0.0, 0.0, 
                        -sin(rad), 0.0, cos(rad), 0.0,
                        0.0, 0.0, 0.0, 1.0 );

    return yrot;
}

mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(   cos(rad), -sin(rad), 0.0, 0.0,
                        sin(rad),  cos(rad), 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0 );

    return zrot;
}

mat4 buildScale(float x, float y, float z) {
    mat4 scale = mat4(
          x, 0.0, 0.0, 0.0,
        0.0,   y, 0.0, 0.0, 
        0.0, 0.0,   z, 0.0, 
        0.0, 0.0, 0.0, 1.0
    );

    return scale;
}

void main() {
    float i = gl_InstanceID + tf;
    // float a = sin(203.0 * i/8000.0) * 403.0;
    // float b = sin(301.0 * i/4001.0) * 401.0;
    // float c = sin(400.0 * i/6003.0) * 405.0;

    // mat4 localRotX = buildRotateX(1 * i);
    // mat4 localRotY = buildRotateY(1 * i);
    // mat4 localRotZ = buildRotateZ(1 * i);
    float a = sin(2.0 * i) * 1.0;
    float b = sin(3.0 * i) * 1.0;
    float c = sin(4.0 * i) * 1.0;

    mat4 localRotX = buildRotateX(10 * i);
    mat4 localRotY = buildRotateY(10 * i);
    mat4 localRotZ = buildRotateZ(10 * i);
    mat4 localTrans = buildTranslate(a, b, c);

    mat4 newM_matrix = m_matrix * localTrans * localRotX * localRotY * localRotZ;
    mat4 mv_matrix = v_matrix * newM_matrix;
    
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    tc = texCoord;
}