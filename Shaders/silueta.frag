#version 330 core
// Cubos PROVISIONALES de la silueta (D-1/D-2): color plano con una luz fija de la luna para que se lea el volumen.
// Se sustituye por lighting.frag cuando exista el modelo de la silueta.
in vec3 Normal;
out vec4 FragColor;

uniform vec3 color;

void main()
{
    vec3 luz = normalize(vec3(0.3f, 1.0f, 0.5f));
    float difusa = max(dot(normalize(Normal), luz), 0.0f);
    FragColor = vec4(color * (0.35f + 0.65f * difusa), 1.0f);
}
