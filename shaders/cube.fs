#version 330 core

struct Material
{
    // vec3 diffuse;
    sampler2D diffuse;
    // vec3 specular;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

out vec4 FragColor;

in vec3 normal;  
in vec3 worldPos;
in vec2 texCoords;

uniform vec3 lightColor;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - worldPos);
    vec3 norm = normalize(normal);

    // 内外圆锥
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));
    // ambient
    vec3 ambient = lightColor * attenuation * vec3(texture(material.diffuse, texCoords)) * light.ambient * intensity;
  	
    // diffuse 
    // vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * attenuation * (diff * vec3(texture(material.diffuse, texCoords)) * light.diffuse) * intensity;

    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * attenuation * (spec * vec3(texture(material.specular, texCoords)) * light.specular) * intensity;

    // emissive
    vec3 emissive = (texture(material.emissive, texCoords)).xyz;
            
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}