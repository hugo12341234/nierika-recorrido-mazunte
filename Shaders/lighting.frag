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
uniform PointLight velasAltar;   // X-8: luz calida y parpadeante de las velas del altar (una sola puntual, no cuenta entre las 4 de D1)
uniform SpotLight escalonSpots[NUMBER_OF_STEP_SPOTS];
uniform Material material;          // shininess la envia Mesh::Draw
uniform sampler2D texture_diffuse1; // nombre que envia Mesh::Draw
uniform float specularStrength;     // brillo especular por objeto (los modelos no traen mapa especular)

// B-12: mapa de relieve (altura en gris). Solo el pan lo trae: Mesh::Draw pone tieneRelieve = 1 en sus mallas.
uniform int       tieneRelieve;
uniform int       relieveLuma;     // X-23: 1 = relieve a partir de la luminancia de la textura difusa (barda, tumbas, cruz, roca); la arena usa 'arena'
uniform sampler2D texture_height1;
const float RELIEVE_M = 0.003;      // metros de altura que representa el blanco del mapa (3 mm en un pan de 16 cm)

// Perturba la normal N con el gradiente de la altura h (en metros) sin necesitar tangentes: usa las derivadas de pantalla
// de la posicion p y de h (Mikkelsen, "Bump Mapping Unparametrized Surfaces on the GPU", 2010).
vec3 perturbaNormal(vec3 N, vec3 p, float h)
{
    vec3 dpdx = dFdx(p);
    vec3 dpdy = dFdy(p);
    float dhdx = dFdx(h);
    float dhdy = dFdy(h);
    vec3 r1 = cross(dpdy, N);
    vec3 r2 = cross(N, dpdx);
    float det = dot(dpdx, r1);
    vec3 gradiente = sign(det) * (dhdx * r1 + dhdy * r2);
    return normalize(abs(det) * N - gradiente);
}

// C-2: llama de las velas. Con llama = 1 el fragmento ignora la textura y la luz: la forma, el color y el parpadeo
// salen de ruido de Perlin 2D (ver docs/manual_tecnico/justificacion_simples.md, seccion 2.3).
uniform int   espuma;               // X-22: 1 solo al dibujar el mar (espuma en la orilla)
uniform int   arena;                // X-13: 1 solo al dibujar la playa (ver main.cpp)
uniform int   llama;
uniform float tiempo;               // segundos (el mismo uniform del vertex shader)
uniform float llamaFase;            // desfase de cada llama (s), para que no parpadeen al unisono

// Gradiente pseudoaleatorio unitario de un vertice de la rejilla (hash clasico de seno)
vec2 gradienteRejilla(vec2 v)
{
    float ang = 6.2831853 * fract(sin(dot(v, vec2(127.1, 311.7))) * 43758.5453);
    return vec2(cos(ang), sin(ang));
}

// Ruido de Perlin 2D: producto punto de los gradientes de las 4 esquinas de la celda con el vector hacia el punto,
// interpoladas con la curva suave de Perlin mejorado s(u) = 6u^5 - 15u^4 + 10u^3. Rango aproximado [-0.7, 0.7].
float perlin(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
    float a = dot(gradienteRejilla(i),                f);
    float b = dot(gradienteRejilla(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0));
    float c = dot(gradienteRejilla(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0));
    float d = dot(gradienteRejilla(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0));
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// Suma de 3 octavas (frecuencia x2, amplitud x0.5 en cada una)
float ruidoFbm(vec2 p)
{
    float suma = 0.0;
    float amp = 0.5;
    for (int k = 0; k < 3; k++)
    {
        suma += amp * perlin(p);
        p *= 2.0;
        amp *= 0.5;
    }
    return suma;
}

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
    // X-22: luciernaga (llama = 2): punto emisivo; llamaFase = brillo 0..1 (parpadeo calculado en main.cpp); sin luz ni textura
    if (llama == 2)
    {
        if (llamaFase < 0.04) discard;
        color = vec4(vec3(0.75, 1.0, 0.35) * (0.4 + 1.6 * llamaFase), 1.0);
        return;
    }

    if (llama == 1)
    {
        // El .obj de la llama tiene v = 0 en la base y Assimp voltea las UV (FlipUVs): h = 0 en la base, 1 en la punta
        float h = 1.0 - TexCoords.y;
        float x = TexCoords.x * 2.0 - 1.0;                 // -1..1 a lo ancho
        float t = tiempo + llamaFase;
        // el ruido sube con el tiempo (se resta a la coordenada vertical) y mueve la llama de lado, mas en la punta
        float n = ruidoFbm(vec2(TexCoords.x * 2.5, h * 3.0 - t * 2.0));
        float xd = x + n * 0.55 * h;
        float ancho = 0.85 * pow(1.0 - h, 0.7);            // silueta de gota: ancha abajo, afilada arriba
        float forma = 1.0 - abs(xd) / max(ancho, 0.001);   // 1 en el eje, 0 en el borde
        forma -= (0.5 + 0.5 * n) * 0.25 * h;               // el ruido roe la punta: parpadeo
        if (forma < 0.1)
            discard;
        float calor = clamp(forma * 1.2 - h * 0.5 + 0.3, 0.0, 1.0);
        vec3 col = mix(vec3(0.9, 0.2, 0.02), vec3(1.0, 0.6, 0.1), smoothstep(0.0, 0.5, calor));
        col = mix(col, vec3(1.0, 0.95, 0.7), smoothstep(0.5, 1.0, calor));
        color = vec4(col, 1.0);                            // emisiva: no recibe luz
        return;
    }

    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (texColor.a < 0.1)
        discard;                      // descarte por alfa (papel picado, flores)

    // gl_FrontFacing: si es back-face (interior del modelo), invertimos la normal
    // para que la iluminacion difusa funcione desde adentro
    vec3 norm = normalize(gl_FrontFacing ? Normal : -Normal);
    if (tieneRelieve == 1)   // B-12: la condicion es un uniform, asi que las derivadas de pantalla estan bien definidas
        norm = perturbaNormal(norm, FragPos, texture(texture_height1, TexCoords).r * RELIEVE_M);
    if (relieveLuma == 1 || arena == 1)   // X-23: mapa de altura = luminancia de la textura (condicion uniforme: las derivadas de pantalla estan bien definidas)
    {
        float hl = dot(texColor.rgb, vec3(0.30, 0.59, 0.11));
        norm = perturbaNormal(norm, FragPos, hl * (arena == 1 ? 0.012 : 0.035));   // 1.2 cm en la arena, 3.5 cm en la piedra
    }
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 albedo = texColor.rgb;
    if (espuma == 1)
    {
        // X-22: franja de espuma donde el mar toca la orilla (z ~ 24): ruido de Perlin que avanza hacia la playa; f en [0, 1]
        float borde = smoothstep(26.2, 24.3, FragPos.z);
        float n = ruidoFbm(vec2(FragPos.x * 0.7, FragPos.z * 0.9 - tiempo * 0.45));
        float f = borde * smoothstep(0.02, 0.30, n + 0.18 + 0.18 * sin(tiempo * 0.8 - FragPos.z * 1.6));
        albedo = mix(albedo, vec3(0.80, 0.88, 0.95), clamp(f, 0.0, 0.85));
    }
    if (arena == 1)
    {
        float manchas = ruidoFbm(FragPos.xz * 0.35);                         // ~[-0.4, 0.4], periodo de ~3 m
        albedo *= 0.85 + 0.45 * manchas;
        float mojada = smoothstep(0.0, -0.25, FragPos.y);                    // 0 en seco, 1 a 25 cm bajo y = 0
        albedo *= mix(1.0, 0.45, mojada);
    }

    vec3 result = CalcDirLight(dirLight, norm, viewDir, albedo);
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, albedo);
    result += CalcPointLight(velasAltar, norm, FragPos, viewDir, albedo);
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, albedo);
    for (int i = 0; i < NUMBER_OF_STEP_SPOTS; i++)
        result += CalcSpotLight(escalonSpots[i], norm, FragPos, viewDir, albedo);

    // X-18: bruma de distancia. f = 1 - exp(-(d * k)^1.5): 0 cerca, tiende a 1 lejos; se mezcla con el color del horizonte del cielo
    float distCam = length(viewPos - FragPos);
    float bruma = 1.0 - exp(-pow(distCam * 0.012, 1.5));
    result = mix(result, vec3(0.07, 0.12, 0.20), clamp(bruma, 0.0, 0.85));
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
