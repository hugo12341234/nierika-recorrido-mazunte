// C-3 (act. 41): velas flotando mar adentro. Animacion simple (NO es KeyFrames): trayectoria de Bezier cuadratica
// + oscilacion senoidal vertical con desfase por vela. Todo es funcion cerrada del tiempo t (no acumula deltaTime):
// el resultado no depende de los FPS. Solo <cmath>: no usa GLM ni GPU (la matriz de modelo se arma en main.cpp).
#pragma once

#include <cmath>

// Parametros de una vela
struct VelaFlotante
{
    float p0x, p0z;     // P0: punto de salida en la orilla (m)
    float p1x, p1z;     // P1: punto de control (m)
    float p2x, p2z;     // P2: punto mas lejano, mar adentro (m)
    float periodo;      // T: duracion del viaje de ida y vuelta (s)
    float desfase;      // t0: adelanto de tiempo para que no salgan todas a la vez (s)
    float amp;          // a: amplitud del vaiven vertical (m)
    float omegaV;       // omega_v = 2 pi / T_v (rad/s)
    float faseV;        // phi_j: desfase del vaiven de la vela j (rad)
};

const float PI_VELAS = 3.14159265358979f;

// Parametro de la curva u(t) en [0, 1]: va y vuelve suavemente (marea que se lleva la vela y la regresa).
//     u(t) = 0.5 - 0.5 cos(2 pi (t + t0) / T)
// u(0) = 0 (orilla), u(T/2) = 1 (mar adentro), u(T) = 0; du/dt = (pi/T) sin(2 pi (t + t0)/T) vale 0 en los extremos,
// asi la vela arranca y se detiene sin saltos y no hay reinicio visible.
inline float ParametroVela(const VelaFlotante& v, float t)
{
    return 0.5f - 0.5f * std::cos(2.0f * PI_VELAS * (t + v.desfase) / v.periodo);
}

// Bezier cuadratica en XZ:  B(u) = (1-u)^2 P0 + 2 (1-u) u P1 + u^2 P2
inline void PosicionVelaXZ(const VelaFlotante& v, float t, float& x, float& z)
{
    const float u = ParametroVela(v, t);
    const float a = (1.0f - u) * (1.0f - u), b = 2.0f * (1.0f - u) * u, c = u * u;
    x = a * v.p0x + b * v.p1x + c * v.p2x;
    z = a * v.p0z + b * v.p1z + c * v.p2z;
}

// Vaiven vertical respecto al agua:  dy(t) = a sin(omega_v t + phi_j)
// (en main.cpp: y = Olas::Altura(x, z, t) + CALADO_VELA + dy, asi sigue tambien a las olas de C-1)
inline float VaivenVela(const VelaFlotante& v, float t)
{
    return v.amp * std::sin(v.omegaV * t + v.faseV);
}

const float CALADO_VELA = -0.03f;   // m: la base de la vela (0.15 m de alto) queda 3 cm bajo la superficie
