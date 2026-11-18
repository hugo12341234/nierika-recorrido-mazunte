// Recorrido.cpp - X-62: ver Recorrido.h
#include "Recorrido.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "Audio.h"
#include "Colisiones.h"

namespace
{
    const float PI = 3.14159265358979f;

    // Una estacion: donde se detiene la camara, a donde mira, cuanto dura y que se dice.
    // Coordenadas en planta (x, z) en metros; los ojos van a 1.6 m del suelo (Colisiones::alturaTerreno + ALTURA_OJOS).
    // 'mira' = (x, altura sobre el suelo, z) del punto observado. Las posiciones salen de Extras.h, Fichas.cpp, PlazaTrasera.h, OfrendasFamiliares.h y el ESTADO.
    struct Parada
    {
        const char* titulo;
        const char* texto;            // lo que dice la voz (sin marcas); sin raya larga ni comillas tipograficas (ImGui)
        float tramo;                  // s para llegar desde la estacion anterior (<= 0: se calcula por distancia, solo la primera)
        float reten;                  // s detenida, solo si no hay voz (con voz: duracion del mp3 + 1.5 s)
        int nVia;                     // puntos de paso del tramo (Catmull-Rom)
        glm::vec2 via[4];
        glm::vec2 llega;              // donde se detiene
        glm::vec2 fin;                // donde esta al terminar el reten (travelling lento); igual a llega = quieta
        glm::vec3 mira;               // punto observado al llegar
        glm::vec3 miraFin;            // punto observado al terminar el reten
        float giro;                   // grados extra de giro de la cabeza durante el reten (0 = ninguno)
        float pitchFin;               // con giro != 0: pitch final
        bool miraComparsa;            // true: mira al centro de la comparsa (se mueve sola)
    };

    // Textos: el contenido de cada afirmacion sale de las fichas culturales (Fichas.cpp) y del guion de docs/RECORRIDO_GUIADO.md; lo que es adaptacion del estudio se dice como tal.
    const Parada E[Recorrido::N_ESTACIONES] = {
        { "Mazunte y el mar",
          "Oaxaca... una costa, una noche, y una fiesta que se vive con más alma. "
          "Estás en la playa de Mazunte. Hubo un tiempo en que la explotación de la tortuga fue la actividad principal de este pueblo, hasta que en mayo de 1990 se decretó la veda total; desde entonces, Mazunte cuida a sus tortugas, según México Desconocido. "
          "Pero esta noche venimos por otra razón: es noche de muertos, y los que se fueron... vuelven a casa. Ven, camina conmigo.",
          0.0f, 34.0f, 0, {}, { 19.5f, 18.6f }, { 19.5f, 18.6f }, { 22.0f, 0.6f, 23.5f }, { 22.0f, 0.6f, 23.5f }, 0.0f, 0.0f, false },
        { "Velas y flores al mar",
          "En la ceremonia local que se vive al atardecer, se lanzan velas y flores al mar para recordar a quienes partieron, cuenta Baños, en dos mil veinticinco. "
          "La flama de cada vela guía a los difuntos de regreso a casa, y el aroma de las flores, sobre todo del cempasúchil, los guía hacia el altar. Así lo explica la Secretaría de Educación Pública. "
          "Mira cómo la luz se va con el agua.",
          8.7f, 32.0f, 0, {}, { -1.0f, 23.3f }, { -1.0f, 23.3f }, { 9.0f, 0.5f, 21.0f }, { 9.0f, 0.5f, 21.0f }, 0.0f, 0.0f, false },
        { "Comparsa en la orilla",
          "Y mientras el mar se lleva las velas, por la orilla llega la música. En la Gran Comparsa del Día de Muertos de la ciudad de Oaxaca desfilan más de cincuenta contingentes, entre catrinas, esqueletos y disfraces de papel, con música y color, según La Razón. La muerte, vestida de fiesta.",
          6.3f, 23.0f, 0, {}, { -9.5f, 18.4f }, { -9.5f, 18.4f }, { -16.0f, 1.0f, 22.6f }, { -16.0f, 1.0f, 22.6f }, 0.0f, 0.0f, true },
        { "Camino de flores al arco",
          "Ahora sigue el camino de flores hasta la puerta. Ese papel picado que se mece sobre el sendero le da color a la ofrenda y representa el aire, dice la Secretaría de Educación Pública. Fíjate en la figura que va caminando rumbo al altar: lleva una flor para dejarla ahí.",
          6.5f, 23.0f, 0, {}, { -0.8f, 20.5f }, { -1.6f, 7.5f }, { 0.0f, 1.6f, 1.3f }, { 0.0f, 1.6f, 1.3f }, 0.0f, 0.0f, false },
        { "Banda de viento",
          "¿Los oyes? Son metales. Las bandas de viento son una tradición oaxaqueña: dan identidad y hermanan a los pueblos indígenas del estado, y acompañan sus fiestas comunales, según México Desconocido. Su música llena la noche.",
          7.0f, 17.0f, 2, { { 1.0f, 12.8f }, { 7.5f, 12.5f } }, { 13.0f, 12.2f }, { 13.0f, 12.2f }, { 14.0f, 1.3f, 8.0f }, { 14.0f, 1.3f, 8.0f }, 0.0f, 0.0f, false },
        { "Panteón",
          "En las alumbradas de Oaxaca, las familias pasan la noche del treinta y uno de octubre junto a las tumbas de sus seres queridos, y las velas sobre las lápidas iluminan, de forma simbólica, el camino de regreso, según México Desconocido. Los platillos del difunto se colocan alrededor del altar familiar y de la tumba, dice la UNESCO.",
          6.0f, 26.0f, 1, { { 6.5f, 4.2f } }, { 2.6f, 7.0f }, { -2.6f, 7.0f }, { 4.0f, 0.7f, 6.2f }, { -4.0f, 0.7f, 5.75f }, 0.0f, 0.0f, false },
        { "Copal",
          "Antes de entrar, un aroma: el copal. Forma parte de la ofrenda, junto con las velas, las flores y el pan, según la Secretaría de Educación Pública. En el panteón tepehua de Pisaflores, en Veracruz, las tumbas se adornan en medio del humo del copal, informó el INAH.",
          4.5f, 22.0f, 0, {}, { 0.0f, 3.3f }, { 0.0f, 3.3f }, { 0.0f, 1.0f, 1.2f }, { 0.0f, 1.0f, 1.2f }, 0.0f, 0.0f, false },
        { "El altar",
          "Y llegamos al altar. Cada objeto cumple una función para recibir a las ánimas: velas, flores, pan, agua y sal, y copal, entre otros, según la Secretaría de Educación Pública. El pan de muerto representa el afecto y la fraternidad hacia los difuntos. Mira... llega la figura del camino, y deja su flor.",
          6.0f, 24.0f, 2, { { 0.0f, 2.2f }, { 0.0f, 1.2f } }, { 0.85f, 0.55f }, { 0.85f, 0.55f }, { 0.0f, 0.9f, -0.7f }, { 0.0f, 0.9f, -0.7f }, 0.0f, 0.0f, false },
        { "Ofrendas de las familias",
          "Afuera, detrás de la capilla, cada familia prepara su ofrenda a su manera. Los platillos del difunto se colocan alrededor del altar y de la tumba, y la flama de las velas y el aroma de las flores guían a las almas de regreso, según la UNESCO y la Secretaría de Educación Pública. Fíjate en los objetos que cada familia deja para su difunto.",
          11.0f, 29.0f, 4, { { 0.0f, 1.9f }, { 0.0f, 3.0f }, { -3.2f, 3.6f }, { -3.2f, -3.0f } }, { -3.2f, -8.5f }, { -3.2f, -8.5f }, { -6.1f, 1.0f, -13.2f }, { -6.1f, 1.0f, -13.2f }, 0.0f, 0.0f, false },
        { "Camino de las ánimas",
          "Sube conmigo por el camino de las ánimas. En las alumbradas de Oaxaca, una comparsa recorre el pueblo entre música y baile, según México Desconocido. Los faroles, el cempasúchil y las velas marcan el camino.",
          11.0f, 17.0f, 2, { { -3.0f, -16.0f }, { 0.0f, -21.0f } }, { 0.5f, -26.6f }, { 0.8f, -30.5f }, { 0.5f, 1.2f, -32.0f }, { 0.8f, 1.2f, -36.0f }, 0.0f, 0.0f, false },
        { "Mariposas monarca",
          "Y entre las luces, mariposas. Según una creencia asociada al pueblo purépecha de Michoacán, las mariposas monarca que llegan a finales de octubre y principios de noviembre son las almas de los difuntos que regresan, según Expansión Digital. Es una interpretación simbólica.",
          6.0f, 20.0f, 1, { { -0.3f, -33.0f } }, { -0.8f, -35.0f }, { -0.8f, -35.0f }, { -3.8f, 1.6f, -36.1f }, { -3.8f, 1.6f, -36.1f }, 0.0f, 0.0f, false },
        { "Mirador y cierre",
          "Desde aquí arriba se ve todo: la capilla, la playa, el mar. "
          "Así es esta fiesta: una noche en que la flama y el aroma guían a quienes se fueron de regreso a casa, y quienes se quedan los reciben con luz, con flores, con pan... y con música. "
          "Gracias por caminar con nosotros. Ahora el camino es tuyo: recórrelo a tu ritmo, acércate a cada ofrenda y lee su ficha.",
          0.0f, 31.0f, 0, {}, { -0.8f, -35.0f }, { -0.8f, -35.0f }, { -0.8f, 1.6f, -50.0f }, { -0.8f, 1.6f, -50.0f }, 180.0f, -5.0f, false },
    };

    const int ESTACION_ALTAR = 7;       // la silueta llega al altar 14 s despues de que la camara se detiene ahi (cuando la voz dice: llega la figura y deja su flor)
    const float SILUETA_RETRASO = 14.0f;

    float Suave(float u) { u = (std::min)(1.0f, (std::max)(0.0f, u)); return u * u * (3.0f - 2.0f * u); }   // smoothstep 3u^2 - 2u^3
    float Envolver180(float a) { while (a > 180.0f) a -= 360.0f; while (a < -180.0f) a += 360.0f; return a; }
    float AlturaOjos(float x, float z) { return Colisiones::alturaTerreno(x, z) + Colisiones::ALTURA_OJOS; }

    // Catmull-Rom uniforme por los puntos p[0..n-1], recorrido por longitud de arco con fraccion u en [0, 1]
    glm::vec2 CatmullRom(const std::vector<glm::vec2>& p, float u)
    {
        const int n = (int)p.size();
        if (n == 1) return p[0];
        std::vector<float> acum(n, 0.0f);
        for (int i = 1; i < n; i++) acum[i] = acum[i - 1] + glm::length(p[i] - p[i - 1]);
        const float total = acum[n - 1];
        if (total < 1e-5f) return p[n - 1];
        const float s = (std::min)(1.0f, (std::max)(0.0f, u)) * total;
        int k = 0;
        while (k < n - 2 && s > acum[k + 1]) k++;
        const float largo = acum[k + 1] - acum[k];
        const float f = largo > 1e-6f ? (s - acum[k]) / largo : 0.0f;
        const glm::vec2 p0 = p[(std::max)(k - 1, 0)], p1 = p[k], p2 = p[k + 1], p3 = p[(std::min)(k + 2, n - 1)];
        return 0.5f * ((2.0f * p1) + (-p0 + p2) * f + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * f * f + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * f * f * f);
    }

    // yaw y pitch (grados) para mirar desde 'ojos' hacia 'objetivo'
    void Apuntar(const glm::vec3& ojos, const glm::vec3& objetivo, float& yaw, float& pitch)
    {
        const glm::vec3 d = objetivo - ojos;
        yaw = std::atan2(d.z, d.x) * 180.0f / PI;
        pitch = std::atan2(d.y, std::sqrt(d.x * d.x + d.z * d.z)) * 180.0f / PI;
        pitch = (std::max)(-89.0f, (std::min)(89.0f, pitch));
    }

    // Estacion de la comparsa: la camara se pone 9 m DELANTE del grupo (camina hacia +x) y 3.8 m tierra adentro, y lo ve venir
    glm::vec2 Llega(const Parada& e, const glm::vec2& comparsa)
    {
        if (!e.miraComparsa) return e.llega;
        return glm::vec2((std::min)(40.0f, (std::max)(-40.0f, comparsa.x + 9.0f)), e.llega.y);
    }

    glm::vec3 PuntoMira(const Parada& e, bool fin, const glm::vec2& comparsa)
    {
        if (e.miraComparsa) return glm::vec3(comparsa.x, Colisiones::alturaTerreno(comparsa.x, comparsa.y) + 1.0f, comparsa.y);
        const glm::vec3& m = fin ? e.miraFin : e.mira;
        return glm::vec3(m.x, Colisiones::alturaTerreno(m.x, m.z) + m.y, m.z);
    }
}

void Recorrido::Iniciar(const RecorridoPose& actual, Audio* audio, float duracionCaminataSilueta)
{
    if (activo_) return;
    duracionSil_ = duracionCaminataSilueta;
    activo_ = true;
    audio_ = audio;
    idx_ = 0;
    reloj_ = 0.0f;
    siluetaInicio_ = -1.0f;
    comparsaSuave_ = comparsa_;
    pose_ = actual;
    EmpezarTramo();
}

void Recorrido::Cancelar()
{
    if (!activo_) return;
    Terminar();
}

void Recorrido::Terminar()
{
    activo_ = false;
    enReten_ = false;
    if (audio_) audio_->DetenerVoz();
}

void Recorrido::EmpezarTramo()
{
    enReten_ = false;
    t_ = 0.0f;
    inicioTramo_ = pose_;
    const Parada& e = E[idx_];
    if (e.tramo > 0.0f) tramoReal_ = e.tramo;
    else
    {
        // Primer tramo: depende de donde estaba el visitante (4 m/s de promedio, entre 3 y 8 s)
        const float d = glm::length(glm::vec2(pose_.pos.x, pose_.pos.z) - e.llega);
        tramoReal_ = (std::min)(8.0f, (std::max)(3.0f, d / 4.0f));
    }
    if (idx_ == 0 && e.tramo <= 0.0f && glm::length(glm::vec2(pose_.pos.x, pose_.pos.z) - e.llega) < 0.5f) tramoReal_ = 0.0f;
    if (idx_ > 0 && e.tramo <= 0.0f) tramoReal_ = 0.0f;   // la ultima estacion se queda donde esta
    // La silueta llega al altar SILUETA_RETRASO s despues de que la camara se detenga ahi: sale al empezar este tramo (o despues, si no alcanza)
    if (idx_ == ESTACION_ALTAR)
        siluetaInicio_ = reloj_ + (std::max)(0.0f, tramoReal_ + SILUETA_RETRASO - duracionSil_);
    if (tramoReal_ <= 0.0f) EmpezarReten();
}

void Recorrido::EmpezarReten()
{
    enReten_ = true;
    t_ = 0.0f;
    const Parada& e = E[idx_];
    retenReal_ = e.reten;
    if (audio_)
    {
        const float dur = audio_->IniciarVoz(idx_);   // 0 si no hay archivo: el recorrido sigue con su tiempo de diseno
        if (dur > 0.0f) retenReal_ = (std::max)(4.0f, dur + 1.5f);
    }
}

bool Recorrido::Actualizar(float dt, RecorridoPose& pose)
{
    if (!activo_) return false;
    dt = (std::min)(dt, 0.1f);   // un tiron de cuadro no debe saltar la ruta
    reloj_ += dt;
    t_ += dt;
    comparsaSuave_ += (comparsa_ - comparsaSuave_) * (1.0f - std::exp(-dt / 1.5f));   // filtro de 1.5 s: si el grupo da la vuelta al final de la playa, la camara no salta
    const Parada& e = E[idx_];

    if (!enReten_)
    {
        const float u = Suave(t_ / tramoReal_);
        std::vector<glm::vec2> pts;
        pts.push_back(glm::vec2(inicioTramo_.pos.x, inicioTramo_.pos.z));
        for (int i = 0; i < e.nVia; i++) pts.push_back(e.via[i]);
        const glm::vec2 llegaE = Llega(e, comparsaSuave_);
        pts.push_back(llegaE);
        const glm::vec2 p = CatmullRom(pts, u);
        // La orientacion va del rumbo con que salio al rumbo de la estacion, por el arco mas corto, con el mismo smoothstep
        float yawB, pitchB;
        const glm::vec3 ojosB(llegaE.x, AlturaOjos(llegaE.x, llegaE.y), llegaE.y);
        Apuntar(ojosB, PuntoMira(e, false, comparsaSuave_), yawB, pitchB);
        pose_.pos = glm::vec3(p.x, AlturaOjos(p.x, p.y), p.y);
        pose_.yaw = inicioTramo_.yaw + Envolver180(yawB - inicioTramo_.yaw) * u;
        pose_.pitch = glm::mix(inicioTramo_.pitch, pitchB, u);
        if (t_ >= tramoReal_) EmpezarReten();
    }
    else
    {
        const float u = Suave(t_ / retenReal_);
        const glm::vec2 p = glm::mix(Llega(e, comparsaSuave_), e.fin, u);
        const glm::vec3 ojos(p.x, AlturaOjos(p.x, p.y), p.y);
        float yawA, pitchA;
        if (e.giro != 0.0f)
        {
            // Giro de la cabeza en los primeros 8 s (o la mitad del reten): del rumbo de llegada a +giro grados
            float y0, p0;
            Apuntar(ojos, PuntoMira(e, false, comparsaSuave_), y0, p0);
            const float ug = Suave(t_ / (std::min)(8.0f, 0.5f * retenReal_));
            yawA = y0 + e.giro * ug;
            pitchA = glm::mix(p0, e.pitchFin, ug);
        }
        else
        {
            const glm::vec3 m = e.miraComparsa ? PuntoMira(e, false, comparsaSuave_) : glm::mix(PuntoMira(e, false, comparsaSuave_), PuntoMira(e, true, comparsaSuave_), u);
            Apuntar(ojos, m, yawA, pitchA);
        }
        pose_.pos = ojos + glm::vec3(0.0f, 0.03f * std::sin(2.0f * PI * reloj_ / 7.0f), 0.0f);   // balanceo minimo para que no se vea congelada
        pose_.yaw = yawA;
        pose_.pitch = pitchA;
        if (t_ >= retenReal_)
        {
            if (audio_) audio_->DetenerVoz();
            idx_++;
            if (idx_ >= N_ESTACIONES) { pose = pose_; Terminar(); return true; }
            EmpezarTramo();
        }
    }
    pose = pose_;
    return true;
}

float Recorrido::TiempoSilueta() const
{
    if (!activo_) return -1.0f;
    if (siluetaInicio_ < 0.0f) return 0.0f;
    return (std::max)(0.0f, reloj_ - siluetaInicio_);
}

const char* Recorrido::Titulo() const { return E[(std::min)(idx_, N_ESTACIONES - 1)].titulo; }
const char* Recorrido::Texto() const { return E[(std::min)(idx_, N_ESTACIONES - 1)].texto; }
