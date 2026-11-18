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
in vec3 LocalPos;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform PointLight luzCalida[2]; // X-34: las 2 fogatas o faroles mas cercanos al visitante (luz calida que parpadea; color 0 = apagada)
uniform PointLight velasAltar;   // X-8: luz calida y parpadeante de las velas del altar (una sola puntual, no cuenta entre las 4 de D1)
uniform PointLight altarMayor[2]; // X-51: luz calida de cada ofrenda mayor del patio (parpadea; siempre con constantes validas)
uniform SpotLight escalonSpots[NUMBER_OF_STEP_SPOTS];
uniform Material material;          // shininess la envia Mesh::Draw
uniform sampler2D texture_diffuse1; // nombre que envia Mesh::Draw
uniform float specularStrength;     // brillo especular por objeto (los modelos no traen mapa especular)

// B-12: mapa de relieve (altura en gris). Solo el pan lo trae: Mesh::Draw pone tieneRelieve = 1 en sus mallas.
uniform int       tieneRelieve;
uniform int       relieveLuma;     // X-23: 1 = relieve a partir de la luminancia de la textura difusa (barda, tumbas, cruz, roca); la arena usa 'arena'
uniform sampler2D texture_height1;
uniform sampler2D texture_mezcla;   // X-66: textura del material vecino en el borde arena/pasto (en la playa = pasto; en las colinas = arena)
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
uniform int   colina;               // X-36: 1 solo al dibujar las colinas (manchas de color a gran escala: selva y pasto seco)
uniform int   arena;                // X-13: 1 solo al dibujar la playa (ver main.cpp)
uniform int   llama;
uniform float tiempo;               // segundos (el mismo uniform del vertex shader)
uniform int   humo;                 // X-47: 1 solo al dibujar el humo del copal (modelo de D1): su textura es opaca, la transparencia sale de humoAlfa
uniform float humoAlfa;
uniform float llamaFase;            // desfase de cada llama (s), para que no parpadeen al unisono
uniform vec3  tinteObjeto;           // X-55: color por el que se multiplica la textura de las piezas de las ofrendas familiares (1,1,1 = sin cambio; main.cpp lo pone en 1 al inicio de cada cuadro)

// X-56: ventanas encendidas de las casas. Sin una luz por casa: el vidrio (malla con textura "vidrio", esVidrio = 1) suma una emision
// de color ventanaColor y brillo ventanaBrillo.x (ventana izquierda, x local < 0) o .y (derecha). Brillo 0 = ventana apagada (vidrio oscuro).
uniform int   esVidrio;
uniform vec3  ventanaColor;
uniform vec2  ventanaBrillo;

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

// X-66: tinte de la arena (X-36) y del pasto (X-36); se usan tambien para mezclar los dos materiales en el borde z = -20
vec3 colorArena(vec3 tex, vec2 xz, float y)
{
    vec3 a = tex * (0.85 + 0.45 * ruidoFbm(xz * 0.35));
    a *= vec3(1.30, 1.02, 0.72) * (0.88 + 0.30 * ruidoFbm(xz * 4.0));
    return a * mix(1.0, 0.45, smoothstep(0.0, -0.25, y));
}
vec3 colorPasto(vec3 tex, vec2 xz, float y)
{
    float m1 = ruidoFbm(xz * 0.025);
    float m2 = ruidoFbm(xz * 0.18);
    float alto = smoothstep(2.0, 26.0, y);
    vec3 tinte = mix(vec3(0.82, 1.18, 0.86), vec3(1.55, 1.28, 0.68), clamp(0.45 + m1 * 1.8 + alto * 0.45, 0.0, 1.0));
    return tex * tinte * (0.95 + 0.75 * m2);
}
// X-66: peso de arena en [0, 1] en el borde norte de la playa (z = -20). Misma funcion en las dos mallas => sin costura.
// s > 0 hacia la playa; el borde se ondula con ruido (+-2.4 m a ~3 m de periodo y +-0.8 m a ~0.5 m) y la mezcla dura 2 x BORDE_ANCHO
const float BORDE_Z = -20.0;
const float BORDE_ANCHO = 3.0;
float pesoArena(vec2 xz)
{
    float s = (xz.y - BORDE_Z) + 6.0 * ruidoFbm(xz * 0.30) + 2.0 * ruidoFbm(xz * 1.9);
    return smoothstep(-BORDE_ANCHO, BORDE_ANCHO, s);
}
// X-66 (correccion): la mezcla solo existe frente a la playa (|x| < 50). Las laderas de las colinas junto a la playa (z > -20, |x| > 50)
// quedaban con peso de arena = 1; esta mascara las deja con pasto. 1 dentro de |x| <= 46, 0 a partir de |x| >= 52
float mascaraPlaya(float x)
{
    return 1.0 - smoothstep(46.0, 52.0, abs(x));
}

void main()
{
    // X-22: luciernaga (llama = 2): punto emisivo; llamaFase = brillo 0..1 (parpadeo calculado en main.cpp); sin luz ni textura
    // X-24: orbe del farol (llama = 3): esfera emisiva calida; llamaFase = brillo 0..1 (parpadeo calculado en main.cpp)
    if (llama == 3)
    {
        color = vec4(vec3(1.0, 0.62, 0.22) * (0.55 + 1.1 * llamaFase), 1.0);
        return;
    }

    // X-36: halo calido (llama = 4): resplandor radial sobre un plano (UV 0..1); se dibuja con mezcla aditiva y sin escribir profundidad (main.cpp).
    // llamaFase = intensidad. Alfa = resplandor, para que la mezcla (SRC_ALPHA, ONE) lo sume sin cuadro visible en los bordes.
    if (llama == 4)
    {
        float rr = length(TexCoords - vec2(0.5)) * 2.0;
        float a = pow(clamp(1.0 - rr, 0.0, 1.0), 2.2) * llamaFase;
        if (a < 0.004) discard;
        color = vec4(vec3(1.0, 0.55, 0.20), a);
        return;
    }

    // X-56: halo de ventana (llama = 6): igual que el halo 4 pero con el color de la casa (ventanaColor); llamaFase = intensidad
    if (llama == 6)
    {
        float rr = length(TexCoords - vec2(0.5)) * 2.0;
        float a = pow(clamp(1.0 - rr, 0.0, 1.0), 2.4) * llamaFase;
        if (a < 0.004) discard;
        color = vec4(ventanaColor, a);
        return;
    }

    if (llama == 5)
    {
        vec4 tc = texture(texture_diffuse1, TexCoords);
        color = vec4(tc.rgb * 1.6, 1.0);
        return;
    }

    // X-49: emisivo con textura (llama = 5): llama del modelo fuego y mariposas. llamaFase = factor de brillo (sin luz ni sombra)
    if (llama == 5)
    {
        vec4 tc = texture(texture_diffuse1, TexCoords);
        color = vec4(tc.rgb * llamaFase, 1.0);
        return;
    }

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
    vec3 albedo = texColor.rgb * tinteObjeto;
    if (espuma == 1)
    {
        // X-22: franja de espuma donde el mar toca la orilla (z ~ 24): ruido de Perlin que avanza hacia la playa; f en [0, 1]
        float borde = smoothstep(26.2, 24.3, FragPos.z);
        float n = ruidoFbm(vec2(FragPos.x * 0.7, FragPos.z * 0.9 - tiempo * 0.45));
        float f = borde * smoothstep(0.02, 0.30, n + 0.18 + 0.18 * sin(tiempo * 0.8 - FragPos.z * 1.6));
        albedo = mix(albedo, vec3(0.80, 0.88, 0.95), clamp(f, 0.0, 0.85));
    }
    if (colina == 1)
    {
        // X-36: la textura se repite cada 10 m y las colinas se veian como una sabana lisa. Dos octavas de ruido en el mundo la tinen:
        // manchas de ~40 m (selva oscura <-> pasto seco ocre; mas seco en lo alto) y de ~5 m (matas claras y oscuras)
        // X-36 (pulido): colinas ~30 % mas claras (se veian casi negras de noche)
        albedo = colorPasto(albedo, FragPos.xz, FragPos.y);
        // X-66: cerca de z = -20 aparecen manchas de arena (misma funcion de peso que la playa: el borde ya no es una recta)
        float wA = pesoArena(FragPos.xz) * mascaraPlaya(FragPos.x);   // solo en el borde de la playa
        if (wA > 0.001)
            albedo = mix(albedo, colorArena(texture(texture_mezcla, vec2(FragPos.x, -FragPos.z) * 0.5).rgb, FragPos.xz, FragPos.y), wA);
    }
    if (arena == 1)
    {
        // X-36 (pulido): con la luna azul la arena parecia concreto. Tinte calido (menos azul) y grano fino a ~25 cm; mojada = bajo y = 0
        float wA = pesoArena(FragPos.xz);
        vec3 arenaCol = colorArena(albedo, FragPos.xz, FragPos.y);
        // X-66: cerca de z = -20 entran matas de pasto en la arena (solo se muestrea el pasto donde hace falta)
        if (wA < 0.999)
            arenaCol = mix(colorPasto(texture(texture_mezcla, vec2(FragPos.x, -FragPos.z) * 0.1).rgb * tinteObjeto, FragPos.xz, FragPos.y), arenaCol, wA);
        albedo = arenaCol;
    }

    vec3 result = CalcDirLight(dirLight, norm, viewDir, albedo);
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, albedo);
    result += CalcPointLight(velasAltar, norm, FragPos, viewDir, albedo);
    for (int i = 0; i < 2; i++)                                                  // X-51
        result += CalcPointLight(altarMayor[i], norm, FragPos, viewDir, albedo);
    for (int i = 0; i < 2; i++)                                                  // X-34
        result += CalcPointLight(luzCalida[i], norm, FragPos, viewDir, albedo);
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, albedo);
    for (int i = 0; i < NUMBER_OF_STEP_SPOTS; i++)
        result += CalcSpotLight(escalonSpots[i], norm, FragPos, viewDir, albedo);

    // X-18: bruma de distancia. f = 1 - exp(-(d * k)^1.5): 0 cerca, tiende a 1 lejos; se mezcla con el color del horizonte del cielo
    float distCam = length(viewPos - FragPos);
    float bruma = 1.0 - exp(-pow(distCam * 0.010, 1.5));          // X-36: 0.012 -> 0.010 (mas alcance de vista)
    vec3 haciaFondo = normalize(FragPos - viewPos);
    vec3 colBruma = mix(vec3(0.08, 0.13, 0.21), vec3(0.24, 0.16, 0.16), 0.6 * smoothstep(-0.2, 0.9, haciaFondo.z));   // X-36: calida hacia el ocaso (+z)
    if (espuma == 1)
    {
        // Mar (17/11): lejos se funde del todo con colHorizonte = el color del cielo EN el horizonte (el mismo que sky.frag usa debajo de
        // la linea del horizonte), asi no hay franja oscura ni borde de malla: el agua se pierde en la bruma del ocaso como un mar infinito.
        vec3 colHorizonte = vec3(0.10, 0.17, 0.26) + vec3(0.55, 0.26, 0.10) * 0.55 * (0.35 + 0.65 * smoothstep(-0.2, 0.9, haciaFondo.z));
        float fondoMar = max(bruma, smoothstep(55.0, 115.0, distCam));
        result = mix(result, colHorizonte, clamp(fondoMar, 0.0, 1.0));
    }
    else
        result = mix(result, colBruma, clamp(bruma, 0.0, 0.85));
    if (esVidrio == 1)
    {
        // X-56: emision de la ventana. Se suma DESPUES de la bruma (con la mitad de su efecto) para que las casas lejanas sigan brillando.
        // El vidrio apagado queda oscuro (0.5 de lo que recibe de la noche).
        float b = (LocalPos.x < 0.0) ? ventanaBrillo.x : ventanaBrillo.y;
        // q = posicion dentro del vidrio (m) respecto al centro de su ventana (x = +-1.65, y = 1.55 en el modelo de la casa)
        vec2 q = LocalPos.xy - vec2(LocalPos.x < 0.0 ? -1.65 : 1.65, 1.55);
        float cruz = (abs(q.x) < 0.025 || abs(q.y) < 0.025) ? 0.30 : 1.0;                 // travesanos de madera en cruz (la luz no pasa)
        float centro = 1.0 - 0.45 * smoothstep(0.18, 0.55, length(q * vec2(1.0, 0.9)));  // mas claro al centro, mas tenue en los bordes
        result *= 0.5;
        result += ventanaColor * b * 1.5 * cruz * centro * (1.0 - 0.5 * clamp(bruma, 0.0, 0.85));
    }
    float alfaFinal = texColor.a;
    if (humo == 1) alfaFinal *= humoAlfa;   // X-47
    color = vec4(result, alfaFinal);
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
