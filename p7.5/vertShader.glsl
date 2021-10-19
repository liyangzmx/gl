#version 430
layout (location=0) in vec3 vertPos;
layout (location=1) in vec3 vertNormal;
out vec4 varyingColor;

struct PositionalLight
{ vec4 ambient; 
  vec4 diffuse; 
  vec4 specular; 
  vec3 position;
};
struct Material
{ vec4 ambient; 
  vec4 diffuse; 
  vec4 specular; 
  float shininess;
};
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;   

void main(void)
{ vec4 color; 

  vec4 P = mv_matrix * vec4(vertPos,1.0); 
  vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz); 
  vec3 L = normalize(light.position - P.xyz); 

  // 视觉向量等于视觉空间中的负顶点位置
  vec3 V = normalize(-P.xyz); 

  // R是-L的相对于表面向量N的镜像
  vec3 R = reflect(-L,N); 

  // 环境光、漫反射和镜面反射分量
  vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L), 0.0); 
  vec3 specular = 
     material.specular.xyz * light.specular.xyz * pow(max(dot(R,V), 0.0f), material.shininess); 

  varyingColor = vec4((ambient + diffuse + specular), 1.0); 


  gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}