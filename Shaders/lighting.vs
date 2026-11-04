#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// C-1: oleaje (suma de senos). Solo el mar lo activa (oleaje = 1); el resto de los modelos no cambia.
// Los parametros vienen de src/Olas.h, que calcula la misma altura en CPU.
#define NUMERO_ONDAS 3
uniform int   oleaje;
uniform float tiempo;                   // segundos
uniform float ondaA[NUMERO_ONDAS];      // amplitud A_i (m)
uniform float ondaK[NUMERO_ONDAS];      // numero de onda k_i = 2*pi/lambda_i (rad/m)
uniform float ondaW[NUMERO_ONDAS];      // frecuencia angular w_i = sqrt(g*k_i) (rad/s)
uniform vec2  ondaDir[NUMERO_ONDAS];    // direccion de avance d_i (unitaria, en x,z)
uniform float ondaFase[NUMERO_ONDAS];   // fase inicial phi_i (rad)

void main()
{
    vec3 pos = position;
    vec3 norm = normal;
    if (oleaje == 1)
    {
        // h = h0 + sum A_i sin(k_i d_i.(x,z) - w_i t + phi_i); la malla trae x,z en coordenadas del mundo
        float dhdx = 0.0;
        float dhdz = 0.0;
        for (int i = 0; i < NUMERO_ONDAS; i++)
        {
            float arg = ondaK[i] * dot(ondaDir[i], pos.xz) - ondaW[i] * tiempo + ondaFase[i];
            pos.y += ondaA[i] * sin(arg);
            float c = ondaA[i] * ondaK[i] * cos(arg);
            dhdx += c * ondaDir[i].x;
            dhdz += c * ondaDir[i].y;
        }
        norm = normalize(vec3(-dhdx, 1.0, -dhdz));   // normal analitica: gradiente de h
    }

    gl_Position = projection * view * model * vec4(pos, 1.0f);
    FragPos = vec3(model * vec4(pos, 1.0f));
    // Matriz normal: transpuesta de la inversa, para que la normal siga bien con escalas no uniformes
    Normal = mat3(transpose(inverse(model))) * norm;
    TexCoords = texCoords;
}
