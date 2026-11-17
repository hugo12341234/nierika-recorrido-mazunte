// Std. Includes
#include <string>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <sstream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Audio.h"
#include "Fichas.h"
#include "AnimFlores.h"  // C-4
#include "AnimVelas.h"    // C-3: velas flotando (Bezier + vaiven senoidal)
#include "DejarFlor.h"    // D-4: caida libre de la flor que deja la silueta
#include "Olas.h"   // C-1: parametros y altura del oleaje
#include "Colisiones.h"
#include "Terreno.h"        // X-33: altura del terreno (playa + colinas) y zona caminable
#include "Atuendos.h"       // X-35: ropa, piel, pelo y prendas tipicas distintas por persona
#include "Fuego.h"          // X-49: llama de las fogatas con el modelo fuego (D1) + parpadeo con senos
#include "Mariposas.h"      // X-49: mariposas de las colinas (modelos de D1), vuelo y aleteo con senos
#include "VidaColinas.h"    // X-33/X-34: senderos, faroles, fogatas, gente y luciernagas en las colinas
#include "Extras.h"  // X-19..X-22: figuras, petates y formulas de los extras
#include "VegetacionColinas.h"
#include "PuebloFondo.h"        // X-30 (fuera del alcance original): casas y promontorio al fondo (generado)  // X-18: posiciones de palmas y arbustos sobre las colinas (generado)
#include "OfrendaMayor.h"  // X-51: datos de la ofrenda mayor del patio
#include "PlazaTrasera.h"  // X-52: plaza detras de la capilla
#include "OfrendasFamiliares.h"  // X-55: 14 ofrendas familiares distintas en la playa de atras (sustituyen al panteon de 16 tumbas de X-54)
#include "Silueta.h"  // D-1/D-2: silueta de cubos con caminata por KeyFrames

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, Colisiones::ALTURA_OJOS, 6.0f));  // ojos a 1.6 m (A-9)
Colisiones colisiones;  // cajas AABB solidas (se llenan en main)
bool keys[1024];
GLfloat lastX = 640, lastY = 360;  // centro de la ventana de 1280x720
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Contador de FPS (E-1a): promedio cada 0.5 s, se muestra en el titulo de la ventana
const char* TITULO_VENTANA = "Nierika Studio - Recorrido virtual: Dia de Muertos en Mazunte";
const double INTERVALO_FPS = 0.5;  // segundos entre actualizaciones del titulo
double inicioIntervaloFPS = 0.0;
int framesIntervalo = 0;

// ==========================================
// ILUMINACION (A-8). Valores de D1 (319266894, Practica 8 adaptada a Mazunte).
// D1 los penso para la capilla a escala 3.5 en (0, 0, 2); hoy esa capilla ya no esta (se retiro en B-13): solo siguen usandose para las luces.
// Por eso cada posicion de D1 se pasa por DeCapillaVieja() y las atenuaciones se multiplican por la escala.
// ==========================================
const float ESCALA_CAPILLA_VIEJA = 3.5f;
glm::vec3 DeCapillaVieja(glm::vec3 p)
{
    return (p - glm::vec3(0.0f, 0.0f, 2.0f)) / ESCALA_CAPILLA_VIEJA + glm::vec3(0.0f, 0.0f, -0.6f);
}

bool active = false;       // espacio: enciende/apaga la animacion de las 4 luces puntuales
bool mostrarLuces = false; // L: cubos blancos que marcan donde estan las luces (ayuda de depuracion)

// Reflector de la cruz (T/G, Y/H, U/J mueven su posicion; 1-6 su direccion)
glm::vec3 spotLightPos = DeCapillaVieja(glm::vec3(0.0f, 3.0f, 2.0f));
glm::vec3 spotLightDir(0.0f, -1.0f, -0.2f);

// 6 reflectores fijos sobre los escalones del altar
glm::vec3 escalonSpotPos[6] = {
    DeCapillaVieja(glm::vec3(-1.5f, 1.3f, 3.0f)), DeCapillaVieja(glm::vec3(1.5f, 1.3f, 3.0f)),
    DeCapillaVieja(glm::vec3(-1.0f, 1.8f, 2.5f)), DeCapillaVieja(glm::vec3(1.0f, 1.8f, 2.5f)),
    DeCapillaVieja(glm::vec3(-0.5f, 2.3f, 2.0f)), DeCapillaVieja(glm::vec3(0.5f, 2.3f, 2.0f))
};
glm::vec3 escalonSpotDir(0.0f, -1.0f, 0.0f);

// 4 luces puntuales fijas (las animadas con la barra espaciadora)
glm::vec3 pointLightPositions[4] = {
    DeCapillaVieja(glm::vec3(-3.0f, 1.0f, 0.0f)), DeCapillaVieja(glm::vec3(3.0f, 1.0f, 0.0f)),
    DeCapillaVieja(glm::vec3(-3.0f, 1.0f, 4.0f)), DeCapillaVieja(glm::vec3(3.0f, 1.0f, 4.0f))
};
glm::vec3 Light1(0.0f), Light2(0.0f), Light3(0.0f), Light4(0.0f);

// Silueta de cubos (D-1/D-2): el tiempo de su caminata corre con deltaTime; R la reinicia
float tSilueta = 0.0f;

// Modo captura (solo desarrollo): NIERIKA_CAPTURAS="x,y,z,yaw,pitch,t,archivo.png;..." coloca la camara en cada pose,
// guarda la imagen con glReadPixels (SOIL_save_screenshot) y cierra el programa al terminar. Sin la variable no hace nada.
struct PoseCaptura { float x, y, z, yaw, pitch, t; std::string archivo; };

// Modo video (solo desarrollo, tarea X-16): NIERIKA_VIDEO="carpeta;fps;ruta_camara.txt". Fija el reloj (Reloj() = cuadro x dt, dt = 1/fps) y
// guarda cada cuadro como PNG (carpeta/frame_00000.png...) para unirlos con ffmpeg. El archivo de camara tiene una linea por punto:
// "t x y z yaw pitch" (interpolacion lineal) y opcionalmente "silueta t0" para empezar la caminata en t0 s. Sin la variable, Reloj() = glfwGetTime().
struct PuntoCamara { float t, x, y, z, yaw, pitch; };
bool modoVideo = false;
double relojVideo = 0.0;
double dtVideo = 1.0 / 30.0;
int cuadroVideo = 0;
float siluetaT0 = 0.0f;
std::string carpetaVideo;
std::vector<PuntoCamara> rutaVideo;
inline double Reloj() { return modoVideo ? relojVideo : glfwGetTime(); }

std::vector<PoseCaptura> posesCaptura;
size_t indiceCaptura = 0;
int framesCaptura = 0;

// Cubo unitario (posicion + normal) para marcar las luces
float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0,0,-1,  0.5f,-0.5f,-0.5f, 0,0,-1,  0.5f, 0.5f,-0.5f, 0,0,-1,
     0.5f, 0.5f,-0.5f, 0,0,-1, -0.5f, 0.5f,-0.5f, 0,0,-1, -0.5f,-0.5f,-0.5f, 0,0,-1,
    -0.5f,-0.5f, 0.5f, 0,0, 1,  0.5f,-0.5f, 0.5f, 0,0, 1,  0.5f, 0.5f, 0.5f, 0,0, 1,
     0.5f, 0.5f, 0.5f, 0,0, 1, -0.5f, 0.5f, 0.5f, 0,0, 1, -0.5f,-0.5f, 0.5f, 0,0, 1,
    -0.5f, 0.5f, 0.5f,-1,0,0, -0.5f, 0.5f,-0.5f,-1,0,0, -0.5f,-0.5f,-0.5f,-1,0,0,
    -0.5f,-0.5f,-0.5f,-1,0,0, -0.5f,-0.5f, 0.5f,-1,0,0, -0.5f, 0.5f, 0.5f,-1,0,0,
     0.5f, 0.5f, 0.5f, 1,0,0,  0.5f, 0.5f,-0.5f, 1,0,0,  0.5f,-0.5f,-0.5f, 1,0,0,
     0.5f,-0.5f,-0.5f, 1,0,0,  0.5f,-0.5f, 0.5f, 1,0,0,  0.5f, 0.5f, 0.5f, 1,0,0,
    -0.5f,-0.5f,-0.5f, 0,-1,0,  0.5f,-0.5f,-0.5f, 0,-1,0,  0.5f,-0.5f, 0.5f, 0,-1,0,
     0.5f,-0.5f, 0.5f, 0,-1,0, -0.5f,-0.5f, 0.5f, 0,-1,0, -0.5f,-0.5f,-0.5f, 0,-1,0,
    -0.5f, 0.5f,-0.5f, 0, 1,0,  0.5f, 0.5f,-0.5f, 0, 1,0,  0.5f, 0.5f, 0.5f, 0, 1,0,
     0.5f, 0.5f, 0.5f, 0, 1,0, -0.5f, 0.5f, 0.5f, 0, 1,0, -0.5f, 0.5f,-0.5f, 0, 1,0
};

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITULO_VENTANA, nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Camara en primera persona: oculta el cursor y lo mantiene dentro de la ventana (ESC cierra el programa)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach
    glewExperimental = GL_TRUE;
    // Initialize GLEW
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // C-7b: Dear ImGui para las fichas culturales por proximidad
    FichasInit(window);

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader skyShader("Shaders/sky.vs", "Shaders/sky.frag");  // X-5: cielo nocturno procedural
    GLuint vaoCielo;
    glGenVertexArrays(1, &vaoCielo);                          // VAO vacio: el vertex shader genera el triangulo con gl_VertexID

    // Load models
    Model fachada((char*)"Models/fachada/fachada.obj");
    Model arco((char*)"Models/arco/arco.obj");    // A-10: dovelas, en coordenadas de la fachada
    Model techo((char*)"Models/techo/techo.obj"); // A-11: techo a dos aguas separado de fachada.obj
    Model piso((char*)"Models/piso/piso.obj");    // A-11: piso interior a y = +0.02
    Model playa((char*)"Models/playa/playa.obj");
    Model mar((char*)"Models/mar/mar.obj");
    // X-3: extras de D1 (Maya), sin tarea propia; escala real ya aplicada en el .obj
    Model calavera((char*)"Models/calavera/calavera.obj");
    Model flor((char*)"Models/flor/flor.obj");
    // B-13 (provisional): modelos propios de los lotes 1-5 de Blender (escala real ya aplicada en cada .obj; texturas provisionales de un color)
    Model altar3((char*)"Models/altar3/altar3.obj");       // B-1
    Model vela((char*)"Models/vela/vela.obj");             // B-2
    Model botella((char*)"Models/botella/botella.obj");    // B-5
    Model marco((char*)"Models/marco/marco.obj");          // B-6
    Model soportes((char*)"Models/soportes/soportes.obj"); // B-8
    Model tumba1((char*)"Models/tumba1/tumba1.obj");       // B-9
    Model tumba2((char*)"Models/tumba2/tumba2.obj");
    Model tumba3((char*)"Models/tumba3/tumba3.obj");
    Model tumba4((char*)"Models/tumba4/tumba4.obj");
    Model cruz((char*)"Models/cruz/cruz.obj");             // B-11
    Model copalero((char*)"Models/copalero/copalero.obj"); // B-14
    Model barda((char*)"Models/barda/barda.obj");          // X-46: tumbas-capilla de Maya x0.75 contra el muro izquierdo (0.98 x 0.68 m de huella, 1.43 m de alto, giradas 90 grados)
    colisiones.agregar({ glm::vec3(-1.82f, 0.0f, -1.11f), glm::vec3(-1.13f, 1.43f, -0.13f) });
    colisiones.agregar({ glm::vec3(-1.82f, 0.0f,  0.13f), glm::vec3(-1.13f, 1.43f,  1.11f) });

    // B-10: barda perimetral del panteón
    Model personaPieA((char*)"Models/persona_pie_a/persona_pie_a.obj");               // X-19: figuras estaticas de la ceremonia
    Model personaPieB((char*)"Models/persona_pie_b/persona_pie_b.obj");
    Model personaAgachadaA((char*)"Models/persona_agachada_a/persona_agachada_a.obj");
    Model personaAgachadaB((char*)"Models/persona_agachada_b/persona_agachada_b.obj");
    Model petate((char*)"Models/petate/petate.obj");
    Model rocaMar((char*)"Models/roca_mar/roca_mar.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model lancha((char*)"Models/lancha/lancha.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model redPesca((char*)"Models/red_pesca/red_pesca.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model aveCuerpo((char*)"Models/ave_cuerpo/ave_cuerpo.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model aveAla((char*)"Models/ave_ala/ave_ala.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model tortugaMar((char*)"Models/tortuga/tortuga.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model hamaca((char*)"Models/hamaca/hamaca.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model fogata((char*)"Models/fogata/fogata.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model palapa((char*)"Models/palapa/palapa.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model casaA((char*)"Models/casa_a/casa_a.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model casaB((char*)"Models/casa_b/casa_b.obj");  // X-25..X-28: lados de la escena (palapa, casa_a y casa_b: fuera del alcance original)
    Model calles((char*)"Models/calles/calles.obj");                  // X-30: caminos de tierra del pueblo de fondo (fuera del alcance original)
    Model cimientos((char*)"Models/cimientos/cimientos.obj");         // X-53: bloques de piedra bajo las casas del pueblo de fondo (una malla en coordenadas del mundo; generada por tools/x53_ajustar_alturas.py)
    Model tambor((char*)"Models/tambor/tambor.obj");                  // X-29 (fuera del alcance original)
    // X-40: banda de viento oaxaquena. Musicos con los brazos en la pose de su instrumento + instrumentos (todos propios, hechos en Blender)
    Model musicoViento((char*)"Models/musico_viento/musico_viento.obj"), musicoTrombon((char*)"Models/musico_trombon/musico_trombon.obj"),
          musicoTuba((char*)"Models/musico_tuba/musico_tuba.obj"), musicoPercusion((char*)"Models/musico_percusion/musico_percusion.obj"),
          musicoPlatillos((char*)"Models/musico_platillos/musico_platillos.obj");
    Model bandaInst[Extras::N_INST_BANDA] = {
        Model((char*)"Models/banda_trompeta/banda_trompeta.obj"), Model((char*)"Models/banda_clarinete/banda_clarinete.obj"), Model((char*)"Models/banda_saxofon/banda_saxofon.obj"),
        Model((char*)"Models/banda_trombon/banda_trombon.obj"), Model((char*)"Models/banda_tuba/banda_tuba.obj"), Model((char*)"Models/banda_tambora/banda_tambora.obj"),
        Model((char*)"Models/banda_tarola/banda_tarola.obj"), Model((char*)"Models/banda_platillos/banda_platillos.obj") };
    Model alfombraFlores((char*)"Models/alfombra_flores/alfombra_flores.obj");
    Model alfombraCruz((char*)"Models/alfombra_cruz/alfombra_cruz.obj");
    Model farol((char*)"Models/farol/farol.obj");                         // X-24: poste con linterna abierta
    Model guitarra((char*)"Models/guitarra/guitarra.obj");                // X-24: instrumento del musico
    Model cesta((char*)"Models/cesta/cesta.obj");
    Model murcielagoCuerpo((char*)"Models/murcielago_cuerpo/murcielago_cuerpo.obj");
    Model murcielagoAla((char*)"Models/murcielago_ala/murcielago_ala.obj");
    Model luciernaga((char*)"Models/luciernaga/luciernaga.obj");         // X-22: icosaedro emisivo de 3 cm
    Model tablaRemo((char*)"Models/tabla_remo/tabla_remo.obj");           // X-21: tabla de remo con su remo
    Model colinas((char*)"Models/colinas/colinas.obj");     // X-18: relieve verde de fondo (malla 300 x 215 m, hueco sobre la playa)
    Model palma((char*)"Models/palma/palma.obj");           // X-9: extra decorativo (no cuenta entre los 18 propios)
    Model arbusto((char*)"Models/arbusto/arbusto.obj");   // X-9: extra decorativo
    Model roca((char*)"Models/roca/roca.obj");             // B-10: roca decorativa junto a la entrada
    Model cempasuchil((char*)"Models/cempasuchil/cempasuchil.obj");       // B-3: flor de planos cruzados (alfa)
    Model panMuerto((char*)"Models/pan_muerto/pan_muerto.obj");           // B-4: pan propio (el de Maya sigue en Models/pan/)
    Model papelPicado((char*)"Models/papel_picado/papel_picado.obj");     // B-7: tira con alfa, cuelga de los soportes
    // X-43: modelos de Maya (de D1, extras: no cuentan entre los 18 propios), reducidos y con pivote propio (tools/x43_piezas.py)
    Model papelFucsia((char*)"Models/papel_fucsia/papel_fucsia.obj");    // par de tiras 1.94 m de ancho, pivote: centro del borde superior
    Model papelMorado((char*)"Models/papel_morado/papel_morado.obj");    // par de tiras 2.15 m
    Model papelNaranja((char*)"Models/papel_naranja/papel_naranja.obj"); // una tira de 0.78 m
    Model mazeta((char*)"Models/mazeta/mazeta.obj");                     // maceta con cempasuchil, 0.39 m, pivote: centro de la base
    Model capillaMaya((char*)"Models/altar/altar.obj");                  // X-44: capilla con domo y cruz (Maya, de D1), vuelve como tumba-capilla del cuarto
    Model humo((char*)"Models/humo/humo.obj");                           // X-47: columna de humo de D1 (0.33 x 3.47 m, pivote: centro de la base)
    Model cuadro((char*)"Models/cuadro/cuadro.obj");                     // retrato con marco tallado, 0.35 m, pivote: centro de la base
    Model refresco((char*)"Models/refresco/refresco.obj");               // botella verde de refresco, 0.25 m
    Model fuegoLlama((char*)"Models/fuego/fuego.obj");                      // X-49: llama de fogata (D1), 0.30 m, pivote en la base (tools/x49_normalizar.py)
    Model mariCuerpo((char*)"Models/mariposa_cuerpo/mariposa_cuerpo.obj");  // X-49: mariposa (D1) en 3 piezas, x0.075 (envergadura 14.5 cm)
    Model mariAlaDer((char*)"Models/mariposa_ala_der/mariposa_ala_der.obj");
    Model mariAlaIzq((char*)"Models/mariposa_ala_izq/mariposa_ala_izq.obj");
    Model llamaModelo((char*)"Models/llama/llama.obj");                   // C-2: 2 planos cruzados; la forma la pinta lighting.frag
    Model arbolSelva((char*)"Models/arbol_selva/arbol_selva.obj");        // X-36: arbol de copa ancha (extra, no cuenta entre los 18 propios)
    Model arcoCemp((char*)"Models/arco_cempasuchil/arco_cempasuchil.obj"); // X-36: arco de carrizo y cempasuchil
    Model altarMayor((char*)"Models/altar_mayor/altar_mayor.obj");     // X-51: ofrenda mayor del patio (tela, faldones, retablo, columnas)
    Model altarPiramide((char*)"Models/altar_piramide/altar_piramide.obj"); // X-52: altar de 5 niveles con dosel, centro de la plaza trasera
    Model patioTierra((char*)"Models/patio_tierra/patio_tierra.obj");   // X-55: tierra apisonada bajo el patio de la capilla (las 4 tumbas dejan de estar sobre arena)
    Model mesaOfrenda((char*)"Models/mesa_ofrenda/mesa_ofrenda.obj");    // X-55: mesa con mantel (modelo propio nuevo, tinte por familia)
    Model veladora((char*)"Models/veladora/veladora.obj");               // X-55: vela en vaso de vidrio (propio nuevo)
    Model calabaza((char*)"Models/calabaza/calabaza.obj");               // X-55: calabaza (propio nuevo)
    Model naranjas((char*)"Models/naranjas/naranjas.obj");               // X-55: monton de 5 naranjas (propio nuevo)
    Model racimoCemp((char*)"Models/racimo_cempasuchil/racimo_cempasuchil.obj"); // X-36: 7 flores juntas (1 llamada de dibujo)
    Model senderoTierra((char*)"Models/sendero/sendero.obj");             // X-36: caminos de tierra sobre las colinas (en coordenadas del mundo)
    Atuendos atuendos;                                                    // X-35: telas y prendas (falda, rebozo, sombrero) de las personas
    const std::vector<VidaColinas::Fuente> fuentesLuz = VidaColinas::Fuentes();   // X-34: fogatas y faroles que pueden iluminar al visitante

    // C-4: 5 flores flotando en la orilla (mar en y = -0.2, orilla en z = +24; valores en ESTADO, decision del 04/11)
    // {centro XZ, radio, sentido, tMov, tPausa, desfase, omegaGiro, faseGiro}
    const FlorFlotante floresFlotantes[5] = {
        { glm::vec2(-4.0f, 28.0f), 0.60f,  1.0f, 5.0f, 2.0f, 0.0f, 0.8f, 0.0f },
        { glm::vec2(-1.5f, 30.0f), 0.45f, -1.0f, 4.5f, 2.5f, 1.7f, 0.6f, 1.3f },
        { glm::vec2( 1.0f, 27.0f), 0.75f,  1.0f, 5.5f, 1.5f, 3.1f, 1.0f, 2.6f },
        { glm::vec2( 3.5f, 29.5f), 0.50f, -1.0f, 5.0f, 3.0f, 4.4f, 0.7f, 3.9f },
        { glm::vec2( 6.0f, 27.5f), 0.65f,  1.0f, 4.0f, 2.0f, 5.8f, 0.9f, 5.2f }
    };
    // C-3: 4 velas flotando mar adentro (orilla z = +24; valores en ESTADO, decision del 11/11)
    // {P0 x,z; P1 x,z; P2 x,z; periodo T; desfase t0; amplitud a; omega_v; fase phi_j = 2 pi j / 4}
    const float OMEGA_V = 2.0f * 3.14159265f / 2.4f;
    const VelaFlotante velasFlotantes[4] = {
        { -2.5f, 25.0f,  5.5f, 40.0f, 0.5f, 60.0f, 100.0f,  0.0f, 0.012f, OMEGA_V, 0.0f },
        {  0.5f, 25.0f, -7.5f, 40.0f, -2.5f, 60.0f, 110.0f, 17.0f, 0.012f, OMEGA_V, 1.5708f },
        {  2.5f, 25.0f, 10.5f, 40.0f, 5.5f, 60.0f, 120.0f, 41.0f, 0.012f, OMEGA_V, 3.1416f },
        {  5.0f, 25.0f, -3.0f, 40.0f, 2.0f, 60.0f, 130.0f, 63.0f, 0.012f, OMEGA_V, 4.7124f }
    };
    // C-6 (12/11): las flores flotantes usan la flor PROPIA de cempasuchil (B-3, planos con alfa: el descarte por alfa de lighting.frag
    // las recorta), no la de Maya (flor.obj, RGB sin alfa). cempasuchil.obj mide 0.07 m y tiene su pivote en el centro:
    // se agranda x2.3 (0.16 m, el tamano que tenia la flor de Maya) y gira sobre su centro (0, 0, 0)
    const glm::vec3 centroFlor(0.0f, 0.0f, 0.0f);
    const float ESCALA_FLOR_FLOTANTE = 2.3f;
    const float ESCALA_ALTAR = 1.0f;   // X-46: altar3 a su tamano original (1.6 x 0.9 x 1.2 m); X-45 lo habia llevado a x1.3
    const float ESCALA_PAPEL = 1.0f, ESCALA_MACETA = 1.0f;   // X-46: papeles y macetas a su tamano original (X-43)
    const float ESCALA_CAPILLA = 0.75f;   // X-46: tumbas-capilla de Maya x0.75 (0.98 x 0.68 x 1.43 m) para que quepan en el cuarto de A-2

    // ==========================================
    // COLISIONES AABB (A-9). Cajas en coordenadas del mundo, medidas de los .obj (fachada girada 180 grados en Y:
    // la apertura queda en z = +1.31). Muros ~0.1 m de espesor; piso = playa.obj (suelo y = 0, no es caja).
    // ==========================================
    colisiones.agregar({ glm::vec3(-1.965f, 0.0f, -1.310f), glm::vec3( 1.965f, 2.62f, -1.217f) });  // muro del fondo (X-46: dimensiones originales de A-2)
    colisiones.agregar({ glm::vec3(-1.965f, 0.0f, -1.310f), glm::vec3(-1.826f, 2.62f,  1.310f) });  // muro izquierdo
    colisiones.agregar({ glm::vec3( 1.826f, 0.0f, -1.310f), glm::vec3( 1.965f, 2.62f,  1.310f) });  // muro derecho
    colisiones.agregar({ glm::vec3(-1.965f, 0.0f,  1.217f), glm::vec3(-0.621f, 2.62f,  1.310f) });  // muro del frente, a la izquierda de la puerta
    colisiones.agregar({ glm::vec3( 0.562f, 0.0f,  1.217f), glm::vec3( 1.965f, 2.62f,  1.310f) });  // muro del frente, a la derecha de la puerta
    // B-13: altar3 en (1.35, 0, 0) girado -90 grados: su huella (1.6 x 0.9 m) ocupa x 0.90..1.80, z -0.80..+0.80, y 0..1.20
    colisiones.agregar({ glm::vec3( 0.900f, 0.0f, -0.800f), glm::vec3( 1.800f, 1.20f,  0.800f) });

    // B-10: barda perimetral del panteón (recinto x -7..+7, z +3..+12, h 0.70, espesor 0.25)
    // Entradas de 2 m en el norte (z = 12) y, desde E-2 (12/11), tambien en el sur (z = 3), las dos centradas en x = 0 (abertura x -1..+1):
    // la del sur queda frente a la puerta del cuarto y por ella camina la silueta
    colisiones.agregar({ glm::vec3(-7.0f, 0.0f,  3.0f ), glm::vec3(-1.0f, 0.70f,  3.25f) });  // muro sur izquierdo
    colisiones.agregar({ glm::vec3( 1.0f, 0.0f,  3.0f ), glm::vec3( 7.0f, 0.70f,  3.25f) });  // muro sur derecho
    colisiones.agregar({ glm::vec3(-7.0f, 0.0f, 11.75f), glm::vec3(-1.0f, 0.70f, 12.0f ) });  // muro norte izquierdo
    colisiones.agregar({ glm::vec3( 1.0f, 0.0f, 11.75f), glm::vec3( 7.0f, 0.70f, 12.0f ) });  // muro norte derecho
    colisiones.agregar({ glm::vec3(-7.0f, 0.0f,  3.25f), glm::vec3(-6.75f,0.70f, 12.0f ) });  // muro oeste
    colisiones.agregar({ glm::vec3( 6.75f,0.0f,  3.25f), glm::vec3( 7.0f, 0.70f, 12.0f ) });  // muro este
    colisiones.agregar({ glm::vec3( 1.275f, 0.0f, 12.325f), glm::vec3( 1.725f, 0.30f, 12.675f) });  // B-10: roca decorativa
    // X-51: ofrendas mayores del patio (huella del peldano incluida, sin giro): x +-(2.0..5.8), z 7.38..9.70
    for (int lado = -1; lado <= 1; lado += 2)
        colisiones.agregar({ glm::vec3(OfrendaMayor::X_CENTRO * lado - OfrendaMayor::SEMI_X, 0.0f, OfrendaMayor::Z_CENTRO + OfrendaMayor::Z_ATRAS),
                             glm::vec3(OfrendaMayor::X_CENTRO * lado + OfrendaMayor::SEMI_X, OfrendaMayor::ALTO, OfrendaMayor::Z_CENTRO + OfrendaMayor::Z_FRENTE) });
    // X-52/X-53: plaza trasera: solo el altar de piramide (ofrenda comunitaria al final del camino de animas)
    colisiones.agregar({ glm::vec3(-PlazaTrasera::SEMI_PIRAMIDE, 0.0f, PlazaTrasera::CZ - PlazaTrasera::SEMI_PIRAMIDE),
                         glm::vec3( PlazaTrasera::SEMI_PIRAMIDE, PlazaTrasera::ALTO_PIRAMIDE, PlazaTrasera::CZ + PlazaTrasera::SEMI_PIRAMIDE) });
    // X-55: estructuras solidas de las 14 ofrendas familiares (mesa, altares, lancha, cruz, soportes, hamaca, tabla; las piezas chicas no estorban)
    for (const OfrendasFamiliares::Familia& f : OfrendasFamiliares::Colocadas())
        for (const OfrendasFamiliares::Caja& c : OfrendasFamiliares::CajasColision(f))
            colisiones.agregar({ glm::vec3(c.x0, 0.0f, c.z0), glm::vec3(c.x1, c.alto, c.z1) });
    for (int t = 0; t < Extras::N_TENDEDEROS; t++)  // X-20: postes de los tendederos (cajas de 0.1 m)
        for (int k = 0; k <= Extras::TENDEDEROS[t].n; k++)
        {
            const float px = Extras::TENDEDEROS[t].x0 + Extras::ANCHO_BASTIDOR * k, pz = Extras::TENDEDEROS[t].z;
            colisiones.agregar({ glm::vec3(px - 0.05f, 0.0f, pz - 0.05f), glm::vec3(px + 0.05f, 2.0f, pz + 0.05f) });
        }
    // X-25..X-28: bandera de lo que esta FUERA DEL ALCANCE ORIGINAL (casas y palapas). Constante Extras::MOSTRAR_FUERA_DE_ALCANCE o NIERIKA_SIN_FUERA_ALCANCE=1
    const bool fueraAlcance = Extras::MOSTRAR_FUERA_DE_ALCANCE && !std::getenv("NIERIKA_SIN_FUERA_ALCANCE");
    if (fueraAlcance)
    {
        for (int i = 0; i < Extras::N_BANDA; i++)    // X-40: musicos de la banda (cajas de 0.6 m)
            colisiones.agregar({ glm::vec3(Extras::BANDA[i].x - 0.3f, 0.0f, Extras::BANDA[i].z - 0.3f), glm::vec3(Extras::BANDA[i].x + 0.3f, 1.7f, Extras::BANDA[i].z + 0.3f) });
        for (int i = 0; i < Extras::N_CASAS; i++)    // cada casa: huella de 5 x 4 m (x y z se intercambian con el giro de 90 grados) + porche
        {
            const Extras::Casa& c = Extras::CASAS[i];
            colisiones.agregar({ glm::vec3(c.x - 2.0f, 0.0f, c.z - 2.6f), glm::vec3(c.x + 2.0f, 3.8f, c.z + 2.6f) });
        }
        for (int i = 0; i < Extras::N_PALAPAS; i++)  // cuatro postes de 0.24 m
            for (int sx = -1; sx <= 1; sx += 2)
                for (int sz = -1; sz <= 1; sz += 2)
                    colisiones.agregar({ glm::vec3(Extras::PALAPAS[i].x + 2.2f * sx - 0.12f, 0.0f, Extras::PALAPAS[i].z + 2.2f * sz - 0.12f), glm::vec3(Extras::PALAPAS[i].x + 2.2f * sx + 0.12f, 2.8f, Extras::PALAPAS[i].z + 2.2f * sz + 0.12f) });
    }
    for (int i = 0; i < Extras::N_LANCHAS_VARADAS; i++)  // lanchas varadas (5.2 x 1.5 m a lo largo de la orilla)
        colisiones.agregar({ glm::vec3(Extras::LANCHAS_VARADAS[i].x - 2.7f, 0.0f, Extras::LANCHAS_VARADAS[i].z - 0.85f), glm::vec3(Extras::LANCHAS_VARADAS[i].x + 2.7f, 1.0f, Extras::LANCHAS_VARADAS[i].z + 0.85f) });
    for (int i = 0; i < Extras::N_FOGATAS; i++)
        colisiones.agregar({ glm::vec3(Extras::FOGATAS[i].x - 0.6f, 0.0f, Extras::FOGATAS[i].z - 0.6f), glm::vec3(Extras::FOGATAS[i].x + 0.6f, 0.4f, Extras::FOGATAS[i].z + 0.6f) });
    for (int i = 0; i < Extras::N_HAMACAS; i++)          // dos postes de 0.1 m
        for (int sx = -1; sx <= 1; sx += 2)
            colisiones.agregar({ glm::vec3(Extras::HAMACAS[i].x + 1.6f * sx - 0.06f, 0.0f, Extras::HAMACAS[i].z - 0.06f), glm::vec3(Extras::HAMACAS[i].x + 1.6f * sx + 0.06f, 1.5f, Extras::HAMACAS[i].z + 0.06f) });
    for (int i = 0; i < Extras::N_ROCAS; i++)            // solo las rocas grandes y las que estan en la arena (z < 23)
        if (Extras::ROCAS[i].esc >= 2.5f && Extras::ROCAS[i].z < 23.0f)
            colisiones.agregar({ glm::vec3(Extras::ROCAS[i].x - Extras::ROCAS[i].esc * 0.9f, 0.0f, Extras::ROCAS[i].z - Extras::ROCAS[i].esc * 0.8f), glm::vec3(Extras::ROCAS[i].x + Extras::ROCAS[i].esc * 0.9f, 3.0f, Extras::ROCAS[i].z + Extras::ROCAS[i].esc * 0.8f) });
    for (int i = 0; i < Extras::N_FIGURAS_N; i++)  // X-24: figuras de la narrativa (cajas de 0.4 x 0.4 m)
        colisiones.agregar({ glm::vec3(Extras::FIGURAS_N[i].x - 0.2f, 0.0f, Extras::FIGURAS_N[i].z - 0.2f), glm::vec3(Extras::FIGURAS_N[i].x + 0.2f, 1.7f, Extras::FIGURAS_N[i].z + 0.2f) });
    for (int i = 0; i < Extras::N_FAROLES; i++)    // X-24: postes de los faroles (0.1 m)
        colisiones.agregar({ glm::vec3(Extras::FAROLES[i].x - 0.05f, 0.0f, Extras::FAROLES[i].z - 0.05f), glm::vec3(Extras::FAROLES[i].x + 0.05f, 1.7f, Extras::FAROLES[i].z + 0.05f) });
    for (int i = 0; i < Extras::N_FIGURAS; i++)  // X-19: cada figura es una caja de 0.5 x 0.5 m (como el cuerpo del visitante)
        colisiones.agregar({ glm::vec3(Extras::FIGURAS[i].x - 0.25f, 0.0f, Extras::FIGURAS[i].z - 0.25f), glm::vec3(Extras::FIGURAS[i].x + 0.25f, 1.7f, Extras::FIGURAS[i].z + 0.25f) });

    // X-33: colisiones de lo que hay sobre las colinas y la franja norte. Estas cajas llevan la altura ABSOLUTA del terreno (el suelo ya no es y = 0):
    // van de 0.6 m bajo el punto de apoyo hasta 'alto' m encima. Los arbustos y las flores no bloquean el paso.
    {
        namespace VD = VidaColinasDatos;
        auto caja = [&](float x, float y, float z, float mx, float mz, float alto)
        {
            colisiones.agregar({ glm::vec3(x - mx, y - 0.6f, z - mz), glm::vec3(x + mx, y + alto, z + mz) });
        };
        for (int i = 0; i < VegetacionColinas::N_PALMAS; i++)   // tronco de las palmas de X-18 (0.3 m de semiancho x escala)
            caja(VegetacionColinas::PALMAS[i].x, VegetacionColinas::PALMAS[i].y, VegetacionColinas::PALMAS[i].z, 0.3f * VegetacionColinas::PALMAS[i].escala, 0.3f * VegetacionColinas::PALMAS[i].escala, 3.0f);
        for (int i = 0; i < VD::N_PALMAS; i++)
            caja(VD::PALMAS[i].x, VD::PALMAS[i].y, VD::PALMAS[i].z, 0.3f * VD::PALMAS[i].esc, 0.3f * VD::PALMAS[i].esc, 3.0f);
        for (int i = 0; i < VD::N_ARBOLES; i++)   // X-36: tronco del arbol (0.4 m de semiancho x escala)
            caja(VD::ARBOLES[i].x, VD::ARBOLES[i].y, VD::ARBOLES[i].z, 0.4f * VD::ARBOLES[i].esc, 0.4f * VD::ARBOLES[i].esc, 4.0f);
        for (int i = 0; i < VD::N_ROCAS; i++)     // X-36: solo las rocas de 0.9 m o mas bloquean el paso
            if (VD::ROCAS[i].esc >= 0.9f) caja(VD::ROCAS[i].x, VD::ROCAS[i].y, VD::ROCAS[i].z, 0.8f * VD::ROCAS[i].esc, 0.7f * VD::ROCAS[i].esc, 1.2f * VD::ROCAS[i].esc);
        for (int i = 0; i < VD::N_OFRENDAS; i++) caja(VD::OFRENDAS[i].x, VD::OFRENDAS[i].y, VD::OFRENDAS[i].z, 0.4f, 0.4f, 1.5f);   // cruz
        for (int i = 0; i < VD::N_FAROLES; i++) caja(VD::FAROLES[i].x, VD::FAROLES[i].y, VD::FAROLES[i].z, 0.1f, 0.1f, 1.7f);
        for (int i = 0; i < VD::N_FOGATAS; i++) caja(VD::FOGATAS[i].x, VD::FOGATAS[i].y, VD::FOGATAS[i].z, 0.6f, 0.6f, 0.4f);
        for (int i = 0; i < VD::N_PERSONAS; i++) caja(VD::PERSONAS[i].x, VD::PERSONAS[i].y, VD::PERSONAS[i].z, 0.3f * VD::PERSONAS[i].esc + 0.05f, 0.3f * VD::PERSONAS[i].esc + 0.05f, 1.7f * VD::PERSONAS[i].esc);
        if (fueraAlcance)
        {
            for (int i = 0; i < PuebloFondo::N_CASAS; i++)       // huella de 5 x 4 m: se usa un cuadrado de 5.6 m para cualquier giro
                caja(PuebloFondo::CASAS[i].x, PuebloFondo::CASAS[i].y, PuebloFondo::CASAS[i].z, 2.8f, 2.8f, 3.8f);
            for (int i = 0; i < PuebloFondo::N_ROCAS; i++)
                caja(PuebloFondo::ROCAS[i].x, PuebloFondo::ROCAS[i].y, PuebloFondo::ROCAS[i].z, PuebloFondo::ROCAS[i].esc * 0.9f, PuebloFondo::ROCAS[i].esc * 0.8f, 3.0f);
            for (int i = 0; i < VD::N_PALAPAS; i++)               // 4 postes de la palapa (a +-2.2 m, girados como la palapa)
                for (int sx = -1; sx <= 1; sx += 2)
                    for (int sz = -1; sz <= 1; sz += 2)
                    {
                        const float g = glm::radians(VD::PALAPAS[i].giro);
                        const float ox = 2.2f * sx, oz = 2.2f * sz;
                        const float px = VD::PALAPAS[i].x + ox * std::cos(g) + oz * std::sin(g), pz = VD::PALAPAS[i].z - ox * std::sin(g) + oz * std::cos(g);
                        caja(px, Colisiones::alturaTerreno(px, pz), pz, 0.12f, 0.12f, 2.8f);
                    }
        }
    }

    // VAO del cubo que marca las luces (tecla L)
    GLuint cuboVBO, cuboVAO;
    glGenVertexArrays(1, &cuboVAO);
    glGenBuffers(1, &cuboVBO);
    glBindVertexArray(cuboVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cuboVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Silueta silueta;  // jerarquia + KeyFrames (src/Silueta.cpp)
    // D-1: una pieza por .obj con el pivote en la articulacion; el orden es el de los nodos de Silueta
    // (torso, cabeza, brazo/antebrazo izq, brazo/antebrazo der, muslo/pierna izq, muslo/pierna der)
    const char* piezasSilueta[10] = { "torso", "cabeza", "brazo_izq", "antebrazo_izq", "brazo_der", "antebrazo_der",
                                      "muslo_izq", "pierna_izq", "muslo_der", "pierna_der" };
    std::vector<Model> modelosSilueta;
    modelosSilueta.reserve(10);
    for (int i = 0; i < 10; i++)
        modelosSilueta.push_back(Model((char*)("Models/silueta/" + std::string(piezasSilueta[i]) + ".obj").c_str()));

    // D-4: la silueta lleva una flor de cempasuchil en la mano derecha y la suelta en T_SUELTA s despues de llegar al altar.
    // La mano al soltar sale de la pose del brazo (se calcula una vez); el destino es la repisa del nivel inferior de altar3
    // (altar3 en (1.35, 0, 0) girado -90 grados: repisa en x 0.90..1.15, tope a y = 0.40; la flor mide 0.07 m, su centro queda 0.035 mas arriba).
    // PROVISIONAL: se ajusta en B-13 junto con la posicion final de la silueta y del altar.
    const glm::vec3 manoSuelta = silueta.ManoAlSoltar();
    const glm::vec3 destinoFlor(1.05f, 0.435f, manoSuelta.z);   // repisa inferior del altar3 (x 0.90..1.15, tope a y 0.40; la flor mide 0.07 m); X-46: valor original de D-4

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 300.0f);  // far plane a 300 m (A-4)

    // Game loop
    // D-6: audio ambiental en bucle (si faltan los archivos de Audio/, avisa y sigue sin sonido)
    Audio audio;
    audio.init();

    // D-5: la carga de modelos y texturas tarda segundos antes de este punto. Sin esto, el primer deltaTime los incluye
    // (lastFrame arranca en 0) y la silueta empezaria su caminata ya avanzada o terminada.
    lastFrame = (GLfloat)Reloj();
    tSilueta = 0.0f;

    // Solo para las capturas del manual: NIERIKA_LUCES=1 enciende las luces de colores (como la barra espaciadora), =2 ademas muestra los cubos (L)
    if (const char* luces = std::getenv("NIERIKA_LUCES"))
    {
        active = true;
        Light1 = glm::vec3(0.0f, 1.0f, 0.0f); Light2 = glm::vec3(0.0f, 0.2f, 1.0f);
        Light3 = glm::vec3(1.0f, 0.0f, 0.0f); Light4 = glm::vec3(1.0f, 0.1f, 0.8f);
        if (luces[0] == '2') mostrarLuces = true;
    }
    if (const char* cap = std::getenv("NIERIKA_CAPTURAS"))
    {
        std::stringstream ss(cap);
        std::string item;
        while (std::getline(ss, item, ';'))
        {
            for (char& c : item) if (c == ',') c = ' ';
            std::stringstream is(item);
            PoseCaptura pc;
            if (is >> pc.x >> pc.y >> pc.z >> pc.yaw >> pc.pitch >> pc.t >> pc.archivo) posesCaptura.push_back(pc);
        }
    }

    if (const char* vid = std::getenv("NIERIKA_VIDEO"))
    {
        std::stringstream sv(vid);
        std::string fpsTxt, rutaTxt;
        std::getline(sv, carpetaVideo, ';'); std::getline(sv, fpsTxt, ';'); std::getline(sv, rutaTxt, ';');
        double fps = std::atof(fpsTxt.c_str());
        FILE* f = std::fopen(rutaTxt.c_str(), "r");
        if (f && fps > 0.0)
        {
            char linea[256];
            while (std::fgets(linea, sizeof(linea), f))
            {
                PuntoCamara pc;
                if (std::sscanf(linea, "%f %f %f %f %f %f", &pc.t, &pc.x, &pc.y, &pc.z, &pc.yaw, &pc.pitch) == 6) rutaVideo.push_back(pc);
                else std::sscanf(linea, "silueta %f", &siluetaT0);
            }
            std::fclose(f);
            if (!rutaVideo.empty()) { modoVideo = true; dtVideo = 1.0 / fps; tSilueta = siluetaT0; }
        }
        else std::cout << "[Video] No se pudo leer la ruta de camara: " << rutaTxt << std::endl;
    }
    while (!glfwWindowShouldClose(window))
    {
        if (indiceCaptura < posesCaptura.size() && framesCaptura == 0)
        {
            const PoseCaptura& pc = posesCaptura[indiceCaptura];
            camera.SetPosition(glm::vec3(pc.x, pc.y, pc.z));
            camera.SetOrientacion(pc.yaw, pc.pitch);
            tSilueta = pc.t;
        }

        if (modoVideo)
        {
            relojVideo = cuadroVideo * dtVideo;
            const double tt = relojVideo;
            size_t k = 0;
            while (k + 1 < rutaVideo.size() && rutaVideo[k + 1].t <= tt) k++;
            PuntoCamara a = rutaVideo[k], b2 = rutaVideo[std::min(k + 1, rutaVideo.size() - 1)];
            float u = (b2.t > a.t) ? glm::clamp((float)((tt - a.t) / (b2.t - a.t)), 0.0f, 1.0f) : 0.0f;
            camera.SetPosition(glm::vec3(glm::mix(a.x, b2.x, u), glm::mix(a.y, b2.y, u), glm::mix(a.z, b2.z, u)));
            camera.SetOrientacion(glm::mix(a.yaw, b2.yaw, u), glm::mix(a.pitch, b2.pitch, u));
        }
        // Set frame time
        GLfloat currentFrame = Reloj();
        deltaTime = currentFrame - lastFrame;
        if (modoVideo) deltaTime = (GLfloat)dtVideo;
        lastFrame = currentFrame;

        // FPS promedio de los ultimos 0.5 s: cuadros contados / tiempo transcurrido
        framesIntervalo++;
        if (currentFrame - inicioIntervaloFPS >= INTERVALO_FPS)
        {
            double fps = framesIntervalo / (currentFrame - inicioIntervaloFPS);
            char titulo[160];
            snprintf(titulo, sizeof(titulo), "%s | %.0f FPS (%.1f ms)", TITULO_VENTANA, fps, 1000.0 / fps);
            glfwSetWindowTitle(window, titulo);
            inicioIntervaloFPS = currentFrame;
            framesIntervalo = 0;
        }

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // X-5: cielo (se dibuja primero, sin escribir profundidad; la escena lo tapa)
        {
            glm::mat4 vistaCielo = camera.GetViewMatrix();
            glm::mat4 invPV = glm::inverse(projection * vistaCielo);
            skyShader.Use();
            glUniformMatrix4fv(glGetUniformLocation(skyShader.Program, "invProjView"), 1, GL_FALSE, glm::value_ptr(invPV));
            glUniform3f(glGetUniformLocation(skyShader.Program, "camPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
            glm::vec3 haciaLuna = glm::normalize(glm::vec3(0.65f, 0.50f, 0.35f));
            glUniform3f(glGetUniformLocation(skyShader.Program, "dirLuna"), haciaLuna.x, haciaLuna.y, haciaLuna.z);
            glDepthMask(GL_FALSE);
            glBindVertexArray(vaoCielo);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            glDepthMask(GL_TRUE);
        }

        shader.Use();
        glUniform3f(glGetUniformLocation(shader.Program, "tinteObjeto"), 1.0f, 1.0f, 1.0f);   // X-55: sin tinte salvo en las ofrendas familiares

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // 1. Luz direccional (luna / ambiente)
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.65f, -0.50f, -0.35f);  // X-5: luna
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.20f, 0.25f, 0.39f);    // X-36: luna ~25 % mas fuerte (antes 0.16/0.20/0.32 y 0.42/0.50/0.68)
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.58f, 0.69f, 0.93f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

        // 2. Animacion ciclica de las puntuales: las traseras brillan con sen(2t) > 0 y las delanteras con sen(2t) < 0
        glm::vec3 colors[4] = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
        if (active)
        {
            float ciclo = sin(Reloj() * 2.0f);
            float traseras = glm::max(0.0f, ciclo);
            float delanteras = glm::max(0.0f, -ciclo);
            colors[0] = Light1 * traseras;
            colors[1] = Light2 * traseras;
            colors[2] = Light3 * delanteras;
            colors[3] = Light4 * delanteras;
        }
        // Atenuacion de D1 (1, 0.14, 0.07) para la escena vieja: las distancias se redujeron 3.5 veces,
        // asi que linear x3.5 y cuadratica x3.5^2 dejan el mismo alcance relativo
        const float kLin = ESCALA_CAPILLA_VIEJA, kCuad = ESCALA_CAPILLA_VIEJA * ESCALA_CAPILLA_VIEJA;
        for (int i = 0; i < 4; i++)
        {
            std::string n = "pointLights[" + std::to_string(i) + "].";
            glUniform3f(glGetUniformLocation(shader.Program, (n + "position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "ambient").c_str()), colors[i].x, colors[i].y, colors[i].z);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "diffuse").c_str()), colors[i].x, colors[i].y, colors[i].z);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "specular").c_str()), 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "linear").c_str()), 0.14f * kLin);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "quadratic").c_str()), 0.07f * kCuad);
        }

        // X-8: luz calida de las velas del altar. Parpadeo = suma de dos senos de periodos distintos (sin ruido ni particulas)
        {
            float t = (float)Reloj();
            float parpadeo = 0.85f + 0.10f * sin(t * 7.3f) + 0.05f * sin(t * 13.1f + 1.3f);   // 0.70 .. 1.00
            glm::vec3 col = glm::vec3(1.0f, 0.55f, 0.22f) * parpadeo;
            glUniform3f(glGetUniformLocation(shader.Program, "velasAltar.position"), 1.15f, 1.25f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, "velasAltar.ambient"), col.x * 0.10f, col.y * 0.10f, col.z * 0.10f);
            glUniform3f(glGetUniformLocation(shader.Program, "velasAltar.diffuse"), col.x * 1.1f, col.y * 1.1f, col.z * 1.1f);
            glUniform3f(glGetUniformLocation(shader.Program, "velasAltar.specular"), col.x * 0.4f, col.y * 0.4f, col.z * 0.4f);
            glUniform1f(glGetUniformLocation(shader.Program, "velasAltar.constant"), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, "velasAltar.linear"), 0.35f);
            glUniform1f(glGetUniformLocation(shader.Program, "velasAltar.quadratic"), 0.44f);
        }

        // X-51/X-52: luz calida de las ofrendas mayores. Hay 3 fuentes (2 en el patio, 1 en la plaza trasera) pero solo las 2 MAS CERCANAS a la camara
        // se envian al shader (costo fijo de 2 luces). Parpadeo = 2 senos de periodos distintos, con fase propia por fuente.
        {
            const float t = (float)Reloj();
            const glm::vec3 fuentes[3] = {
                glm::vec3(-OfrendaMayor::X_CENTRO, 2.0f, OfrendaMayor::Z_CENTRO + 1.0f), glm::vec3(OfrendaMayor::X_CENTRO, 2.0f, OfrendaMayor::Z_CENTRO + 1.0f),
                glm::vec3(0.0f, 2.2f, PlazaTrasera::CZ) };
            int sel[2] = { 0, 1 };
            float dmin[2] = { 1e9f, 1e9f };
            sel[0] = sel[1] = -1;
            for (int i = 0; i < 3; i++)
            {
                const glm::vec3 d = fuentes[i] - camera.GetPosition();
                const float d2 = glm::dot(d, d);
                if (d2 < dmin[0]) { dmin[1] = dmin[0]; sel[1] = sel[0]; dmin[0] = d2; sel[0] = i; }
                else if (d2 < dmin[1]) { dmin[1] = d2; sel[1] = i; }
            }
            for (int k = 0; k < 2; k++)
            {
                const int i = sel[k];
                const float par = 0.85f + 0.10f * sin(t * 6.1f + 2.1f * i) + 0.05f * sin(t * 11.3f + 0.9f + 1.7f * i);
                const glm::vec3 col = glm::vec3(1.0f, 0.55f, 0.22f) * par;
                const glm::vec3 pos = fuentes[i];
                const std::string n = "altarMayor[" + std::to_string(k) + "].";
                glUniform3f(glGetUniformLocation(shader.Program, (n + "position").c_str()), pos.x, pos.y, pos.z);
                glUniform3f(glGetUniformLocation(shader.Program, (n + "ambient").c_str()), col.x * 0.05f, col.y * 0.05f, col.z * 0.05f);
                glUniform3f(glGetUniformLocation(shader.Program, (n + "diffuse").c_str()), col.x * 1.3f, col.y * 1.3f, col.z * 1.3f);
                glUniform3f(glGetUniformLocation(shader.Program, (n + "specular").c_str()), col.x * 0.3f, col.y * 0.3f, col.z * 0.3f);
                glUniform1f(glGetUniformLocation(shader.Program, (n + "constant").c_str()), 1.0f);
                glUniform1f(glGetUniformLocation(shader.Program, (n + "linear").c_str()), 0.22f);
                glUniform1f(glGetUniformLocation(shader.Program, (n + "quadratic").c_str()), 0.20f);
            }
        }

        // X-34: luz calida de la fogata o el farol mas cercano (2 luces puntuales). Se envia SIEMPRE con constantes validas aunque el color sea 0
        {
            VidaColinas::Luz lc[2];
            VidaColinas::ElegirLuces(fuentesLuz, camera.GetPosition(), (float)Reloj(), lc);
            for (int i = 0; i < 2; i++)
            {
                std::string n = "luzCalida[" + std::to_string(i) + "].";
                glUniform3f(glGetUniformLocation(shader.Program, (n + "position").c_str()), lc[i].pos.x, lc[i].pos.y, lc[i].pos.z);
                glUniform3f(glGetUniformLocation(shader.Program, (n + "ambient").c_str()), lc[i].color.x * 0.05f, lc[i].color.y * 0.05f, lc[i].color.z * 0.05f);
                glUniform3f(glGetUniformLocation(shader.Program, (n + "diffuse").c_str()), lc[i].color.x, lc[i].color.y, lc[i].color.z);
                glUniform3f(glGetUniformLocation(shader.Program, (n + "specular").c_str()), lc[i].color.x * 0.3f, lc[i].color.y * 0.3f, lc[i].color.z * 0.3f);
                glUniform1f(glGetUniformLocation(shader.Program, (n + "constant").c_str()), 1.0f);
                glUniform1f(glGetUniformLocation(shader.Program, (n + "linear").c_str()), lc[i].lineal);
                glUniform1f(glGetUniformLocation(shader.Program, (n + "quadratic").c_str()), lc[i].cuadratica);
            }
        }

        // 3. Reflector de la cruz
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.position"), spotLightPos.x, spotLightPos.y, spotLightPos.z);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.direction"), spotLightDir.x, spotLightDir.y, spotLightDir.z);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), 0.09f * kLin);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), 0.032f * kCuad);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));

        // 4. Seis reflectores de los escalones (calidos)
        for (int i = 0; i < 6; i++)
        {
            std::string n = "escalonSpots[" + std::to_string(i) + "].";
            glUniform3f(glGetUniformLocation(shader.Program, (n + "position").c_str()), escalonSpotPos[i].x, escalonSpotPos[i].y, escalonSpotPos[i].z);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "direction").c_str()), escalonSpotDir.x, escalonSpotDir.y, escalonSpotDir.z);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "diffuse").c_str()), 1.0f, 0.6f, 0.2f);
            glUniform3f(glGetUniformLocation(shader.Program, (n + "specular").c_str()), 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "linear").c_str()), 0.09f * kLin);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "quadratic").c_str()), 0.032f * kCuad);
            glUniform1f(glGetUniformLocation(shader.Program, (n + "cutOff").c_str()), glm::cos(glm::radians(15.0f)));
            glUniform1f(glGetUniformLocation(shader.Program, (n + "outerCutOff").c_str()), glm::cos(glm::radians(20.0f)));
        }

        // Brillo especular por objeto (los .mtl no se leen): arena casi mate, mar mas brillante
        GLint specLoc = glGetUniformLocation(shader.Program, "specularStrength");
        glUniform1f(specLoc, 0.05f);

        // 0. PLAYA (100 x 50 m, vertices ya en coordenadas del mundo: x -50..50, z -20..30; A-5)
        glm::mat4 modelPlaya(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlaya));
        glUniform1i(glGetUniformLocation(shader.Program, "arena"), 1);  // X-13: relieve de manchas y arena mojada
        playa.Draw(shader);
        glUniform1i(glGetUniformLocation(shader.Program, "arena"), 0);

        // 0b. MAR (100 x 100 m en y = -0.2, vertices ya en coordenadas del mundo: x -50..50, z 22..122; orilla en z = 24; A-7)
        glm::mat4 modelMar(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMar));
        glUniform1f(specLoc, 0.6f);
        // C-1: oleaje por vertex shader (suma de senos, parametros en src/Olas.h)
        {
            float oA[Olas::N], oK[Olas::N], oW[Olas::N], oDir[2 * Olas::N], oFase[Olas::N];
            Olas::ParametrosShader(oA, oK, oW, oDir, oFase);
            glUniform1i(glGetUniformLocation(shader.Program, "oleaje"), 1);
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), (float)Reloj());
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaA"), Olas::N, oA);
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaK"), Olas::N, oK);
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaW"), Olas::N, oW);
            glUniform2fv(glGetUniformLocation(shader.Program, "ondaDir"), Olas::N, oDir);
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaFase"), Olas::N, oFase);
        }
        glUniform1i(glGetUniformLocation(shader.Program, "espuma"), 1);  // X-22: espuma en la orilla
        mar.Draw(shader);
        glUniform1i(glGetUniformLocation(shader.Program, "espuma"), 0);
        glUniform1i(glGetUniformLocation(shader.Program, "oleaje"), 0);   // el resto de los modelos sin oleaje

        // 1. FACHADA (escala real en el .obj desde A-2: planta exterior de 3.93 x 2.62 m, arco de 2.11 m)
        glm::mat4 modelFachada(1.0f);
        modelFachada = glm::translate(modelFachada, glm::vec3(0.0f, 0.0f, 0.0f));
        modelFachada = glm::rotate(modelFachada, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        float escalaFachada = 1.0f;
        modelFachada = glm::scale(modelFachada, glm::vec3(escalaFachada, escalaFachada, escalaFachada));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFachada));
        glUniform1f(specLoc, 0.1f);
        glUniform1i(glGetUniformLocation(shader.Program, "relieveLuma"), 1);  // X-44: relieve de roca rugosa (como el video de referencia) en muros, arco y piso
        fachada.Draw(shader);
        arco.Draw(shader);   // A-10: misma matriz que la fachada
        glUniform1i(glGetUniformLocation(shader.Program, "relieveLuma"), 0);
        techo.Draw(shader);  // A-11: misma matriz que la fachada (mismo sistema de coord. Blender)

        // 1b. PISO INTERIOR (A-11): plano a y = +0.02 con transformacion identidad
        glm::mat4 modelPiso(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiso));
        glUniform1f(specLoc, 0.05f);  // piso: poco especular (igual que la arena)
        glUniform1i(glGetUniformLocation(shader.Program, "relieveLuma"), 1);
        piso.Draw(shader);
        glUniform1i(glGetUniformLocation(shader.Program, "relieveLuma"), 0);

        // 3b. MODELOS PROPIOS DE BLENDER (B-1, B-2, B-5, B-6, B-8, B-9, B-11, B-14). Posiciones PROVISIONALES: B-13 las ajusta.
        // Altar de 3 niveles contra el muro derecho del cuarto (interior x +-1.826), girado -90 grados para que su frente (+z local) mire a -x.
        // Cubre x 0.9..1.8 y tiene colision (B-13). La capilla provisional y el pan de Maya se retiraron en B-13 (12/11).
        glUniform1f(specLoc, 0.1f);
        auto colocar = [&](Model& m, glm::vec3 pos, float giroY)
        {
            glm::mat4 mm = glm::translate(glm::mat4(1.0f), pos);
            mm = glm::rotate(mm, glm::radians(giroY), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
            m.Draw(shader);
        };
        {
            // Posicion local sobre el altar (x, y, z locales) -> mundo: giro -90 grados en Y alrededor de (1.35, 0, 0)
            const glm::vec3 altarPos(1.35f, 0.0f, 0.0f);
            auto sobreAltar = [&](Model& m, glm::vec3 loc, float esc = 1.0f)
            {
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), altarPos);
                mm = glm::rotate(mm, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::scale(mm, glm::vec3(ESCALA_ALTAR));  // X-45: el altar y su ofrenda crecen juntos (el cuarto es x1.4 desde X-44)
                mm = glm::translate(mm, loc);
                mm = glm::scale(mm, glm::vec3(esc));  // X-10: escala opcional (las flores extra del altar van x1.7)
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                m.Draw(shader);
            };
            {
                glm::mat4 ma = glm::translate(glm::mat4(1.0f), altarPos);
                ma = glm::rotate(ma, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                ma = glm::scale(ma, glm::vec3(ESCALA_ALTAR));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ma));
                altar3.Draw(shader);
            }
            sobreAltar(cuadro,   glm::vec3( 0.00f, 1.20f, -0.25f));  // X-43: retrato tallado (Maya, de D1) en el nivel superior, como en el video de referencia
            sobreAltar(marco,    glm::vec3(-0.12f, 0.40f,  0.36f));  // X-43: el marco propio (B-6) baja al nivel inferior (el nivel medio queda tapado por el superior en z < -0.05)
            sobreAltar(vela,     glm::vec3(-0.35f, 1.20f, -0.25f));
            sobreAltar(vela,     glm::vec3( 0.35f, 1.20f, -0.25f));
            sobreAltar(refresco, glm::vec3(-0.40f, 0.80f,  0.05f));  // X-43: botella de refresco (Maya, de D1) en el nivel medio (y = 0.80); la botella propia (B-5) sigue en los miradores
            sobreAltar(copalero, glm::vec3( 0.40f, 0.80f,  0.05f));
            // C-2: llamas de las 2 velas del altar (la vela mide 0.15 m; la llama nace en su punta). Sin luz, forma por ruido de Perlin
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), (float)Reloj());
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
            glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.0f);
            sobreAltar(llamaModelo, glm::vec3(-0.35f, 1.35f, -0.25f));
            glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 1.3f);
            sobreAltar(llamaModelo, glm::vec3( 0.35f, 1.35f, -0.25f));
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            // B-13 (11/11, provisional, sin ver en pantalla): pan propio y cempasuchil en el nivel medio (y = 0.80; la flor
            // tiene su pivote en el centro y mide 0.07 m, por eso va a 0.80 + 0.035)
            sobreAltar(panMuerto,   glm::vec3( 0.00f, 0.80f, -0.05f));
            sobreAltar(cempasuchil, glm::vec3(-0.20f, 0.835f, 0.00f));
            sobreAltar(cempasuchil, glm::vec3( 0.20f, 0.835f, 0.00f));
            // X-9: mas cempasuchil (ofrenda mas llena): flanquea el marco, rodea los panes y llena el nivel inferior
            const glm::vec3 floresExtra[10] = { { -0.17f, 1.235f, -0.32f }, { 0.17f, 1.235f, -0.32f }, { -0.60f, 0.835f, -0.12f }, { 0.60f, 0.835f, -0.12f },
                                                { -0.62f, 0.835f, 0.20f }, { -0.30f, 0.435f, 0.30f }, { -0.55f, 0.435f, 0.28f }, { -0.75f, 0.435f, 0.12f },
                                                { 0.05f, 0.435f, 0.33f }, { 0.75f, 0.435f, 0.10f } };
            for (int i = 0; i < 10; i++) sobreAltar(cempasuchil, floresExtra[i] + glm::vec3(0.0f, 0.025f, 0.0f), 1.7f);  // x1.7: mas visibles (12 cm); +2.5 cm porque el pivote esta al centro
            // B-13 (12/11): extras de D1 (X-3, sin tarea propia) que estaban sobre los escalones de la capilla retirada:
            // calavera en la repisa del nivel inferior (y = 0.40), en el extremo +x local (mundo z = +0.45), lejos de donde
            // cae la flor de la silueta (D-4: mundo z ~ -0.39), y una flor de Maya (pivote en el centro, 0.12 m de alto) a cada lado
            sobreAltar(calavera, glm::vec3( 0.45f, 0.40f, 0.32f));
            sobreAltar(flor,     glm::vec3( 0.27f, 0.461f, 0.34f));
            sobreAltar(flor,     glm::vec3( 0.63f, 0.461f, 0.34f));
        }
        // X-43: papel picado en los muros del cuarto (alto del borde superior y = 2.40; 1.5 cm dentro del muro) y 4 macetas con cempasuchil
        // (2 flanquean el altar, 2 en el muro izquierdo). Giro Y: +90 mira a +x, -90 mira a -x, 0 mira a +z.
        glUniform1f(specLoc, 0.1f);
        auto colocarEsc = [&](Model& m, glm::vec3 pos, float giroY, float esc)
        {
            glm::mat4 mm = glm::translate(glm::mat4(1.0f), pos);
            mm = glm::rotate(mm, glm::radians(giroY), glm::vec3(0.0f, 1.0f, 0.0f));
            mm = glm::scale(mm, glm::vec3(esc));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
            m.Draw(shader);
        };
        // X-46: papeles y macetas a su tamano original (X-43), en el cuarto de A-2 (interior x +-1.826, z +-1.217, muros de 2.62 m)
        colocarEsc(papelFucsia, glm::vec3(-1.811f, 2.40f,  0.00f),  90.0f, ESCALA_PAPEL);  // muro izquierdo
        colocarEsc(papelMorado, glm::vec3( 1.811f, 2.40f,  0.00f), -90.0f, ESCALA_PAPEL);  // muro derecho, sobre el altar (que llega a y = 1.45)
        for (int i = 0; i < 3; i++)
            colocarEsc(papelNaranja, glm::vec3(-1.15f + 1.15f * i, 2.40f, -1.202f), 0.0f, ESCALA_PAPEL);  // muro del fondo
        colocarEsc(mazeta, glm::vec3( 1.55f, 0.02f,  1.02f), 0.0f,   ESCALA_MACETA);   // flanquean el altar
        colocarEsc(mazeta, glm::vec3( 1.55f, 0.02f, -1.02f), 90.0f,  ESCALA_MACETA);
        colocarEsc(mazeta, glm::vec3(-1.60f, 0.02f, -1.05f), 40.0f,  ESCALA_MACETA);   // esquinas del fondo (la izquierda queda junto a la capilla)
        // X-44/X-46: dos tumbas-capilla (la capilla con domo y cruz de Maya, extra de D1) x0.75 contra el muro izquierdo, de frente al cuarto.
        // Posicion del mundo de un punto local (lx, ly, lz) de la capilla en zc: (xc + lz*S, 0.02 + ly*S, zc - lx*S)  (giro 90 grados en Y)
        {
            const float S = ESCALA_CAPILLA, xc = -1.47f;
            for (int c = -1; c <= 1; c += 2)
            {
                const float zc = 0.62f * c;
                colocarEsc(capillaMaya, glm::vec3(xc, 0.02f, zc), 90.0f, S);
                // ofrenda en el nicho (suelo del nicho a y = 0.52 local): retrato (marco propio B-6), 2 velas y cempasuchil; la de z < 0 lleva la calavera de D1
                const float yn = 0.02f + 0.52f * S + 0.02f;
                colocar(marco, glm::vec3(xc + 0.0f, yn, zc), 90.0f);
                colocar(vela,  glm::vec3(xc + 0.10f, yn, zc + 0.19f), 0.0f);
                colocar(vela,  glm::vec3(xc + 0.10f, yn, zc - 0.19f), 0.0f);
                for (int f = -1; f <= 1; f += 2)
                    colocarEsc(cempasuchil, glm::vec3(xc + 0.11f, yn + 0.045f, zc + 0.10f * f), 0.0f, 1.7f);
            }
            colocar(calavera, glm::vec3(xc + 0.10f, 0.02f + 0.52f * S + 0.02f, -0.62f - 0.0f), 90.0f);
        }
        // X-44/X-46: un copalero a cada lado de la entrada (el modelo mide 0.18 m: x2.5 para que se vea como los braseros del video)
        glUniform1f(specLoc, 0.1f);
        for (int s = -1; s <= 1; s += 2)
            colocarEsc(copalero, glm::vec3(1.05f * s, 0.0f, 1.75f), 0.0f, 2.5f);
        // Panteon provisional afuera, entre la fachada y la orilla (z = +24); la ruta central x ~ 0 queda libre
        glUniform1i(glGetUniformLocation(shader.Program, "relieveLuma"), 1);  // X-23: relieve de piedra en cruz, tumbas y barda
        colocar(cruz,     glm::vec3(-2.5f, 0.0f,  3.7f), 0.0f);  // E-2: antes z = 3.0, dentro del muro sur (z 3.00..3.25)
        colocar(tumba2,   glm::vec3(-4.0f, 0.0f,  4.5f), 0.0f);
        colocar(tumba3,   glm::vec3( 4.0f, 0.0f,  5.0f), 0.0f);
        colocar(tumba4,   glm::vec3(-5.8f, 0.0f,  6.2f), 0.0f);  // X-51: antes (-4.5, 9.0); cede el sitio a la ofrenda mayor
        colocar(tumba1,   glm::vec3( 5.8f, 0.0f,  6.2f), 0.0f);  // X-51: antes (4.5, 10.0)
        colocar(soportes, glm::vec3( 0.0f, 0.0f,  6.0f), 0.0f);  // postes del papel picado
        // B-13: papel picado colgado del travesano (y = 1.95; su pivote es el borde superior). Provisional, sin ver en pantalla.
        colocar(papelPicado, glm::vec3(0.0f, 1.95f, 6.0f), 0.0f);
        glUniform1f(specLoc, 0.02f);
        colocar(patioTierra, glm::vec3(0.0f), 0.0f);   // X-55: suelo de tierra apisonada dentro de la barda (plano a y = 0.015, en coordenadas del mundo)
        glUniform1f(specLoc, 0.1f);
        colocar(barda,    glm::vec3( 0.0f, 0.0f,  0.0f), 0.0f);  // B-10: barda perimetral (ya en coordenadas del mundo)
        glUniform1i(glGetUniformLocation(shader.Program, "relieveLuma"), 0);
        // X-51/X-52: ofrendas mayores (patio y plaza trasera). 'dibujarOfrenda' dibuja una lista de objetos locales con la matriz T(P) R(giro) T(pos) S(esc);
        // las velas llevan llama (sin luz; forma por ruido en lighting.frag) en su punta (0.15 m x escala).
        {
            glUniform1f(specLoc, 0.08f);
            const std::vector<OfrendaMayor::Objeto> objs = OfrendaMayor::Lista();
            const std::vector<OfrendaMayor::Objeto> objsPir = PlazaTrasera::ListaPiramide();
            auto modeloDe = [&](OfrendaMayor::Tipo t) -> Model*
            {
                switch (t)
                {
                case OfrendaMayor::FLOR: return &cempasuchil;   case OfrendaMayor::RACIMO: return &racimoCemp;
                case OfrendaMayor::PAN: return &panMuerto;      case OfrendaMayor::CALAVERA: return &calavera;
                case OfrendaMayor::COPALERO: return &copalero;  case OfrendaMayor::REFRESCO: return &refresco;
                case OfrendaMayor::MARCO: return &marco;        default: return &vela;
                }
            };
            auto matriz = [&](glm::vec3 P, float giro, glm::vec3 pos, float esc, float giroItem)
            {
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), P);
                mm = glm::rotate(mm, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::translate(mm, pos);
                mm = glm::rotate(mm, glm::radians(giroItem), glm::vec3(0.0f, 1.0f, 0.0f));
                return glm::scale(mm, glm::vec3(esc));
            };
            auto dibujarLista = [&](const std::vector<OfrendaMayor::Objeto>& lista, glm::vec3 P, float giro, bool listaYaGirada)
            {
                for (const OfrendaMayor::Objeto& o : lista)
                {
                    glm::mat4 mm = matriz(P, giro, o.pos, o.esc, listaYaGirada ? o.giro : 0.0f);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                    modeloDe(o.tipo)->Draw(shader);
                }
            };
            int nfGlobal = 0;
            auto llamasLista = [&](const std::vector<OfrendaMayor::Objeto>& lista, glm::vec3 P, float giro)
            {
                for (const OfrendaMayor::Objeto& o : lista)
                {
                    if (o.tipo != OfrendaMayor::VELA) continue;
                    glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.83f * (nfGlobal++));
                    glm::mat4 ml = matriz(P, giro, o.pos + glm::vec3(0.0f, 0.15f * o.esc, 0.0f), o.esc, 0.0f);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                    llamaModelo.Draw(shader);
                }
            };
            struct MayorColocado { glm::vec3 P; float giro; };
            const MayorColocado mayores[2] = {
                { glm::vec3(-OfrendaMayor::X_CENTRO, 0.0f, OfrendaMayor::Z_CENTRO), 0.0f }, { glm::vec3(OfrendaMayor::X_CENTRO, 0.0f, OfrendaMayor::Z_CENTRO), 0.0f } };
            for (const MayorColocado& m : mayores)
            {
                glm::mat4 ma = matriz(m.P, m.giro, glm::vec3(0.0f), 1.0f, 0.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ma));
                altarMayor.Draw(shader);
                dibujarLista(objs, m.P, m.giro, false);
                for (int k = 0; k < 2; k++)   // papel picado: 2 tiras de 1.7 m bajo el travesano (y = 2.68)
                {
                    glm::mat4 mp = matriz(m.P, m.giro, glm::vec3(-0.85f + 1.70f * k, 2.66f, -0.78f), 1.0f, 0.0f);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp));
                    papelPicado.Draw(shader);
                }
            }
            colocar(arcoCemp, glm::vec3(0.0f, 0.0f, OfrendaMayor::ARCO_Z), 0.0f);
            // X-52: altar de piramide al centro de la plaza trasera, con papel picado colgado de los 4 travesanos del dosel (y = 2.50)
            {
                const glm::vec3 P(0.0f, 0.0f, PlazaTrasera::CZ);
                glm::mat4 mp0 = matriz(P, 0.0f, glm::vec3(0.0f), 1.0f, 0.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp0));
                altarPiramide.Draw(shader);
                dibujarLista(objsPir, P, 0.0f, true);
                for (int k = 0; k < 4; k++)
                {
                    glm::mat4 mp = matriz(P, 90.0f * k, glm::vec3(0.0f, 2.48f, 1.44f), 1.0f, 0.0f);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp));
                    papelPicado.Draw(shader);
                }
            }
            // X-55: ofrendas familiares de la playa de atras (14, todas distintas; ver src/OfrendasFamiliares.h). Cada pieza lleva su tinte (uniform tinteObjeto).
            // Se dibujan solo las familias a menos de 55 m (las llamas, a menos de 32 m) para no cargar la GPU integrada con ~550 dibujos.
            static const std::vector<OfrendasFamiliares::Familia> familias = OfrendasFamiliares::Colocadas();   // fijo: sin calculo en tiempo de ejecucion
            namespace OF = OfrendasFamiliares;
            const glm::vec3 camF = camera.GetPosition();
            auto cercaF = [&](const OF::Familia& f, float d) { const float dx = f.x - camF.x, dz = f.z - camF.z; return dx * dx + dz * dz < d * d; };
            {
                GLint tinteLoc = glGetUniformLocation(shader.Program, "tinteObjeto");
                auto modeloFam = [&](OF::Pieza t) -> Model*
                {
                    switch (t)
                    {
                    case OF::VELA: return &vela;              case OF::VELADORA: return &veladora;      case OF::RACIMO: return &racimoCemp;
                    case OF::FLOR: return &cempasuchil;       case OF::PAN: return &panMuerto;          case OF::COPALERO: return &copalero;
                    case OF::BOTELLA: return &botella;        case OF::REFRESCO: return &refresco;      case OF::MARCO: return &marco;
                    case OF::CUADRO: return &cuadro;          case OF::CESTA: return &cesta;            case OF::PETATE: return &petate;
                    case OF::CRUZ: return &cruz;              case OF::MAZETA: return &mazeta;          case OF::ALF_FLORES: return &alfombraFlores;
                    case OF::ALF_CRUZ: return &alfombraCruz;  case OF::MESA: return &mesaOfrenda;       case OF::ALTAR3: return &altar3;
                    case OF::ALTAR_MAYOR: return &altarMayor; case OF::ARCO: return &arcoCemp;          case OF::SOPORTES: return &soportes;
                    case OF::PAPEL: return &papelPicado;      case OF::CALABAZA: return &calabaza;      case OF::NARANJAS: return &naranjas;
                    case OF::GUITARRA: return &guitarra;      case OF::HAMACA: return &hamaca;          case OF::LANCHA: return &lancha;
                    case OF::RED: return &redPesca;           case OF::TABLA: return &tablaRemo;        case OF::TAMBOR: return &tambor;
                    case OF::FAROL: return &farol;            case OF::ROCA: return &roca;              case OF::CALAVERA: return &calavera;
                    default: return &vela;
                    }
                };
                glm::vec3 tinteActual(1.0f);
                for (const OF::Familia& f : familias)
                {
                    if (!cercaF(f, 55.0f)) continue;
                    const glm::vec3 P(f.x, 0.0f, f.z);
                    for (const OF::Item& it : f.items)
                    {
                        if (it.tinte != tinteActual) { glUniform3f(tinteLoc, it.tinte.x, it.tinte.y, it.tinte.z); tinteActual = it.tinte; }
                        glm::mat4 mm = matriz(P, f.giro, it.pos, it.esc, it.giro);
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                        modeloFam(it.tipo)->Draw(shader);
                    }
                }
                glUniform3f(tinteLoc, 1.0f, 1.0f, 1.0f);
                // personas de rodillas que velan cada ofrenda (miran a la ofrenda); misma pose y atuendos que las de la orilla (X-19)
                const float tv = (float)Reloj();
                Model* tiposV[2] = { &personaAgachadaA, &personaAgachadaB };
                int nv = 0;
                for (const OF::Familia& f : familias)
                {
                    const glm::vec3 P(f.x, 0.0f, f.z);
                    for (const OF::Velador& pv : f.velan)
                    {
                        const int k = nv++;
                        if (!cercaF(f, 55.0f)) continue;
                        glm::mat4 mm = matriz(P, f.giro, glm::vec3(pv.x, 0.0f, pv.z), 1.0f, pv.mira);
                        mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tv, 5.0f, 0.7f * (float)k, 2.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                        atuendos.dibujar(shader, *tiposV[pv.modelo % 2], true, 600 + k, mm);
                    }
                }
            }
            for (int i = 0; i < PlazaTrasera::N_ARCOS; i++)
                colocar(arcoCemp, glm::vec3(PlazaTrasera::ARCOS[i].x, 0.0f, PlazaTrasera::ARCOS[i].y), 0.0f);
            // llamas de todas las velas de las ofrendas
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), (float)Reloj());
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
            for (const MayorColocado& m : mayores) llamasLista(objs, m.P, m.giro);
            for (const OF::Familia& f : familias)   // X-55: llama de cada vela y veladora de las familias cercanas
            {
                if (!cercaF(f, 32.0f)) continue;
                const glm::vec3 P(f.x, 0.0f, f.z);
                for (const OF::Item& it : f.items)
                {
                    if (!OF::LlevaLlama(it.tipo)) continue;
                    glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.83f * (nfGlobal++));
                    glm::mat4 ml = matriz(P, f.giro, it.pos + glm::vec3(0.0f, OF::AltoVela(it.tipo) * it.esc, 0.0f), it.esc, 0.0f);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                    llamaModelo.Draw(shader);
                }
            }

            llamasLista(objsPir, glm::vec3(0.0f, 0.0f, PlazaTrasera::CZ), 0.0f);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
        }
        // X-9: vegetacion decorativa en la playa (z 14..22, fuera de la barda y lejos del eje x = 0 de la caminata)
        {
            const glm::vec3 palmas[8] = { { -25.0f, 0.0f, 13.0f }, { -13.0f, 0.0f, -4.0f }, { -17.0f, 0.0f, 5.0f }, { 16.0f, 0.0f, -3.0f },
                                          { -19.0f, 0.0f, 17.0f }, { 21.0f, 0.0f, 6.0f }, { -9.5f, 0.0f, -6.4f }, { 7.6f, 0.0f, -7.4f } };   // X-42: palmas repartidas hacia el centro   // X-41: la palma de (-5.5, 21.5) estaba dentro del grupo de personas E1
            for (int i = 0; i < 8; i++) colocar(palma, palmas[i], 47.0f * i);
            const glm::vec3 arbustos[8] = { { -2.8f, 0.0f, 13.7f }, { 3.4f, 0.0f, 13.8f }, { -18.0f, 0.0f, 9.0f }, { -15.0f, 0.0f, 0.0f },
                                            { -27.0f, 0.0f, 3.0f }, { 19.0f, 0.0f, 12.0f }, { 12.0f, 0.0f, 0.0f }, { -6.0f, 0.0f, -4.0f } };
            glUniform1f(specLoc, 0.05f);
            for (int i = 0; i < 8; i++) colocar(arbusto, arbustos[i], 31.0f * i);
        }
        // X-19: ceremonia en la orilla (propuesta, tabla 2.1 y fig. 1 y 6): 12 figuras ESTATICAS (poses fijas, sin IA), 3 petates con ofrenda y
        // veladoras sobre la arena. Unica animacion: balanceo senoidal Extras::Balanceo (sin KeyFrames).
        {
            const float tc = (float)Reloj();
            glUniform1f(specLoc, 0.05f);
            Model* tiposFigura[4] = { &personaPieA, &personaPieB, &personaAgachadaA, &personaAgachadaB };
            for (int i = 0; i < Extras::N_FIGURAS; i++)
            {
                const Extras::Figura& f = Extras::FIGURAS[i];
                const bool agachada = f.tipo >= 2;
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(f.x, Colisiones::alturaSuelo(f.z), f.z));
                mm = glm::rotate(mm, glm::radians(f.giroY), glm::vec3(0.0f, 1.0f, 0.0f));
                // de pie: ladeo suave (+-1.2 grados, 4.5 s); de rodillas: cabeceo al respirar (+-2 grados, 5 s) alrededor de las rodillas
                if (agachada) mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tc, 5.0f, f.fase, 2.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                else          mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tc, 4.5f, f.fase, 1.2f)), glm::vec3(0.0f, 0.0f, 1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                atuendos.dibujar(shader, *tiposFigura[f.tipo], agachada, i, mm);   // X-35: atuendo propio (semilla = indice)
            }
            for (int i = 0; i < Extras::N_PETATES; i++)
            {
                const Extras::Petate& pt = Extras::PETATES[i];
                const glm::vec3 base(pt.x, Colisiones::alturaSuelo(pt.z), pt.z);
                colocar(petate, base, pt.giroY);
                // ofrenda sobre el petate: velas, cempasuchil, pan y copalero (coordenadas locales del petate)
                const glm::vec3 vel[2] = { { -0.35f, 0.02f, -0.15f }, { 0.35f, 0.02f, -0.15f } };
                for (int k = 0; k < 2; k++) colocar(vela, base + vel[k], 0.0f);
                colocar(panMuerto, base + glm::vec3(0.0f, 0.02f, 0.10f), 0.0f);
                colocar(copalero, base + glm::vec3(-0.10f, 0.02f, -0.20f), 0.0f);
                for (int k = 0; k < 6; k++)
                {
                    glm::mat4 mf = glm::translate(glm::mat4(1.0f), base + glm::vec3(-0.40f + 0.16f * k, 0.055f, 0.22f + 0.04f * (k % 2)));
                    mf = glm::scale(mf, glm::vec3(1.7f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mf));
                    cempasuchil.Draw(shader);
                }
            }
            // veladoras sobre la arena (fig. 6): 26 velas pequenas en dos hileras ondulantes entre la ceremonia y la orilla
            for (int i = 0; i < 26; i++)
            {
                const float vx = 3.0f + 0.62f * i;
                const float vz = 21.3f + 0.35f * std::sin(0.9f * i) + 0.5f * (i % 2);
                glm::mat4 mv = glm::translate(glm::mat4(1.0f), glm::vec3(vx, Colisiones::alturaSuelo(vz), vz));
                mv = glm::scale(mv, glm::vec3(0.7f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mv));
                vela.Draw(shader);
            }
            // llamas (sin luz, ruido de Perlin del lighting.frag): las de los petates y las veladoras
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tc);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
            int nf = 0;
            auto llamaEn = [&](glm::vec3 p, float esc)
            {
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.83f * (nf++));
                glm::mat4 ml = glm::translate(glm::mat4(1.0f), p);
                ml = glm::scale(ml, glm::vec3(esc));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                llamaModelo.Draw(shader);
            };
            for (int i = 0; i < Extras::N_PETATES; i++)
            {
                const Extras::Petate& pt = Extras::PETATES[i];
                const float y0 = Colisiones::alturaSuelo(pt.z) + 0.02f + 0.15f;
                llamaEn(glm::vec3(pt.x - 0.35f, y0, pt.z - 0.15f), 1.0f);
                llamaEn(glm::vec3(pt.x + 0.35f, y0, pt.z - 0.15f), 1.0f);
            }
            for (int i = 0; i < 26; i++)
            {
                const float vx = 3.0f + 0.62f * i;
                const float vz = 21.3f + 0.35f * std::sin(0.9f * i) + 0.5f * (i % 2);
                llamaEn(glm::vec3(vx, Colisiones::alturaSuelo(vz) + 0.7f * 0.15f, vz), 0.7f);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
        }
        // X-20: tendederos de papel picado cruzando la playa. Cada tira se mece como un pendulo de pequena amplitud: theta = A*sen(2*pi*t/T + fase)
        {
            const float tt = (float)Reloj();
            glUniform1f(specLoc, 0.1f);
            for (int t = 0; t < Extras::N_TENDEDEROS; t++)
                for (int i = 0; i < Extras::TENDEDEROS[t].n; i++)
                {
                    const float cx = Extras::TENDEDEROS[t].x0 + Extras::ANCHO_BASTIDOR * (i + 0.5f);
                    const float cz = Extras::TENDEDEROS[t].z;
                    colocar(soportes, glm::vec3(cx, Colisiones::alturaSuelo(cz), cz), 0.0f);
                    glm::mat4 mp = glm::translate(glm::mat4(1.0f), glm::vec3(cx, Colisiones::alturaSuelo(cz) + Extras::ALTO_PAPEL, cz));
                    mp = glm::rotate(mp, glm::radians(Extras::Balanceo(tt, 3.6f, 0.8f * i + 2.1f * t, 6.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp));
                    papelPicado.Draw(shader);
                }
            // ofrendas al pie de las tumbas y la cruz: una vela y tres flores
            glUniform1f(specLoc, 0.05f);
            for (int i = 0; i < Extras::N_OFRENDAS_TUMBA; i++)
            {
                const Extras::OfrendaTumba& o = Extras::OFRENDAS_TUMBA[i];
                colocar(vela, glm::vec3(o.x + 0.25f, 0.0f, o.z), 0.0f);
                for (int k = 0; k < 3; k++)
                {
                    glm::mat4 mf = glm::translate(glm::mat4(1.0f), glm::vec3(o.x - 0.2f + 0.14f * k, 0.06f, o.z + 0.05f * (k % 2)));
                    mf = glm::scale(mf, glm::vec3(1.7f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mf));
                    cempasuchil.Draw(shader);
                }
            }
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tt);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
            for (int i = 0; i < Extras::N_OFRENDAS_TUMBA; i++)
            {
                const Extras::OfrendaTumba& o = Extras::OFRENDAS_TUMBA[i];
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 1.7f * i + 0.4f);
                glm::mat4 ml = glm::translate(glm::mat4(1.0f), glm::vec3(o.x + 0.25f, 0.15f, o.z));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                llamaModelo.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
        }
        // X-21: personas sobre tablas de remo. Cada tabla recorre un circulo lento (Extras::PosicionTabla), flota sobre la ola
        // (Olas::Altura) y alinea su vertical con la normal de la ola (Olas::Normal): sin KeyFrames.
        {
            const float tb = (float)Reloj();
            glUniform1f(specLoc, 0.3f);
            for (int i = 0; i < Extras::N_TABLAS; i++)
            {
                float bx, bz, dx, dz, nx, ny, nz;
                Extras::PosicionTabla(Extras::TABLAS[i], tb, bx, bz, dx, dz);
                Olas::Normal(bx, bz, tb, nx, ny, nz);
                const float by = Olas::Altura(bx, bz, tb) + 0.03f;
                const float yaw = std::atan2(-dx, -dz);                                   // la proa (-z local) apunta a (dx, dz)
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(bx, by, bz));
                const glm::vec3 eje = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(nx, ny, nz));
                if (glm::length(eje) > 1e-5f) mm = glm::rotate(mm, std::acos(glm::clamp(ny, -1.0f, 1.0f)), glm::normalize(eje));
                mm = glm::rotate(mm, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                tablaRemo.Draw(shader);
                glm::mat4 mp = glm::translate(mm, glm::vec3(0.0f, 0.12f, 0.0f));          // la persona de pie sobre la cubierta
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp));
                atuendos.dibujar(shader, Extras::TABLAS[i].persona == 0 ? personaPieA : personaPieB, false, 400 + i, mp);   // X-35
            }
            glUniform1f(specLoc, 0.05f);
        }
        // X-22: luciernagas (Extras::PosicionLuciernaga / BrilloLuciernaga, curvas de Lissajous; no son un sistema de particulas de humo)
        {
            const float tl = (float)Reloj();
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 2);
            for (int i = 0; i < Extras::N_LUCIERNAGAS; i++)
            {
                float lx, ly, lz;
                Extras::PosicionLuciernaga(i, tl, lx, ly, lz);
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), Extras::BrilloLuciernaga(i, tl));
                glm::mat4 ml = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(lx, ly, lz)), glm::vec3(4.0f));  // X-36: 6 cm (antes 2.7 cm: no se veian a distancia)
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                luciernaga.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
        }
        // X-24: narrativa de la ruta en 5 estaciones (ver docs/EXTRAS_Y_MEJORAS.md). Todo con matematica simple, sin KeyFrames.
        {
            const float tn = (float)Reloj();
            glUniform1f(specLoc, 0.05f);
            Model* tiposN[4] = { &personaPieA, &personaPieB, &personaAgachadaA, &personaAgachadaB };
            for (int i = 0; i < Extras::N_FIGURAS_N; i++)
            {
                const Extras::FiguraN& f = Extras::FIGURAS_N[i];
                const bool agachada = f.tipo >= 2;
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(f.x, Colisiones::alturaSuelo(f.z), f.z));
                mm = glm::rotate(mm, glm::radians(f.giroY), glm::vec3(0.0f, 1.0f, 0.0f));
                if (agachada) mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tn, 5.0f, f.fase, 2.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                else          mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tn, 4.5f, f.fase, 1.2f)), glm::vec3(0.0f, 0.0f, 1.0f));
                mm = glm::scale(mm, glm::vec3(f.esc));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                atuendos.dibujar(shader, *tiposN[f.tipo], agachada, 100 + i, mm);   // X-35
                if (f.inst == 1)   // musico: la guitarra cruza el pecho y se mece al compas (+-4 grados, 1.3 s)
                {
                    glm::mat4 mg = glm::translate(mm, glm::vec3(0.05f, 1.05f, -0.16f));
                    mg = glm::rotate(mg, glm::radians(58.0f + Extras::Balanceo(tn, 1.3f, f.fase, 4.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mg));
                    guitarra.Draw(shader);
                }
            }
            for (int i = 0; i < Extras::N_CESTAS; i++)
            {
                const glm::vec3 base(Extras::CESTAS[i].x, Colisiones::alturaSuelo(Extras::CESTAS[i].z), Extras::CESTAS[i].z);
                colocar(cesta, base, 20.0f * i);
                for (int k = 0; k < 5; k++)
                {
                    glm::mat4 mf = glm::translate(glm::mat4(1.0f), base + glm::vec3(0.08f * std::cos(1.3f * k + i), 0.19f, 0.08f * std::sin(1.3f * k + i)));
                    mf = glm::scale(mf, glm::vec3(1.5f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mf));
                    cempasuchil.Draw(shader);
                }
            }
            // camino de cempasuchil de la orilla a la puerta del cuarto
            const int nFlores = Extras::NumFloresSendero();
            for (int i = 0; i < nFlores; i++)
            {
                float fx, fz;
                if (!Extras::FlorSenderoEn(i, fx, fz)) continue;
                glm::mat4 mf = glm::translate(glm::mat4(1.0f), glm::vec3(fx, Colisiones::alturaSuelo(fz) + 0.035f, fz));
                mf = glm::rotate(mf, glm::radians(37.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
                mf = glm::scale(mf, glm::vec3(1.25f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mf));
                cempasuchil.Draw(shader);
            }
            // faroles: poste y luz calida (esfera emisiva que parpadea con dos senos; fase distinta por farol)
            glUniform1f(specLoc, 0.15f);
            for (int i = 0; i < Extras::N_FAROLES; i++)
                colocar(farol, glm::vec3(Extras::FAROLES[i].x, Colisiones::alturaSuelo(Extras::FAROLES[i].z), Extras::FAROLES[i].z), 90.0f * (i % 4));
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 3);
            for (int i = 0; i < Extras::N_FAROLES; i++)
            {
                const float gir = glm::radians(90.0f * (i % 4));
                const glm::vec3 ob(Extras::FAROLES[i].x + 0.14f * std::cos(gir), Colisiones::alturaSuelo(Extras::FAROLES[i].z) + 1.52f, Extras::FAROLES[i].z - 0.14f * std::sin(gir));
                const float br = 0.55f + 0.30f * std::sin(5.1f * tn + 1.7f * i) + 0.15f * std::sin(11.3f * tn + 0.9f * i);   // 0.10 .. 1.00
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), br);
                glm::mat4 mo = glm::scale(glm::translate(glm::mat4(1.0f), ob), glm::vec3(4.4f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mo));
                luciernaga.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            glUniform1f(specLoc, 0.05f);
            // veladoras a los lados del sendero (cada 2.2 m, a +-1.3 m del eje) con su llama
            std::vector<glm::vec3> velasSendero;
            for (float z = 21.0f; z > 3.5f; z -= 2.2f)
            {
                if ((z > 2.7f && z < 3.6f) || (z > 11.3f && z < 12.4f)) continue;      // no sobre los muros de la barda
                for (int lado = -1; lado <= 1; lado += 2)
                    velasSendero.push_back(glm::vec3(Extras::EjeSendero(z) + 1.3f * lado, Colisiones::alturaSuelo(z), z));
            }
            for (const glm::vec3& v : velasSendero) colocar(vela, v, 0.0f);
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tn);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
            int nl2 = 0;
            for (const glm::vec3& v : velasSendero)
            {
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.61f * (nl2++) + 7.0f);
                glm::mat4 ml = glm::translate(glm::mat4(1.0f), v + glm::vec3(0.0f, 0.15f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                llamaModelo.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            // murcielagos al anochecer: circulos de radio 10..26 m, altura 6.5..14 m; alas con theta = 40 grados * sen(2*pi*3.2*t)
            glUniform1f(specLoc, 0.02f);
            for (int i = 0; i < Extras::N_MURCIELAGOS; i++)
            {
                float mx, my, mz, dx, dz;
                Extras::PosicionMurcielago(Extras::MURCIELAGOS[i], tn, mx, my, mz, dx, dz);
                glm::mat4 mb = glm::translate(glm::mat4(1.0f), glm::vec3(mx, my, mz));
                mb = glm::rotate(mb, std::atan2(-dx, -dz), glm::vec3(0.0f, 1.0f, 0.0f));
                mb = glm::rotate(mb, glm::radians(-14.0f), glm::vec3(0.0f, 0.0f, 1.0f));      // se inclina hacia el centro del giro
                mb = glm::scale(mb, glm::vec3(1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mb));
                murcielagoCuerpo.Draw(shader);
                const float th = Extras::AMP_ALETEO * std::sin(2.0f * Extras::PI * Extras::FREC_ALETEO * tn + 1.7f * i);
                glm::mat4 mw = glm::rotate(mb, glm::radians(th), glm::vec3(0.0f, 0.0f, 1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mw));
                murcielagoAla.Draw(shader);
                glm::mat4 mw2 = glm::rotate(glm::scale(mb, glm::vec3(-1.0f, 1.0f, 1.0f)), glm::radians(th), glm::vec3(0.0f, 0.0f, 1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mw2));
                murcielagoAla.Draw(shader);
            }
            glUniform1f(specLoc, 0.05f);
        }
        // ===== X-25..X-28: los lados de la escena (investigacion de Mazunte en docs/investigacion_mazunte.md). Sin KeyFrames: solo formulas simples =====
        if (!std::getenv("NIERIKA_SIN_LADOS"))   // solo para las capturas "antes" de docs/EXTRAS_Y_MEJORAS.md
        {
            const float tl = (float)Reloj();
            auto enSuelo = [&](float z) { return z < 23.5f ? Colisiones::alturaSuelo(z) : -0.30f; };
            auto poner = [&](Model& m, glm::vec3 pos, float giro, float esc)
            {
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), pos);
                mm = glm::rotate(mm, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::scale(mm, glm::vec3(esc));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                m.Draw(shader);
            };
            // sobre la ola: posicion (x, z), altura = Olas::Altura, vertical = normal de la ola; yaw = giro de proa
            auto sobreOla = [&](float x, float z, float yawGrados, float subir) -> glm::mat4
            {
                float nx, ny, nz;
                Olas::Normal(x, z, tl, nx, ny, nz);
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(x, Olas::Altura(x, z, tl) + subir, z));
                const glm::vec3 eje = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(nx, ny, nz));
                if (glm::length(eje) > 1e-5f) mm = glm::rotate(mm, std::acos(glm::clamp(ny, -1.0f, 1.0f)), glm::normalize(eje));
                return glm::rotate(mm, glm::radians(yawGrados), glm::vec3(0.0f, 1.0f, 0.0f));
            };
            glUniform1f(specLoc, 0.05f);
            for (int i = 0; i < Extras::N_ROCAS; i++)
                poner(rocaMar, glm::vec3(Extras::ROCAS[i].x, enSuelo(Extras::ROCAS[i].z), Extras::ROCAS[i].z), Extras::ROCAS[i].giro, Extras::ROCAS[i].esc);
            for (int i = 0; i < Extras::N_PALMAS_LADOS; i++)
                poner(palma, glm::vec3(Extras::PALMAS_LADOS[i].x, 0.0f, Extras::PALMAS_LADOS[i].z), Extras::PALMAS_LADOS[i].giro, Extras::PALMAS_LADOS[i].esc);
            glUniform1f(specLoc, 0.2f);
            for (int i = 0; i < Extras::N_LANCHAS_VARADAS; i++)
                poner(lancha, glm::vec3(Extras::LANCHAS_VARADAS[i].x, enSuelo(Extras::LANCHAS_VARADAS[i].z) + 0.05f, Extras::LANCHAS_VARADAS[i].z), Extras::LANCHAS_VARADAS[i].giro, 1.0f);
            for (int i = 0; i < Extras::N_LANCHAS_AGUA; i++)       // ancladas: suben y bajan con la ola y giran +-8 grados lentamente (T = 12 s)
            {
                const Extras::Obj& l = Extras::LANCHAS_AGUA[i];
                glm::mat4 mm = sobreOla(l.x, l.z, l.giro + Extras::Balanceo(tl, 12.0f, l.esc, 8.0f), 0.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                lancha.Draw(shader);
            }
            glUniform1f(specLoc, 0.05f);
            for (int i = 0; i < Extras::N_REDES; i++)              // red colgada de un bastidor de soportes estirado x1.3; se mece 3 grados (T = 5 s)
            {
                const Extras::Obj& r = Extras::REDES[i];
                poner(soportes, glm::vec3(r.x, enSuelo(r.z), r.z), r.giro, r.esc);
                glm::mat4 mn = glm::translate(glm::mat4(1.0f), glm::vec3(r.x, enSuelo(r.z) + 1.95f, r.z));
                mn = glm::rotate(mn, glm::radians(Extras::Balanceo(tl, 5.0f, 1.3f * i, 3.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mn));
                redPesca.Draw(shader);
            }
            for (int i = 0; i < Extras::N_FOGATAS; i++)
                poner(fogata, glm::vec3(Extras::FOGATAS[i].x, enSuelo(Extras::FOGATAS[i].z), Extras::FOGATAS[i].z), 0.0f, 1.0f);
            for (int i = 0; i < Extras::N_HAMACAS; i++)            // la tela se balancea +-2 grados (T = 6 s) alrededor de su eje largo
            {
                const Extras::Obj& h = Extras::HAMACAS[i];
                glm::mat4 mh = glm::translate(glm::mat4(1.0f), glm::vec3(h.x, enSuelo(h.z), h.z));
                mh = glm::rotate(mh, glm::radians(Extras::Balanceo(tl, 6.0f, 2.0f * i, 2.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mh));
                hamaca.Draw(shader);
            }
            if (fueraAlcance)   // FUERA DEL ALCANCE ORIGINAL (decision del 13/11/2026): palapas y casas costeras
            {
                for (int i = 0; i < Extras::N_PALAPAS; i++)
                    poner(palapa, glm::vec3(Extras::PALAPAS[i].x, Colisiones::alturaSuelo(Extras::PALAPAS[i].z), Extras::PALAPAS[i].z), Extras::PALAPAS[i].giro, 1.0f);
                for (int i = 0; i < Extras::N_CASAS; i++)
                    poner(Extras::CASAS[i].tipo == 0 ? casaA : casaB, glm::vec3(Extras::CASAS[i].x, 0.0f, Extras::CASAS[i].z), Extras::CASAS[i].giro, 1.0f);
            }
            if (fueraAlcance)   // X-29 (FUERA DEL ALCANCE ORIGINAL): alfombras de petalos, banda de musicos y comparsa
            {
                glUniform1f(specLoc, 0.02f);
                for (int i = 0; i < Extras::N_ALFOMBRAS; i++)
                {
                    const Extras::Alfombra& al = Extras::ALFOMBRAS[i];
                    glm::mat4 ma = glm::translate(glm::mat4(1.0f), glm::vec3(al.x, Colisiones::alturaSuelo(al.z) + 0.01f, al.z));
                    ma = glm::scale(ma, glm::vec3(al.ancho, 1.0f, al.largo));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ma));
                    (al.tipo == 1 ? alfombraCruz : alfombraFlores).Draw(shader);
                }
                glUniform1f(specLoc, 0.05f);
                // X-40: banda de viento. Cada musico lleva uniforme (ropa_14), a escala 1, y su instrumento se dibuja con la MISMA matriz (esta modelado en sus coordenadas).
                // Extra ambiental simple (no es KeyFrame): balanceo de +-2 grados al compas, theta(t) = 2 sen(2 pi t / T + fase), T = 2 * 60 / BPM.
                Model* poseMusico[8] = { &musicoViento, &musicoViento, &musicoViento, &musicoTrombon, &musicoTuba, &musicoPercusion, &musicoPercusion, &musicoPlatillos };
                for (int i = 0; i < Extras::N_BANDA; i++)
                {
                    const Extras::Musico& mu = Extras::BANDA[i];
                    glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(mu.x, Colisiones::alturaSuelo(mu.z), mu.z));
                    mm = glm::rotate(mm, glm::radians(mu.giro), glm::vec3(0.0f, 1.0f, 0.0f));
                    mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tl, 60.0f / Extras::BPM_BANDA * 2.0f, mu.fase, 2.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
                    atuendos.dibujar(shader, *poseMusico[mu.inst], false, 600 + i, mm, false, 14);   // X-35 + X-40: uniforme, sin talla
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                    glUniform1f(specLoc, 0.40f);   // metales y parches: brillo mayor que el de la ropa
                    bandaInst[mu.inst].Draw(shader);
                    glUniform1f(specLoc, 0.05f);
                }
                // comparsa: 12 figuras marchando por la orilla; cada 5.a lleva un tambor; cada par de adelante sostiene una tira de papel picado
                for (int i = 0; i < Extras::N_COMPARSA; i++)
                {
                    float cx, cz, cy;
                    Extras::ComparsaN(i, tl, cx, cz, cy);
                    glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(cx, Colisiones::alturaSuelo(cz) + cy, cz));
                    mm = glm::rotate(mm, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));          // avanzan hacia +x
                    mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tl, 1.0f / Extras::COMPARSA_F * 2.0f, 0.6f * i, 3.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                    atuendos.dibujar(shader, i % 3 == 0 ? personaPieB : personaPieA, false, 300 + i, mm);   // X-35
                    if (i % 5 == 0)
                    {
                        glm::mat4 mt = glm::translate(mm, glm::vec3(0.0f, 0.62f, -0.38f));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mt));
                        tambor.Draw(shader);
                    }
                }
                {   // tira de papel picado entre las dos primeras columnas, mecida como pendulo (6 grados, 2.4 s)
                    float cx0, cz0, cy0, cx1, cz1, cy1;
                    Extras::ComparsaN(0, tl, cx0, cz0, cy0);  Extras::ComparsaN(1, tl, cx1, cz1, cy1);
                    glm::mat4 mp = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f * (cx0 + cx1), Colisiones::alturaSuelo(22.6f) + 2.2f, 0.5f * (cz0 + cz1)));
                    mp = glm::rotate(mp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    mp = glm::rotate(mp, glm::radians(Extras::Balanceo(tl, 2.4f, 0.0f, 6.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                    mp = glm::scale(mp, glm::vec3(1.0f, 1.0f, 1.0f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp));
                    papelPicado.Draw(shader);
                }
            }
            if (fueraAlcance)   // X-30 (FUERA DEL ALCANCE ORIGINAL): pueblo de fondo sobre las colinas y promontorio tipo Punta Cometa
            {
                glUniform1f(specLoc, 0.03f);
                glm::mat4 mcal(1.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mcal));
                calles.Draw(shader);
                cimientos.Draw(shader);   // X-53: las casas de las laderas se apoyan en su punto mas alto; el cimiento llena el hueco hasta el suelo
                for (int i = 0; i < PuebloFondo::N_CASAS; i++)
                    poner(PuebloFondo::CASAS[i].tipo == 0 ? casaA : casaB, glm::vec3(PuebloFondo::CASAS[i].x, PuebloFondo::CASAS[i].y, PuebloFondo::CASAS[i].z), PuebloFondo::CASAS[i].giro, 1.0f);
                for (int i = 0; i < PuebloFondo::N_ROCAS; i++)
                    poner(rocaMar, glm::vec3(PuebloFondo::ROCAS[i].x, PuebloFondo::ROCAS[i].y, PuebloFondo::ROCAS[i].z), PuebloFondo::ROCAS[i].giro, PuebloFondo::ROCAS[i].esc);
                poner(palapa, glm::vec3(PuebloFondo::MIRADOR_X, PuebloFondo::MIRADOR_Y, PuebloFondo::MIRADOR_Z), 30.0f, 0.8f);   // mirador pequeno sobre la roca mayor
                glUniform1f(specLoc, 0.05f);
            }
            // tortugas marinas nadando en la superficie (Extras::PosicionTortuga) con cabeceo de aletas
            glUniform1f(specLoc, 0.3f);
            for (int i = 0; i < Extras::N_TORTUGAS; i++)
            {
                float tx, tz, dx, dz;
                Extras::PosicionTortuga(Extras::TORTUGAS[i], tl, tx, tz, dx, dz);
                glm::mat4 mm = sobreOla(tx, tz, glm::degrees(std::atan2(-dx, -dz)), 0.02f);
                mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tl, 1.8f, 1.1f * i, 10.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
                mm = glm::scale(mm, glm::vec3(0.85f));  // X-32: caparazon de ~0.8 m
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                tortugaMar.Draw(shader);
            }
            // crias de tortuga que cruzan la arena hacia el mar y vuelven a empezar (z = Z0 + (v t + fase) mod L), con bamboleo de +-8 grados
            glUniform1f(specLoc, 0.1f);
            for (int i = 0; i < Extras::N_CRIAS; i++)
            {
                const float cz = Extras::CriaZ(i, tl), cx = Extras::CriaX(i);
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(cx, Colisiones::alturaSuelo(cz) + 0.02f, cz));
                mm = glm::rotate(mm, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tl, 0.9f, 1.7f * i, 8.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
                mm = glm::scale(mm, glm::vec3(0.12f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                tortugaMar.Draw(shader);
            }
            // aves: circulos de 9 a 20 m de radio a 9..20 m de altura; alas con theta = 25 grados * sen(2*pi*0.9*t + fase)
            glUniform1f(specLoc, 0.05f);
            for (int i = 0; i < Extras::N_AVES; i++)
            {
                float ax, ay, az, dx, dz;
                Extras::PosicionAve(Extras::AVES[i], tl, ax, ay, az, dx, dz);
                glm::mat4 mb = glm::translate(glm::mat4(1.0f), glm::vec3(ax, ay, az));
                mb = glm::rotate(mb, std::atan2(-dx, -dz), glm::vec3(0.0f, 1.0f, 0.0f));
                mb = glm::rotate(mb, glm::radians(-12.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                mb = glm::scale(mb, glm::vec3(1.2f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mb));
                aveCuerpo.Draw(shader);
                const float th = Extras::AMP_ALETEO_AVE * std::sin(2.0f * Extras::PI * Extras::FREC_ALETEO_AVE * tl + 1.3f * i);
                glm::mat4 mw = glm::rotate(mb, glm::radians(th), glm::vec3(0.0f, 0.0f, 1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mw));
                aveAla.Draw(shader);
                glm::mat4 mw2 = glm::rotate(glm::scale(mb, glm::vec3(-1.0f, 1.0f, 1.0f)), glm::radians(th), glm::vec3(0.0f, 0.0f, 1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mw2));
                aveAla.Draw(shader);
            }
            // emisivos: llamas de las fogatas (X-49: modelo fuego de D1, 3 lenguas con parpadeo por senos; sin humo) y linternas de papel flotando (orbe calido que sube y baja con la ola)
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tl);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 5);
            for (int i = 0; i < Extras::N_FOGATAS; i++)
                for (int l = 0; l < LlamasFogata::N_LENGUAS; l++)
                {
                    glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), LlamasFogata::Brillo(i, l, tl));
                    glm::mat4 ml = LlamasFogata::Modelo(glm::vec3(Extras::FOGATAS[i].x, enSuelo(Extras::FOGATAS[i].z) + 0.16f, Extras::FOGATAS[i].z), i, l, tl);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                    fuegoLlama.Draw(shader);
                }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 3);
            for (int i = 0; i < Extras::N_LINTERNAS; i++)
            {
                float lx, lz;
                Extras::LinternaN(i, lx, lz);
                const float br = 0.55f + 0.30f * std::sin(3.1f * tl + 1.9f * i) + 0.15f * std::sin(7.7f * tl + 0.7f * i);
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), br);
                glm::mat4 mo = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(lx, Olas::Altura(lx, lz, tl) + 0.06f, lz)), glm::vec3(5.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mo));
                luciernaga.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            glUniform1f(specLoc, 0.05f);
        }
        // X-18: colinas verdes de fondo y su vegetacion (50 m detras de la barda; no son Punta Cometa)
        {
            glUniform1f(specLoc, 0.02f);
            glm::mat4 mc(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mc));
            glUniform1i(glGetUniformLocation(shader.Program, "colina"), 1);   // X-36: manchas de color a gran escala (lighting.frag)
            colinas.Draw(shader);
            glUniform1i(glGetUniformLocation(shader.Program, "colina"), 0);
            auto conEscala = [&](Model& m, const VegetacionColinas::Instancia& v)
            {
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(v.x, v.y, v.z));
                mm = glm::rotate(mm, glm::radians(v.giro), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::scale(mm, glm::vec3(v.escala));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                m.Draw(shader);
            };
            for (int i = 0; i < VegetacionColinas::N_PALMAS; i++) conEscala(palma, VegetacionColinas::PALMAS[i]);
            for (int i = 0; i < VegetacionColinas::N_ARBUSTOS; i++) conEscala(arbusto, VegetacionColinas::ARBUSTOS[i]);
            glUniform1f(specLoc, 0.05f);
        }
        // ===== X-33 / X-34 / X-35: VIDA EN LAS COLINAS Y EN LA FRANJA NORTE DE LA PLAYA =====
        // Senderos de cempasuchil con faroles que suben a las colinas (norte, este, oeste), un campamento con fogata en cada mirador y otra fogata
        // en cada cima, 4 campamentos y 2 palapas en la franja norte de la playa, mas vegetacion, luciernagas y gente con atuendos propios.
        // Posiciones: src/VidaColinasDatos.h (generado). Para compararlo sin esta capa (FPS): NIERIKA_SIN_VIDA=1. Sin KeyFrames ni particulas.
        if (!std::getenv("NIERIKA_SIN_VIDA"))
        {
            namespace VD = VidaColinasDatos;
            const float tv = (float)Reloj();
            const glm::vec3 cam = camera.GetPosition();
            auto lejos = [&](float x, float z, float r) { const float dx = x - cam.x, dz = z - cam.z; return dx * dx + dz * dz > r * r; };   // solo se dibuja lo cercano
            auto en = [&](Model& m, float x, float y, float z, float giro, float esc)
            {
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
                mm = glm::rotate(mm, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::scale(mm, glm::vec3(esc));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                m.Draw(shader);
            };
            glUniform1f(specLoc, 0.02f);
            for (int i = 0; i < VD::N_PALMAS; i++)
                if (!lejos(VD::PALMAS[i].x, VD::PALMAS[i].z, 130.0f)) en(palma, VD::PALMAS[i].x, VD::PALMAS[i].y, VD::PALMAS[i].z, VD::PALMAS[i].giro, VD::PALMAS[i].esc);
            for (int i = 0; i < VD::N_ARBUSTOS; i++)
                if (!lejos(VD::ARBUSTOS[i].x, VD::ARBUSTOS[i].z, 100.0f)) en(arbusto, VD::ARBUSTOS[i].x, VD::ARBUSTOS[i].y, VD::ARBUSTOS[i].z, VD::ARBUSTOS[i].giro, VD::ARBUSTOS[i].esc);
            glUniform1f(specLoc, 0.05f);
            // palapas de la franja norte (las del mirador no: el suelo de la colina esta inclinado)
            if (fueraAlcance)   // las palapas estan fuera del alcance original (ESTADO, decision del 13/11)
                for (int i = 0; i < VD::N_PALAPAS; i++) en(palapa, VD::PALAPAS[i].x, Colisiones::alturaTerreno(VD::PALAPAS[i].x, VD::PALAPAS[i].z), VD::PALAPAS[i].z, VD::PALAPAS[i].giro, 1.0f);
            // X-36 · CAMINO DE LAS ANIMAS: sendero de tierra (una malla que sigue la ladera) con racimos de cempasuchil en las dos orillas (la tradicion
            // guia a las animas con flores de cempasuchil), arcos de carrizo y flores, tendederos de papel picado, ofrenda con cruz en cada mirador,
            // rocas y arboles. Todo son modelos ya definidos o extras de X-36; sin KeyFrames (el papel picado se mece con un seno).
            {
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
                glUniform1f(specLoc, 0.02f);
                senderoTierra.Draw(shader);
                for (int i = 0; i < VD::N_ARBOLES; i++)
                    if (!lejos(VD::ARBOLES[i].x, VD::ARBOLES[i].z, 110.0f)) en(arbolSelva, VD::ARBOLES[i].x, VD::ARBOLES[i].y, VD::ARBOLES[i].z, VD::ARBOLES[i].giro, VD::ARBOLES[i].esc);
                for (int i = 0; i < VD::N_ROCAS; i++)
                    if (!lejos(VD::ROCAS[i].x, VD::ROCAS[i].z, 90.0f))
                    {
                        // X-53: la roca se apoya en el plano de la ladera (su base plana se inclina con el suelo), asi no cuelga de un lado ni queda enterrada del otro
                        const VD::Planta& r = VD::ROCAS[i];
                        float gx, gz;  Terreno::inclinacion(r.x, r.z, r.esc, gx, gz);
                        glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(r.x, r.y, r.z));
                        const glm::vec3 nr = glm::normalize(glm::vec3(-gx, 1.0f, -gz));
                        const float ang = std::acos(glm::clamp(nr.y, -1.0f, 1.0f));
                        if (ang > 1e-4f) mm = glm::rotate(mm, ang, glm::normalize(glm::vec3(nr.z, 0.0f, -nr.x)));
                        mm = glm::rotate(mm, glm::radians(r.giro), glm::vec3(0.0f, 1.0f, 0.0f));
                        mm = glm::scale(mm, glm::vec3(r.esc));
                        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                        rocaMar.Draw(shader);
                    }
                glUniform1f(specLoc, 0.05f);
                for (int i = 0; i < VD::N_RACIMOS; i++)
                    if (!lejos(VD::RACIMOS[i].x, VD::RACIMOS[i].z, 60.0f)) en(racimoCemp, VD::RACIMOS[i].x, VD::RACIMOS[i].y, VD::RACIMOS[i].z, VD::RACIMOS[i].giro, 1.0f);
                for (int i = 0; i < VD::N_ARCOS; i++)
                    if (!lejos(VD::ARCOS[i].x, VD::ARCOS[i].z, 90.0f)) en(arcoCemp, VD::ARCOS[i].x, VD::ARCOS[i].y, VD::ARCOS[i].z, VD::ARCOS[i].giro, 1.0f);
                glUniform1f(specLoc, 0.1f);
                for (int i = 0; i < VD::N_TENDEDEROS; i++)     // bastidor de 2 postes + tira de papel picado que se mece (6 grados, 3.6 s), como X-20
                {
                    const VD::Punto& t = VD::TENDEDEROS[i];
                    if (lejos(t.x, t.z, 70.0f)) continue;
                    en(soportes, t.x, t.y, t.z, t.giro, 1.0f);
                    glm::mat4 mp = glm::translate(glm::mat4(1.0f), glm::vec3(t.x, t.y + Extras::ALTO_PAPEL, t.z));
                    mp = glm::rotate(mp, glm::radians(t.giro), glm::vec3(0.0f, 1.0f, 0.0f));
                    mp = glm::rotate(mp, glm::radians(Extras::Balanceo(tv, 3.6f, 1.3f * i, 6.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mp));
                    papelPicado.Draw(shader);
                }
                // ofrenda con cruz en cada mirador: cruz, 5 velas, 3 racimos, pan, botella y copalero (cada pieza se apoya a su propia altura del terreno)
                std::vector<glm::vec3> llamasOfrenda;
                for (int i = 0; i < VD::N_OFRENDAS; i++)
                {
                    const VD::Punto& o = VD::OFRENDAS[i];
                    if (lejos(o.x, o.z, 80.0f)) continue;
                    const float cg = std::cos(glm::radians(o.giro)), sg = std::sin(glm::radians(o.giro));
                    auto sobre = [&](Model& m, float lx, float lz, float esc, float subir, float giro)
                    {
                        const float wx = o.x + lx * cg + lz * sg, wz = o.z - lx * sg + lz * cg;
                        const float wy = o.y + (Terreno::altura(wx, wz) - Terreno::altura(o.x, o.z)) + subir;
                        en(m, wx, wy, wz, o.giro + giro, esc);
                        return glm::vec3(wx, wy, wz);
                    };
                    sobre(cruz, 0.0f, 0.0f, 1.0f, -0.06f, 0.0f);
                    const float vl[5][2] = { { -0.55f, 0.55f }, { 0.55f, 0.55f }, { -0.95f, 0.15f }, { 0.95f, 0.15f }, { 0.0f, 0.95f } };
                    for (int k = 0; k < 5; k++) llamasOfrenda.push_back(sobre(vela, vl[k][0], vl[k][1], 1.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.15f, 0.0f));
                    sobre(racimoCemp, -0.30f, 0.65f, 1.3f, 0.0f, 20.0f); sobre(racimoCemp, 0.30f, 0.65f, 1.3f, 0.0f, 70.0f); sobre(racimoCemp, 0.0f, 0.40f, 1.1f, 0.0f, 140.0f);
                    sobre(panMuerto, 0.20f, 0.80f, 1.0f, 0.0f, 0.0f); sobre(botella, -0.25f, 0.80f, 1.0f, 0.0f, 0.0f); sobre(copalero, 0.45f, 0.75f, 1.0f, 0.0f, 0.0f);
                }
                glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tv);
                glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
                int nlo = 0;
                for (const glm::vec3& v : llamasOfrenda)
                {
                    glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.77f * (nlo++) + 3.0f);
                    glm::mat4 ml = glm::translate(glm::mat4(1.0f), v);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                    llamaModelo.Draw(shader);
                }
                glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            }
            // fogatas: base de piedras y troncos + llama del shader (sin humo); la base se hunde 8 cm porque el suelo de la colina esta inclinado
            for (int i = 0; i < VD::N_FOGATAS; i++)
                if (!lejos(VD::FOGATAS[i].x, VD::FOGATAS[i].z, 110.0f)) en(fogata, VD::FOGATAS[i].x, VD::FOGATAS[i].y - 0.08f, VD::FOGATAS[i].z, 37.0f * i, 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tv);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 5);   // X-49: modelo fuego de D1 (3 lenguas por fogata)
            for (int i = 0; i < VD::N_FOGATAS; i++)
            {
                if (lejos(VD::FOGATAS[i].x, VD::FOGATAS[i].z, 110.0f)) continue;
                for (int l = 0; l < LlamasFogata::N_LENGUAS; l++)
                {
                    glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), LlamasFogata::Brillo(10 + i, l, tv));
                    glm::mat4 ml = LlamasFogata::Modelo(glm::vec3(VD::FOGATAS[i].x, VD::FOGATAS[i].y + 0.16f, VD::FOGATAS[i].z), 10 + i, l, tv);
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                    fuegoLlama.Draw(shader);
                }
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            // X-49: mariposas de las colinas (modelos de D1; vuelo y aleteo en Mariposas.h). Emisivas tenues (llama = 5, factor 0.75) para que se vean de noche;
            // se dibujan solo las que estan a menos de 70 m. Sin colision: no tocan nada ni cuentan para las AABB del visitante.
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 5);
            glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.75f);
            for (int i = 0; i < MariposasDatos::N_MARIPOSAS; i++)
            {
                if (lejos(MariposasDatos::MARIPOSAS[i].cx, MariposasDatos::MARIPOSAS[i].cz, 70.0f)) continue;
                const Mariposas::Pose pm = Mariposas::PoseEn(i, tv);
                const glm::mat4 mc = Mariposas::Cuerpo(pm);
                const float d = Mariposas::Aleteo(i, tv);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mc));
                mariCuerpo.Draw(shader);
                glm::mat4 mi = Mariposas::Ala(mc, d, +1);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mi));
                mariAlaIzq.Draw(shader);
                glm::mat4 md = Mariposas::Ala(mc, d, -1);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(md));
                mariAlaDer.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            // faroles: poste + orbe emisivo que parpadea (igual que X-24)
            glUniform1f(specLoc, 0.15f);
            for (int i = 0; i < VD::N_FAROLES; i++)
                if (!lejos(VD::FAROLES[i].x, VD::FAROLES[i].z, 110.0f)) en(farol, VD::FAROLES[i].x, VD::FAROLES[i].y, VD::FAROLES[i].z, VD::FAROLES[i].giro, 1.0f);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 3);
            for (int i = 0; i < VD::N_FAROLES; i++)
            {
                if (lejos(VD::FAROLES[i].x, VD::FAROLES[i].z, 110.0f)) continue;
                const float gir = glm::radians(VD::FAROLES[i].giro);
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), VidaColinas::BrilloFarol(100 + i, tv));
                glm::mat4 mo = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(VD::FAROLES[i].x + 0.14f * std::cos(gir), VD::FAROLES[i].y + 1.52f, VD::FAROLES[i].z - 0.14f * std::sin(gir))), glm::vec3(4.4f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mo));
                luciernaga.Draw(shader);
            }
            // luciernagas de las colinas (curvas de Lissajous; no es un sistema de particulas)
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 2);
            for (int i = 0; i < VD::N_LUCIERNAGAS; i++)
            {
                if (lejos(VD::LUCIERNAGAS[i].cx, VD::LUCIERNAGAS[i].cz, 60.0f)) continue;
                float lx, ly, lz;
                VidaColinas::PosicionLuciernaga(i, tv, lx, ly, lz);
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), VidaColinas::BrilloLuciernaga(i, tv));
                glm::mat4 ml = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(lx, ly, lz)), glm::vec3(1.8f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                luciernaga.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            // gente: poses fijas con balanceo senoidal y atuendo propio (semilla 500 + indice)
            glUniform1f(specLoc, 0.05f);
            std::vector<glm::vec3> velasMano;
            Model* tiposV[4] = { &personaPieA, &personaPieB, &personaAgachadaA, &personaAgachadaB };
            for (int i = 0; i < VD::N_PERSONAS; i++)
            {
                const VD::Persona& p = VD::PERSONAS[i];
                if (lejos(p.x, p.z, 90.0f)) continue;
                const bool agachada = p.tipo >= 2;
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), glm::vec3(p.x, p.y, p.z));
                mm = glm::rotate(mm, glm::radians(p.giro), glm::vec3(0.0f, 1.0f, 0.0f));
                if (agachada) mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tv, 5.0f, p.fase, 2.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
                else          mm = glm::rotate(mm, glm::radians(Extras::Balanceo(tv, 4.5f, p.fase, 1.2f)), glm::vec3(0.0f, 0.0f, 1.0f));
                mm = glm::scale(mm, glm::vec3(p.esc));
                atuendos.dibujar(shader, *tiposV[p.tipo], agachada, 500 + i, mm);
                if (p.vela && !agachada && !lejos(p.x, p.z, 45.0f))      // X-36: vela en la mano derecha (procesion de las animas)
                {
                    const glm::mat4 mh = glm::translate(mm, glm::vec3(0.23f, 0.60f, -0.12f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mh));
                    vela.Draw(shader);
                    velasMano.push_back(glm::vec3(mh * glm::vec4(0.0f, 0.15f, 0.0f, 1.0f)));
                }
            }
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), tv);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);     // llamas de las velas en las manos
            for (size_t k = 0; k < velasMano.size(); k++)
            {
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 0.93f * (float)k + 9.0f);
                glm::mat4 ml = glm::translate(glm::mat4(1.0f), velasMano[k]);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
                llamaModelo.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
        }
        colocar(roca,     glm::vec3( 1.5f, 0.0f, 12.5f), 0.0f);  // B-10: roca junto a la entrada norte

        // C-4: flores flotando (circulo con velocidad variable y pausas + giro propio + altura de la ola; ver AnimFlores.h)
        glUniform1f(specLoc, 0.1f);
        for (int i = 0; i < 5; i++)
        {
            const float tf = (float)Reloj();
            glm::vec2 pf = PosicionXZ(floresFlotantes[i], tf);
            glm::mat4 mf = MatrizFlorFlotante(floresFlotantes[i], tf, Olas::Altura(pf.x, pf.y, tf), centroFlor);
            mf = glm::scale(mf, glm::vec3(ESCALA_FLOR_FLOTANTE));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mf));
            cempasuchil.Draw(shader);
        }
        // C-3: velas flotando (Bezier cuadratica de ida y vuelta + vaiven senoidal + altura de la ola; ver AnimVelas.h)
        for (int i = 0; i < 4; i++)
        {
            const float tv = (float)Reloj();
            float vx, vz;
            PosicionVelaXZ(velasFlotantes[i], tv, vx, vz);
            const float vy = Olas::Altura(vx, vz, tv) + CALADO_VELA + VaivenVela(velasFlotantes[i], tv);
            glm::mat4 mv = glm::translate(glm::mat4(1.0f), glm::vec3(vx, vy, vz));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mv));
            vela.Draw(shader);
        }
        // C-2: llamas de las velas flotantes (mismo ruido; fase distinta por vela)
        glUniform1i(glGetUniformLocation(shader.Program, "llama"), 1);
        for (int i = 0; i < 4; i++)
        {
            const float tv = (float)Reloj();
            float vx, vz;
            PosicionVelaXZ(velasFlotantes[i], tv, vx, vz);
            const float vy = Olas::Altura(vx, vz, tv) + CALADO_VELA + VaivenVela(velasFlotantes[i], tv);
            glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), 2.1f * i);
            glm::mat4 ml = glm::translate(glm::mat4(1.0f), glm::vec3(vx, vy + 0.15f, vz));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ml));
            llamaModelo.Draw(shader);
        }
        glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
        // 4. SILUETA (D-1/D-2): 10 piezas jerarquicas, caminata de la orilla (z = +24) al altar por KeyFrames con slerp y smoothstep.
        // Cada pieza se dibuja con la matriz de mundo de su nodo (el pivote del .obj es la articulacion)
        tSilueta += deltaTime;
        silueta.Actualizar(tSilueta);
        glUniform1f(specLoc, 0.1f);
        for (size_t i = 0; i < modelosSilueta.size(); i++)
        {
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(silueta.Nodos()[i].mundo));
            modelosSilueta[i].Draw(shader);
        }
        // D-4: flor de cempasuchil: en la mano hasta T_SUELTA; despues cae en tiro horizontal (Bezier + caida libre, DejarFlor.h) y se queda en el altar
        {
            const float tauFlor = tSilueta - silueta.Duracion();     // segundos desde que termino la caminata
            glm::vec3 pFlor = silueta.PosicionMano();
            if (tauFlor >= Silueta::T_SUELTA)
            {
                const float p0[3] = { manoSuelta.x, manoSuelta.y, manoSuelta.z };
                const float p2[3] = { destinoFlor.x, destinoFlor.y, destinoFlor.z };
                float salida[3];
                dejarflor::Posicion(p0, p2, tauFlor - Silueta::T_SUELTA, salida);
                pFlor = glm::vec3(salida[0], salida[1], salida[2]);
            }
            glm::mat4 mFlor = glm::translate(glm::mat4(1.0f), pFlor);
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mFlor));
            cempasuchil.Draw(shader);
        }

        // X-47: humo del copal (modelo `humo` de D1, sin particulas): una columna sobre cada copalero, semitransparente y sin escribir profundidad.
        // Va al final, despues de lo opaco. La textura es opaca: la transparencia es el uniforme humoAlfa (NIERIKA_HUMO_ALFA la cambia para pruebas).
        {
            const char* ea = std::getenv("NIERIKA_HUMO_ALFA");
            const float alfaHumo = ea ? (float)std::atof(ea) : 0.35f;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
            glUniform1i(glGetUniformLocation(shader.Program, "humo"), 1);
            glUniform1f(glGetUniformLocation(shader.Program, "humoAlfa"), alfaHumo);
            for (int s = -1; s <= 1; s += 2)
            {
                glm::mat4 mh = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.05f * s, 0.30f, 1.75f)), glm::vec3(0.62f));   // 0.62 -> 2.15 m, bajo el techo (2.62 m)
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mh));
                humo.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "humo"), 0);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }

        // X-36: halos calidos de fogatas y faroles (resplandor radial en 3 planos ortogonales, mezcla aditiva, sin escribir profundidad).
        // Va al FINAL de la escena para que lo opaco ya este dibujado. Solo lo cercano; llamaFase = intensidad con el mismo parpadeo que la luz.
        if (!std::getenv("NIERIKA_SIN_VIDA"))
        {
            const float th = (float)Reloj();
            const glm::vec3 camh = camera.GetPosition();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDepthMask(GL_FALSE);
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 4);
            for (const VidaColinas::Fuente& f : fuentesLuz)
            {
                const glm::vec3 d = f.pos - camh;
                const float lim = f.fogata ? 70.0f : 35.0f;
                if (d.x * d.x + d.z * d.z > lim * lim) continue;
                const float br = f.fogata ? VidaColinas::BrilloFogata(f.indice, th) : VidaColinas::BrilloFarol(f.indice, th);
                glUniform1f(glGetUniformLocation(shader.Program, "llamaFase"), f.fogata ? 0.55f * br : 0.40f * br);
                glm::mat4 mh = glm::scale(glm::translate(glm::mat4(1.0f), f.pos), glm::vec3(f.fogata ? 36.0f : 14.0f));   // cempasuchil.obj mide 0.07 m: 2.5 m / 1.0 m
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mh));
                cempasuchil.Draw(shader);
            }
            glUniform1i(glGetUniformLocation(shader.Program, "llama"), 0);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }

        // Cubos blancos que marcan las luces (tecla L; ayuda de depuracion, no van en la version final)
        if (mostrarLuces)
        {
            lampShader.Use();
            GLint lampModelLoc = glGetUniformLocation(lampShader.Program, "model");
            glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glBindVertexArray(cuboVAO);
            auto cubo = [&](glm::vec3 pos, float lado)
            {
                glm::mat4 m = glm::scale(glm::translate(glm::mat4(1.0f), pos), glm::vec3(lado));
                glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(m));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            };
            for (int i = 0; i < 4; i++) cubo(pointLightPositions[i], 0.04f);
            cubo(spotLightPos, 0.03f);
            for (int i = 0; i < 6; i++) cubo(escalonSpotPos[i], 0.02f);
            glBindVertexArray(0);
        }

        // C-7b: ficha cultural si la camara esta a < 2 m de uno de los 5 objetos (se dibuja al final, sobre la escena)
        FichasDibuja(camera.GetPosition());

        // Modo captura: espera 3 cuadros con la pose fija y guarda el cuadro actual (antes del swap)
        if (indiceCaptura < posesCaptura.size())
        {
            tSilueta = posesCaptura[indiceCaptura].t;
            if (++framesCaptura == 3)
            {
                SOIL_save_screenshot(posesCaptura[indiceCaptura].archivo.c_str(), SOIL_SAVE_TYPE_PNG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                indiceCaptura++;
                framesCaptura = 0;
                if (indiceCaptura == posesCaptura.size()) glfwSetWindowShouldClose(window, GL_TRUE);
            }
        }

        if (modoVideo)
        {
            char nombre[512];
            std::snprintf(nombre, sizeof(nombre), "%s/frame_%05d.png", carpetaVideo.c_str(), cuadroVideo);
            SOIL_save_screenshot(nombre, SOIL_SAVE_TYPE_PNG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            cuadroVideo++;
            if (relojVideo >= rutaVideo.back().t) glfwSetWindowShouldClose(window, GL_TRUE);
        }
        audio.ActualizarZona(camera.GetPosition().x, camera.GetPosition().z, camera.GetFront().x, camera.GetFront().z);  // X-24: volumen por zona
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    // Liberacion explicita de la GPU (E-1a, decision del 19/10: cleanup() a mano, no destructores).
    // Va antes de glfwTerminate(), mientras el contexto de OpenGL sigue vivo.
    fachada.cleanup();
    playa.cleanup();
    mar.cleanup();
    calavera.cleanup();
    flor.cleanup();
    altar3.cleanup();
    vela.cleanup();
    botella.cleanup();
    marco.cleanup();
    soportes.cleanup();
    tumba1.cleanup();
    tumba2.cleanup();
    tumba3.cleanup();
    tumba4.cleanup();
    mesaOfrenda.cleanup(); veladora.cleanup(); calabaza.cleanup(); naranjas.cleanup(); patioTierra.cleanup();   // X-55
    cruz.cleanup();
    copalero.cleanup();
    barda.cleanup();
    roca.cleanup();
    palma.cleanup();    // X-9
    colinas.cleanup();  // X-18
    atuendos.cleanup();  // X-35
    arbolSelva.cleanup(); arcoCemp.cleanup(); racimoCemp.cleanup(); altarMayor.cleanup(); altarPiramide.cleanup(); senderoTierra.cleanup();  // X-36
    personaPieA.cleanup(); personaPieB.cleanup(); personaAgachadaA.cleanup(); personaAgachadaB.cleanup(); petate.cleanup();  // X-19
    arbusto.cleanup();  // X-9
    tablaRemo.cleanup(); luciernaga.cleanup();  // X-21, X-22 (faltaban)
    rocaMar.cleanup(); lancha.cleanup(); redPesca.cleanup(); aveCuerpo.cleanup(); aveAla.cleanup(); tortugaMar.cleanup(); hamaca.cleanup(); fogata.cleanup(); palapa.cleanup(); casaA.cleanup(); casaB.cleanup();  // X-25..X-28
    calles.cleanup();  cimientos.cleanup();  // X-30, X-53
    tambor.cleanup(); alfombraFlores.cleanup(); alfombraCruz.cleanup();  // X-29
    farol.cleanup(); guitarra.cleanup(); cesta.cleanup(); murcielagoCuerpo.cleanup(); murcielagoAla.cleanup();  // X-24
    cempasuchil.cleanup();  // B-3
    panMuerto.cleanup();    // B-4
    papelPicado.cleanup();  // B-7
    papelFucsia.cleanup(); papelMorado.cleanup(); papelNaranja.cleanup(); capillaMaya.cleanup(); mazeta.cleanup(); cuadro.cleanup(); refresco.cleanup(); humo.cleanup();  // X-43, X-47
    llamaModelo.cleanup();  // C-2
    fuegoLlama.cleanup(); mariCuerpo.cleanup(); mariAlaDer.cleanup(); mariAlaIzq.cleanup();  // X-49
    arco.cleanup();         // faltaban en E-1a (A-10, A-11)
    techo.cleanup();
    piso.cleanup();
    for (Model& m : modelosSilueta) m.cleanup();  // D-1
    shader.cleanup();
    lampShader.cleanup();
    skyShader.cleanup();                  // X-5
    glDeleteVertexArrays(1, &vaoCielo);   // X-5
    glDeleteVertexArrays(1, &cuboVAO);
    glDeleteBuffers(1, &cuboVBO);

    musicoViento.cleanup(); musicoTrombon.cleanup(); musicoTuba.cleanup(); musicoPercusion.cleanup(); musicoPlatillos.cleanup();  // X-40
    for (int i = 0; i < Extras::N_INST_BANDA; i++) bandaInst[i].cleanup();
    audio.cleanup(); // D-6
    FichasCleanup();  // C-7b

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls. Un tiron de deltaTime (ventana arrastrada, etc.) no debe dar un salto enorme: tope de 0.1 s
    GLfloat dtMov = glm::min(deltaTime, 0.1f);
    glm::vec3 posAntes = camera.GetPosition();

    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, dtMov);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, dtMov);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, dtMov);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, dtMov);
    }

    // A-9: corrige la posicion con las colisiones AABB y la deja a 1.6 m sobre el suelo
    camera.SetPosition(colisiones.resolver(posAntes, camera.GetPosition()));

    // Reflector de la cruz: posicion (T/G = X, Y/H = Y, U/J = Z) y direccion (1/2 = X, 3/4 = Y, 5/6 = Z)
    if (keys[GLFW_KEY_T]) spotLightPos.x += 1.0f * deltaTime;
    if (keys[GLFW_KEY_G]) spotLightPos.x -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_Y]) spotLightPos.y += 1.0f * deltaTime;
    if (keys[GLFW_KEY_H]) spotLightPos.y -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_U]) spotLightPos.z -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_J]) spotLightPos.z += 1.0f * deltaTime;
    if (keys[GLFW_KEY_1]) spotLightDir.x += 0.5f * deltaTime;
    if (keys[GLFW_KEY_2]) spotLightDir.x -= 0.5f * deltaTime;
    if (keys[GLFW_KEY_3]) spotLightDir.y += 0.5f * deltaTime;
    if (keys[GLFW_KEY_4]) spotLightDir.y -= 0.5f * deltaTime;
    if (keys[GLFW_KEY_5]) spotLightDir.z += 0.5f * deltaTime;
    if (keys[GLFW_KEY_6]) spotLightDir.z -= 0.5f * deltaTime;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    // Barra espaciadora: enciende/apaga la animacion de las 4 luces puntuales (colores de D1)
    if (GLFW_KEY_SPACE == key && GLFW_PRESS == action)
    {
        active = !active;
        if (active)
        {
            Light1 = glm::vec3(0.0f, 1.0f, 0.0f); // verde
            Light2 = glm::vec3(0.0f, 0.2f, 1.0f); // azul
            Light3 = glm::vec3(1.0f, 0.0f, 0.0f); // rojo
            Light4 = glm::vec3(1.0f, 0.1f, 0.8f); // rosa
        }
        else
        {
            Light1 = Light2 = Light3 = Light4 = glm::vec3(0.0f);
        }
    }

    // R: reinicia la caminata de la silueta (D-2)
    if (GLFW_KEY_R == key && GLFW_PRESS == action)
        tSilueta = 0.0f;

    // L: muestra u oculta los cubos que marcan las luces
    if (GLFW_KEY_L == key && GLFW_PRESS == action)
        mostrarLuces = !mostrarLuces;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}