#version 430
layout (location=0) in vec3 vertPos;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 vertNormal;

struct PositionalLight
{
  vec4 ambient; 
  vec4 diffuse; 
  vec4 specular; 
  vec3 position;
};
struct Material
{
  vec4 ambient; 
  vec4 diffuse; 
  vec4 specular; 
  float shininess;
};
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

layout (binding = 0) uniform sampler2D samp;
uniform float tf;

out vec2 tc;
out vec4 varyingColor;

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

void main(void)
{
  float i = gl_InstanceID + tf;

  float a = sin(2.0 * i) * 1.0;
  float b = sin(2.0 * i) * 1.0;
  float c = sin(2.0 * i) * 1.0;

  mat4 localRotX = buildRotateX(10 * i);
  mat4 localRotY = buildRotateY(10 * 0);
  mat4 localRotZ = buildRotateZ(10 * 0);
  mat4 localTrans = buildTranslate(1, 1, 1);

  mat4 newM_matrix = m_matrix * localTrans * localRotX * localRotY * localRotZ;
  mat4 mv_matrix = v_matrix * newM_matrix;

  gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
  tc = texCoord;

  vec4 P = mv_matrix * vec4(vertPos,1.0); 
  vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz); 
  vec3 L = normalize(light.position - P.xyz); 

  // 视觉向量等于视觉空间中的负顶点位置
  vec3 V = normalize(-P.xyz); 

  // // R是-L的相对于表面向量N的镜像
  vec3 R = reflect(-L,N); 

  // 环境光、漫反射和镜面反射分量
  vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L), 0.0); 
  vec3 specular = 
     material.specular.xyz * light.specular.xyz * pow(max(dot(R,V), 0.0f), material.shininess); 

  varyingColor = vec4((ambient + diffuse + specular), 1.0); 
}
