#ifndef EXTRAS_H
#define EXTRAS_H

// X-19..X-22: datos y formulas de los extras de ambiente (fase "mejorar y producir", ver docs/EXTRAS_Y_MEJORAS.md).
// Solo <cmath>: se puede probar aparte (tools/x19_pruebas_extras.cpp). SIN KeyFrames: todo es matematica simple.

#include <cmath>

namespace Extras
{
    constexpr float PI = 3.14159265358979f;

    // Balanceo senoidal: theta(t) = A * sen(2*pi*t/T + fase), en grados. Lo usan las figuras (balanceo suave sobre su pivote)
    // y los tendederos de papel picado (pendulo de pequena amplitud).
    inline float Balanceo(float t, float periodo, float fase, float amplitudGrados)
    {
        return amplitudGrados * std::sin(2.0f * PI * t / periodo + fase);
    }

    // Figuras de la ceremonia (poses FIJAS; no hay IA). tipo: 0 persona_pie_a, 1 persona_pie_b, 2 persona_agachada_a, 3 persona_agachada_b.
    struct Figura { float x, z, giroY; int tipo; float fase; };
    constexpr int N_FIGURAS = 12;
    // giroY = 180: mira a +z (hacia el mar). Los petates estan en z ~ 20 (la playa es plana hasta z = +20)
    inline const Figura FIGURAS[N_FIGURAS] = {
        {  8.0f, 19.5f, 180.0f, 2, 0.0f }, {  8.7f, 19.5f, 180.0f, 3, 1.1f },     // junto al petate 1
        { 11.0f, 18.7f, 180.0f, 3, 2.0f }, { 11.8f, 18.8f, 175.0f, 2, 0.4f },     // junto al petate 2
        { 14.0f, 19.7f, 180.0f, 2, 3.1f }, { 14.7f, 19.6f, 185.0f, 3, 4.2f },     // junto al petate 3
        {  7.2f, 17.6f, 180.0f, 0, 0.7f }, { 10.0f, 17.0f, 175.0f, 1, 1.9f },     // de pie, detras de los que rezan
        { 12.8f, 17.4f, 185.0f, 0, 2.6f }, { 15.5f, 18.0f, 180.0f, 1, 3.8f },
        {  5.0f, 20.8f, 150.0f, 1, 5.0f }, { 17.2f, 20.9f, 210.0f, 0, 0.3f }      // mirando el mar
    };
    // Petates con ofrenda (centro x, z y giro)
    struct Petate { float x, z, giroY; };
    constexpr int N_PETATES = 3;
    inline const Petate PETATES[N_PETATES] = { { 8.4f, 20.35f, 0.0f }, { 11.4f, 19.55f, 0.0f }, { 14.35f, 20.55f, 0.0f } };

    // X-20: tendederos de papel picado que cruzan la playa (propuesta, tabla 2.1). Cada tendedero es una hilera de N bastidores
    // de Models/soportes (1.85 m de ancho, postes de 2 m) con una tira de Models/papel_picado colgada de cada uno.
    struct Tendedero { float x0, z; int n; };
    constexpr int N_TENDEDEROS = 2;
    inline const Tendedero TENDEDEROS[N_TENDEDEROS] = { { -22.0f, 15.0f, 9 }, { 4.0f, 14.2f, 8 } };
    constexpr float ANCHO_BASTIDOR = 1.85f;   // m (x -0.925..+0.925 en soportes.obj)
    constexpr float ALTO_PAPEL = 1.95f;       // m: la tira cuelga de aqui (pivote en su borde superior)

    // Ofrendas pequenas al pie de las tumbas y la cruz del panteon (x, z): una vela y tres flores de cempasuchil
    struct OfrendaTumba { float x, z; };
    constexpr int N_OFRENDAS_TUMBA = 5;
    inline const OfrendaTumba OFRENDAS_TUMBA[N_OFRENDAS_TUMBA] = { { -4.0f, 5.75f }, { 4.0f, 6.2f }, { -4.5f, 9.95f }, { 4.5f, 10.6f }, { -2.5f, 4.1f } };

    // X-21: personas sobre tablas de remo en el mar (fotos de referencia de D2). La tabla recorre un circulo lento de radio R:
    //   (x, z)(t) = (cx + R*cos(w*t + fase), cz + R*sen(w*t + fase)),  w = 2*pi / PERIODO_TABLA
    // y sigue la ola: y = Olas::Altura(x, z, t) y su vertical se alinea con la normal Olas::Normal (cabeceo y alabeo). Sin KeyFrames.
    struct Tabla { float cx, cz, radio, fase; int persona; };   // persona: 0 persona_pie_a, 1 persona_pie_b
    constexpr int N_TABLAS = 5;
    constexpr float PERIODO_TABLA = 70.0f;   // s por vuelta
    inline const Tabla TABLAS[N_TABLAS] = { { -14.0f, 30.0f, 3.0f, 0.0f, 0 }, { -5.0f, 36.0f, 4.0f, 1.2f, 1 }, { 6.0f, 29.0f, 3.0f, 2.4f, 0 },
                                            { 15.0f, 38.0f, 4.0f, 3.6f, 1 }, { 26.0f, 33.0f, 3.0f, 4.8f, 0 } };
    // Posicion y direccion de avance (dx, dz) de la tabla i en el instante t
    inline void PosicionTabla(const Tabla& b, float t, float& x, float& z, float& dx, float& dz)
    {
        const float w = 2.0f * PI / PERIODO_TABLA, a = w * t + b.fase;
        x = b.cx + b.radio * std::cos(a);  z = b.cz + b.radio * std::sin(a);
        dx = -std::sin(a);  dz = std::cos(a);   // tangente unitaria (la tabla avanza de proa)
    }

    // X-22: luciernagas. Cada una vuela en una curva de Lissajous 3D alrededor de su centro y parpadea con un seno elevado al cubo:
    //   p(t) = c + (Ax*sen(ax*t + px), Ay*sen(ay*t + py), Az*sen(az*t + pz)),   brillo(t) = max(0, sen(2*pi*t/Tb + fase))^3
    // Frecuencias distintas por eje (cocientes casi irracionales) para que la trayectoria no se repita pronto. No es un sistema de particulas
    // con fisica: cada punto se calcula en forma cerrada. Sin KeyFrames.
    constexpr int N_LUCIERNAGAS = 48;
    struct Luciernaga { float cx, cy, cz, fase; };
    inline Luciernaga LuciernagaN(int i)   // centros repartidos por la playa y el panteon (x -32..32, z 4..21), alturas 0.6..1.8 m
    {
        const float u = std::fmod(std::sin(12.9898f * (i + 1)) * 43758.5453f, 1.0f), v = std::fmod(std::sin(78.233f * (i + 1)) * 12345.6789f, 1.0f);
        const float a = u < 0 ? -u : u, b = v < 0 ? -v : v;
        return { -32.0f + 64.0f * a, 0.6f + 1.2f * b, 4.0f + 17.0f * std::fmod(a * 7.31f + b * 3.17f, 1.0f), 6.2832f * std::fmod(a * 13.7f, 1.0f) };
    }
    inline void PosicionLuciernaga(int i, float t, float& x, float& y, float& z)
    {
        const Luciernaga l = LuciernagaN(i);
        const float k = 1.0f + 0.07f * (i % 5);
        x = l.cx + 1.6f * std::sin(0.41f * k * t + l.fase);
        y = l.cy + 0.45f * std::sin(0.67f * k * t + 2.0f * l.fase);
        z = l.cz + 1.6f * std::sin(0.53f * k * t + 3.0f * l.fase);
    }
    inline float BrilloLuciernaga(int i, float t)
    {
        const float s = std::sin(2.0f * PI * t / 2.6f + LuciernagaN(i).fase * 3.0f);
        return s > 0.0f ? s * s * s : 0.0f;
    }

    // ======== X-24: NARRATIVA EN 5 ESTACIONES (del atardecer a la noche) ========
    // E1 llegada a la playa (z ~ 21.6, x < 0) · E2 ofrendas y velas al mar (ceremonia X-19, musico) · E3 sendero de cempasuchil y papel picado
    // (x ~ 0, z 22 -> 2) · E4 panteon con familias ante las tumbas · E5 la puerta del cuarto y el altar. Coinciden con las 5 fichas.
    // Figuras en POSES FIJAS (sin IA) con balanceo senoidal; escala 1 = adulto, ~0.6 = nino. inst: 0 ninguno, 1 guitarra (musico).
    struct FiguraN { float x, z, giroY; int tipo; float fase; float esc; int inst; };
    constexpr int N_FIGURAS_N = 24;
    inline const FiguraN FIGURAS_N[N_FIGURAS_N] = {
        // E1 llegada: una familia que mira el mar y una abuela que deja velas en la orilla
        { -7.0f, 21.6f, 175.0f, 0, 0.2f, 1.00f, 0 }, { -6.2f, 21.7f, 185.0f, 1, 1.4f, 1.00f, 0 }, { -6.6f, 21.2f, 180.0f, 0, 2.6f, 0.62f, 0 },
        { -5.6f, 21.4f, 178.0f, 1, 3.3f, 0.58f, 0 }, { -9.4f, 21.8f, 180.0f, 3, 0.9f, 0.95f, 0 },
        // E2: musico con guitarra y un nino junto a la ceremonia
        {  2.7f, 19.0f, 270.0f, 0, 1.0f, 1.00f, 1 }, {  3.7f, 19.8f, 265.0f, 1, 2.2f, 0.60f, 0 },
        // E3 sendero: una familia que camina hacia el panteon (a >= 1.3 m del eje x = 0 de la caminata)
        { -1.5f, 18.0f,   0.0f, 0, 0.4f, 1.00f, 0 }, { -2.3f, 17.9f,   5.0f, 1, 1.8f, 0.60f, 0 }, {  1.6f, 16.2f, 355.0f, 1, 2.9f, 1.00f, 0 },
        // E4 panteon: cada tumba tiene visitantes
        { -4.3f,  6.6f,   0.0f, 2, 0.5f, 1.00f, 0 }, { -3.3f,  6.7f,   0.0f, 0, 1.7f, 0.60f, 0 },                     // tumba 2: abuela y nino
        {  3.5f,  7.3f,   0.0f, 0, 2.4f, 1.00f, 0 }, {  4.5f,  7.2f,   0.0f, 1, 3.6f, 1.00f, 0 }, {  4.0f,  7.0f,   0.0f, 0, 4.4f, 0.60f, 0 },  // tumba 3: familia
        { -5.0f, 10.9f,   0.0f, 2, 0.8f, 1.00f, 0 }, { -4.0f, 10.9f,   0.0f, 3, 2.0f, 0.95f, 0 },                     // tumba 4: dos personas de rodillas
        {  5.9f, 11.0f, 200.0f, 1, 1.1f, 1.00f, 1 }, {  3.3f, 11.0f, 170.0f, 0, 2.7f, 1.00f, 0 },                     // tumba 1: musico y acompanante
        { -2.5f,  5.1f,   0.0f, 3, 3.9f, 0.95f, 0 },                                                                   // la cruz de la entrada
        // E5 puerta del cuarto: una abuela con su vela y un nino esperan afuera
        {  1.9f,  2.7f,   0.0f, 2, 1.3f, 0.95f, 0 }, { -1.9f,  2.8f,   0.0f, 1, 4.0f, 0.60f, 0 },
        // al fondo de la playa oeste: pareja con tabla al hombro mirando el mar
        { -16.0f, 19.0f, 160.0f, 0, 0.1f, 1.00f, 0 }, { -15.1f, 19.2f, 170.0f, 1, 1.0f, 1.00f, 0 }
    };
    // Cestas con flores (x, z): llegada y panteon
    struct Cesta { float x, z; };
    constexpr int N_CESTAS = 4;
    inline const Cesta CESTAS[N_CESTAS] = { { -6.4f, 21.0f }, { 4.5f, 8.0f }, { -3.2f, 11.4f }, { 2.6f, 18.4f } };
    // Faroles (poste con linterna; la luz es una esfera emisiva que parpadea): x, z
    constexpr int N_FAROLES = 16;
    inline const Cesta FAROLES[N_FAROLES] = {
        { -3.2f, 22.3f }, { -11.0f, 21.8f }, { 4.4f, 21.8f }, { 18.8f, 21.4f },                      // E1, E2
        { -2.2f, 19.4f }, { 2.2f, 17.2f }, { -2.2f, 14.6f }, { 2.2f, 13.0f },                        // E3 sendero
        { -6.2f, 11.2f }, { 6.2f, 11.2f }, { -6.2f, 4.3f }, { 6.2f, 4.3f },                          // E4 esquinas del panteon
        { -1.8f, 2.0f }, { 1.8f, 2.0f }, { -9.0f, 18.0f }, { 9.0f, 14.0f }                           // E5 puerta, y dos en la playa
    };
    // Camino de cempasuchil de la orilla a la puerta: el eje serpentea x(z) = 0.9*sen(0.35*z) y cada paso de 0.28 m deja una flor
    // con dispersion deterministica de +-0.45 m. Las flores no se dibujan sobre los muros de la barda.
    inline float EjeSendero(float z) { return 0.9f * std::sin(0.35f * z); }
    constexpr float Z_SENDERO_ORILLA = 22.4f, Z_SENDERO_PUERTA = 2.3f, PASO_SENDERO = 0.28f;
    inline int NumFloresSendero() { return (int)((Z_SENDERO_ORILLA - Z_SENDERO_PUERTA) / PASO_SENDERO); }
    inline bool FlorSenderoEn(int i, float& x, float& z)
    {
        z = Z_SENDERO_ORILLA - PASO_SENDERO * i;
        const float r1 = std::fmod(std::fabs(std::sin(12.9898f * i + 1.3f) * 43758.5453f), 1.0f);
        const float r2 = std::fmod(std::fabs(std::sin(78.233f * i + 4.1f) * 12345.6789f), 1.0f);
        x = EjeSendero(z) + (r1 * 2.0f - 1.0f) * 0.45f;
        z += (r2 * 2.0f - 1.0f) * 0.12f;
        const bool muroSur = z > 2.9f && z < 3.35f, muroNorte = z > 11.7f && z < 12.05f;
        return !((muroSur || muroNorte) && std::fabs(x) > 0.95f);
    }

    // Murcielagos al anochecer: vuelan en circulos (centro, radio, altura) a velocidad lineal v; las alas baten con theta = 40 grados * sen(2*pi*f*t)
    struct Murcielago { float cx, cz, radio, altura, fase; };
    constexpr int N_MURCIELAGOS = 7;
    constexpr float VEL_MURCIELAGO = 4.5f, FREC_ALETEO = 3.2f, AMP_ALETEO = 40.0f;
    inline const Murcielago MURCIELAGOS[N_MURCIELAGOS] = { { 0.0f, 8.0f, 14.0f, 8.0f, 0.0f }, { 6.0f, 10.0f, 18.0f, 11.0f, 1.9f }, { -8.0f, 6.0f, 22.0f, 9.5f, 3.4f },
                                                          { 12.0f, 14.0f, 12.0f, 7.0f, 4.7f }, { -14.0f, 16.0f, 16.0f, 12.5f, 2.6f }, { 2.0f, 2.0f, 26.0f, 14.0f, 5.5f },
                                                          { 0.0f, 20.0f, 10.0f, 6.5f, 0.8f } };
    inline void PosicionMurcielago(const Murcielago& m, float t, float& x, float& y, float& z, float& dx, float& dz)
    {
        const float a = VEL_MURCIELAGO / m.radio * t + m.fase;
        x = m.cx + m.radio * std::cos(a);  z = m.cz + m.radio * std::sin(a);
        y = m.altura + 0.8f * std::sin(0.7f * t + m.fase * 2.0f);
        dx = -std::sin(a);  dz = std::cos(a);
    }
}

#endif
