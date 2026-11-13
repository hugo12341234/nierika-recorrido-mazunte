#version 330 core
// Cielo nocturno: degradado horizonte -> cenit, estrellas por hash de celda y disco de luna con halo.
in vec3 dirVista;
out vec4 color;

uniform vec3 camPos;
uniform vec3 dirLuna;   // direccion HACIA la luna (unitaria)

float hash(vec3 p)
{
    p = fract(p * 0.3183099 + 0.1);
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

void main()
{
    vec3 d = normalize(dirVista - camPos);
    float h = clamp(d.y, 0.0, 1.0);

    // Degradado: horizonte azul verdoso apagado -> cenit azul noche (mezcla con curva suave)
    vec3 horizonte = vec3(0.10, 0.17, 0.26);
    vec3 cenit     = vec3(0.01, 0.03, 0.10);
    vec3 cielo = mix(horizonte, cenit, pow(h, 0.45));

    // X-22: resplandor del ocaso sobre el mar (la propuesta representa 'del atardecer a la noche'): mas intenso cerca del horizonte
    // y hacia +z (el mar). ocaso = exp(-9 h) * (0.35 + 0.65 * smoothstep(-0.2, 0.9, d.z))
    float ocaso = exp(-9.0 * h) * (0.35 + 0.65 * smoothstep(-0.2, 0.9, d.z));
    cielo += vec3(0.55, 0.26, 0.10) * ocaso * 0.55;

    // Estrellas: rejilla 3D sobre la direccion; una celda de cada ~30 tiene una estrella, mas visibles arriba
    vec3 celda = floor(d * 220.0);
    float e = hash(celda);
    vec3 centro = fract(d * 220.0) - 0.5;
    float punto = 1.0 - smoothstep(0.0, 0.32, length(centro));   // punto redondo dentro de la celda
    float estrella = step(0.988, e) * punto * smoothstep(0.05, 0.4, h);
    cielo += vec3(0.9, 0.95, 1.0) * estrella * (0.5 + 0.5 * hash(celda + 7.0));

    // Luna: disco y halo
    float c = dot(d, normalize(dirLuna));
    cielo += vec3(0.95, 0.93, 0.80) * smoothstep(0.9990, 0.9995, c);
    cielo += vec3(0.25, 0.30, 0.45) * pow(max(c, 0.0), 80.0);

    // Debajo del horizonte se funde con el color del horizonte (la playa y el mar tapan casi todo)
    if (d.y < 0.0) cielo = horizonte * 0.6;

    color = vec4(cielo, 1.0);
}
