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

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    // ambient
    vec3 ambient = lightColor * vec3(texture(material.diffuse, texCoords)) * light.ambient;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * vec3(texture(material.diffuse, texCoords)) * light.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * vec3(texture(material.specular, texCoords)) * light.specular);

    // emissive
    vec3 emissive = (texture(material.emissive, texCoords)).xyz;
            
    vec3 result = (ambient + diffuse + specular + emissive);
    FragColor = vec4(result, 1.0);
}