#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

uniform Material material_diffuse1;
uniform Material material_diffuse2;
uniform Material material_diffuse3;

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

uniform Material texture_specular_1;
uniform Material texture_specular_2;
uniform Material texture_specular_3;

void main()
{
    // 点 - 光源
    vec3 lightDir = normalize(light.position - FragPos);
    // 点 - 相机
    vec3 point_eye = FragPos - viewPos;
    // 光的强度 常数+一次系*distance+二次系*二次系*distance
    float _distance = distance(FragPos, light.position);
    float attenuation  = 1.0 / (light.constant + _distance * light.linear + _distance * _distance * light.quadratic);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    FragColor = texColor * intensity;
}