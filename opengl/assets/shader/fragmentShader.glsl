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

    vec3 color;

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

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

uniform vec3 texture_ambienttexture_ambient;
uniform vec3 texture_diffusetexture_diffuse;
uniform vec3 texture_speculartexture_specular;

void main()
{
    // 点 - 光源
    vec3 lightDir = normalize(light.position - FragPos);
    // 点 - 相机
    vec3 viewDir = normalize(viewPos - FragPos);
    // 光的强度 常数+一次系*distance+二次系*二次系*distance
    float _distance = distance(FragPos, light.position);
    float attenuation  = 1.0 / (light.constant + _distance * light.linear + _distance * _distance * light.quadratic);
    
    vec3 ambientLight = texture_ambienttexture_ambient * light.color;
    vec3 norm = normalize(Normal);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuseLight = diff * light.color;
    vec3 reflectLight = reflect(-lightDir, Normal);
    vec3 specularLight = pow(max(dot(reflectLight, viewDir), 0.0f),  32) * light.color;

    vec3 result = (ambientLight + diffuseLight * attenuation) ;


    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4((texColor * intensity).xyz * result + specularLight * attenuation * texture(texture_specular1, TexCoords).xyz, 1.0f) ;
}