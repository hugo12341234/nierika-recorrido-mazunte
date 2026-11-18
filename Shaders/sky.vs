#version 330 core
// Cielo nocturno procedural: un triangulo que cubre la pantalla; calcula la direccion de la vista de cada pixel.
const vec2 POS[3] = vec2[3](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));

uniform mat4 invProjView; // inversa de projection * view (la traslacion de la camara se descarta en el fragment)
out vec3 dirVista;

void main()
{
    vec2 p = POS[gl_VertexID];
    gl_Position = vec4(p, 0.9999, 1.0);              // casi en el plano lejano (con 1.0 exacto fallaria GL_LESS)
    vec4 lejos = invProjView * vec4(p, 1.0, 1.0);
    dirVista = lejos.xyz / lejos.w;
}
