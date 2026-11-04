#include "Animacion.h"

#include <cmath>

namespace anim
{
    // smoothstep clasico: S(u) = 3u^2 - 2u^3. S(0)=0, S(1)=1, S'(0)=S'(1)=0, S'(1/2)=1.5
    static float S(float u)
    {
        return u * u * (3.0f - 2.0f * u);
    }

    float Suavizar(float u, Curva curva)
    {
        u = glm::clamp(u, 0.0f, 1.0f);
        switch (curva)
        {
        case Curva::Smoothstep: return S(u);
        case Curva::SmoothIn:   return 2.0f * S(0.5f * u);               // mitad izquierda de S, reescalada a [0,1]
        case Curva::SmoothOut:  return 2.0f * S(0.5f * (u + 1.0f)) - 1.0f; // mitad derecha de S, reescalada a [0,1]
        default:                return u;
        }
    }

    glm::quat Slerp(const glm::quat& a, const glm::quat& b, float s)
    {
        glm::quat q2 = b;
        float c = glm::dot(a, b);          // cos(theta) = a . b
        if (c < 0.0f)                      // camino corto: q y -q representan la misma rotacion
        {
            q2 = -b;
            c = -c;
        }
        if (c > 0.9995f)                   // casi iguales: sin(theta) ~ 0, se usa nlerp
            return glm::normalize(glm::quat(
                a.w + s * (q2.w - a.w), a.x + s * (q2.x - a.x),
                a.y + s * (q2.y - a.y), a.z + s * (q2.z - a.z)));

        float theta = std::acos(c);
        float sinTheta = std::sin(theta);
        float wa = std::sin((1.0f - s) * theta) / sinTheta;
        float wb = std::sin(s * theta) / sinTheta;
        return glm::quat(wa * a.w + wb * q2.w, wa * a.x + wb * q2.x,
                         wa * a.y + wb * q2.y, wa * a.z + wb * q2.z);
    }

    void Pista::Agregar(float t, const glm::vec3& pos, const glm::quat& rot, Curva curva)
    {
        kf_.push_back({ t, pos, rot, curva });
    }

    float Pista::Duracion() const
    {
        return kf_.size() < 2 ? 0.0f : kf_.back().t - kf_.front().t;
    }

    Pose Pista::Evaluar(float t) const
    {
        Pose p;
        if (kf_.empty()) return p;
        if (kf_.size() == 1) { p.pos = kf_[0].pos; p.rot = kf_[0].rot; return p; }

        float t0 = kf_.front().t;
        float dur = Duracion();
        if (ciclica_ && dur > 0.0f)
            t = t0 + std::fmod(std::fmod(t - t0, dur) + dur, dur);  // t mod duracion, tambien para t negativo
        else
            t = glm::clamp(t, t0, kf_.back().t);                    // fuera del rango se queda en el primer/ultimo KeyFrame

        // Tramo [i, i+1] que contiene a t (pocos KeyFrames: busqueda lineal)
        size_t i = 0;
        while (i + 2 < kf_.size() && t >= kf_[i + 1].t) i++;

        const KeyFrame& a = kf_[i];
        const KeyFrame& b = kf_[i + 1];
        float u = (b.t > a.t) ? (t - a.t) / (b.t - a.t) : 1.0f;
        float s = Suavizar(u, a.curva);

        p.pos = glm::mix(a.pos, b.pos, s);
        p.rot = Slerp(a.rot, b.rot, s);
        return p;
    }
}
