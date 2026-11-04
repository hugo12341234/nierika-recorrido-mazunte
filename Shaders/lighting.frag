#version 330 core
// A-8: iluminacion Blinn-Phong. Base: shader de la Practica 8 adaptado por D1 (319266894).
// 1 direccional (luna) + 4 puntuales + 1 reflector (cruz) + 6 reflectores (escalones).

#define NUMBER_OF_POINT_LIGHTS 4
#define NUMBER_OF_STEP_SPOTS 6

struct Material
{
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform SpotLight escalonSpots[NUMBER_OF_STEP_SPOTS];
uniform Material material;          // shininess la envia Mesh::Draw
uniform sampler2D texture_diffuse1; // nombre que envia Mesh::Draw
uniform float specularStrength;     // brillo especular por objeto (los modelos no traen mapa especular)

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo);

// Blinn-Phong: H = normalize(L + V); el exponente es 4x el de Phong para un brillo parecido
float BlinnSpec(vec3 lightDir, vec3 normal, vec3 viewDir)
{
    vec3 halfDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfDir), 0.0), material.shininess * 4.0);
}

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (texColor.a < 0.1)
        discard;                      // descarte por alfa (papel picado, flores)

    // gl_FrontFacing: si es back-face (interior del modelo), invertimos la normal
    // para que la iluminacion difusa funcione desde adentro
    vec3 norm = normalize(gl_FrontFacing ? Normal : -Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 albedo = texColor.rgb;

    vec3 result = CalcDirLight(dirLight, norm, viewDir, albedo);
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, albedo);
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, albedo);
    for (int i = 0; i < NUMBER_OF_STEP_SPOTS; i++)
        result += CalcSpotLight(escalonSpots[i], norm, FragPos, viewDir, albedo);

    color = vec4(result, texColor.a);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = BlinnSpec(lightDir, normal, viewDir);

    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * specularStrength;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = BlinnSpec(lightDir, normal, viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * specularStrength;
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = BlinnSpec(lightDir, normal, viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * specularStrength;
    return (ambient + diffuse + specular) * attenuation * intensity;
}
