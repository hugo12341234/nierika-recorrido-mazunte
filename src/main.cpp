// Std. Includes
#include <string>
#include <iostream>
#include <cmath>
#include <cstdio>

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
#include "Olas.h"   // C-1: parametros y altura del oleaje
#include "Colisiones.h"
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
// D1 los penso para la capilla a escala 3.5 en (0, 0, 2); hoy la capilla mide 1.9 m y esta en (0, 0, -0.6).
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
    Shader siluetaShader("Shaders/silueta.vs", "Shaders/silueta.frag");

    // Load models
    Model altar((char*)"Models/altar/altar.obj");
    Model fachada((char*)"Models/fachada/fachada.obj");
    Model arco((char*)"Models/arco/arco.obj");    // A-10: dovelas, en coordenadas de la fachada
    Model techo((char*)"Models/techo/techo.obj"); // A-11: techo a dos aguas separado de fachada.obj
    Model piso((char*)"Models/piso/piso.obj");    // A-11: piso interior a y = +0.02
    Model playa((char*)"Models/playa/playa.obj");
    Model mar((char*)"Models/mar/mar.obj");
    // X-3: extras de D1 (Maya), sin tarea propia; escala real ya aplicada en el .obj
    Model calavera((char*)"Models/calavera/calavera.obj");
    Model pan((char*)"Models/pan/pan.obj");
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

    // C-4: 5 flores flotando en la orilla (mar en y = -0.2, orilla en z = +24; valores en ESTADO, decision del 04/11)
    // {centro XZ, radio, sentido, tMov, tPausa, desfase, omegaGiro, faseGiro}
    const FlorFlotante floresFlotantes[5] = {
        { glm::vec2(-4.0f, 28.0f), 0.60f,  1.0f, 5.0f, 2.0f, 0.0f, 0.8f, 0.0f },
        { glm::vec2(-1.5f, 30.0f), 0.45f, -1.0f, 4.5f, 2.5f, 1.7f, 0.6f, 1.3f },
        { glm::vec2( 1.0f, 27.0f), 0.75f,  1.0f, 5.5f, 1.5f, 3.1f, 1.0f, 2.6f },
        { glm::vec2( 3.5f, 29.5f), 0.50f, -1.0f, 5.0f, 3.0f, 4.4f, 0.7f, 3.9f },
        { glm::vec2( 6.0f, 27.5f), 0.65f,  1.0f, 4.0f, 2.0f, 5.8f, 0.9f, 5.2f }
    };
    const glm::vec3 centroFlor(0.0f, 0.0f, -0.075f);         // centro de flor.obj (medido en sus vertices), para girar sobre si misma

    // ==========================================
    // COLISIONES AABB (A-9). Cajas en coordenadas del mundo, medidas de los .obj (fachada girada 180 grados en Y:
    // la apertura queda en z = +1.31). Muros ~0.1 m de espesor; piso = playa.obj (suelo y = 0, no es caja).
    // ==========================================
    colisiones.agregar({ glm::vec3(-1.965f, 0.0f, -1.310f), glm::vec3( 1.965f, 2.62f, -1.217f) });  // muro del fondo
    colisiones.agregar({ glm::vec3(-1.965f, 0.0f, -1.310f), glm::vec3(-1.826f, 2.62f,  1.310f) });  // muro izquierdo
    colisiones.agregar({ glm::vec3( 1.826f, 0.0f, -1.310f), glm::vec3( 1.965f, 2.62f,  1.310f) });  // muro derecho
    colisiones.agregar({ glm::vec3(-1.965f, 0.0f,  1.217f), glm::vec3(-0.621f, 2.62f,  1.310f) });  // muro del frente, a la izquierda de la puerta
    colisiones.agregar({ glm::vec3( 0.562f, 0.0f,  1.217f), glm::vec3( 1.965f, 2.62f,  1.310f) });  // muro del frente, a la derecha de la puerta
    colisiones.agregar({ glm::vec3(-0.650f, 0.0f, -1.050f), glm::vec3( 0.650f, 1.90f, -0.150f) });  // capilla provisional (altar.obj en (0, 0, -0.6))

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

    // VAO propio de la silueta: mismo cubo (VBO), con posicion y normal
    GLuint siluetaVAO;
    glGenVertexArrays(1, &siluetaVAO);
    glBindVertexArray(siluetaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cuboVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Silueta silueta;  // jerarquia de cubos + KeyFrames (src/Silueta.cpp)

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 300.0f);  // far plane a 300 m (A-4)

    // Game loop
    // D-6: audio ambiental en bucle (si faltan los archivos de Audio/, avisa y sigue sin sonido)
    Audio audio;
    audio.init();

    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
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

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // 1. Luz direccional (luna / ambiente)
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.1f, 0.1f, 0.15f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

        // 2. Animacion ciclica de las puntuales: las traseras brillan con sen(2t) > 0 y las delanteras con sen(2t) < 0
        glm::vec3 colors[4] = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
        if (active)
        {
            float ciclo = sin(glfwGetTime() * 2.0f);
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
        playa.Draw(shader);

        // 0b. MAR (100 x 100 m en y = -0.2, vertices ya en coordenadas del mundo: x -50..50, z 22..122; orilla en z = 24; A-7)
        glm::mat4 modelMar(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMar));
        glUniform1f(specLoc, 0.6f);
        // C-1: oleaje por vertex shader (suma de senos, parametros en src/Olas.h)
        {
            float oA[Olas::N], oK[Olas::N], oW[Olas::N], oDir[2 * Olas::N], oFase[Olas::N];
            Olas::ParametrosShader(oA, oK, oW, oDir, oFase);
            glUniform1i(glGetUniformLocation(shader.Program, "oleaje"), 1);
            glUniform1f(glGetUniformLocation(shader.Program, "tiempo"), (float)glfwGetTime());
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaA"), Olas::N, oA);
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaK"), Olas::N, oK);
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaW"), Olas::N, oW);
            glUniform2fv(glGetUniformLocation(shader.Program, "ondaDir"), Olas::N, oDir);
            glUniform1fv(glGetUniformLocation(shader.Program, "ondaFase"), Olas::N, oFase);
        }
        mar.Draw(shader);
        glUniform1i(glGetUniformLocation(shader.Program, "oleaje"), 0);   // el resto de los modelos sin oleaje

        // 1. FACHADA (escala real en el .obj desde A-2: planta exterior de 3.93 x 2.62 m, arco de 2.11 m)
        glm::mat4 modelFachada(1.0f);
        modelFachada = glm::translate(modelFachada, glm::vec3(0.0f, 0.0f, 0.0f));
        modelFachada = glm::rotate(modelFachada, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        float escalaFachada = 1.0f;
        modelFachada = glm::scale(modelFachada, glm::vec3(escalaFachada, escalaFachada, escalaFachada));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFachada));
        glUniform1f(specLoc, 0.1f);
        fachada.Draw(shader);
        arco.Draw(shader);   // A-10: misma matriz que la fachada
        techo.Draw(shader);  // A-11: misma matriz que la fachada (mismo sistema de coord. Blender)

        // 1b. PISO INTERIOR (A-11): plano a y = +0.02 con transformacion identidad
        glm::mat4 modelPiso(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiso));
        glUniform1f(specLoc, 0.05f);  // piso: poco especular (igual que la arena)
        piso.Draw(shader);

        // 2. ALTAR (capilla provisional de 1.9 m, al fondo del cuarto hasta B-9)
        glm::mat4 modelAltar(1.0f);
        modelAltar = glm::translate(modelAltar, glm::vec3(0.0f, 0.0f, -0.6f));
        float escalaAltar = 1.0f;
        modelAltar = glm::scale(modelAltar, glm::vec3(escalaAltar, escalaAltar, escalaAltar));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAltar));
        altar.Draw(shader);

        // 3. EXTRAS DE D1 (X-3): calavera, pan y 4 flores sobre los escalones de la capilla.
        // Posiciones de D1 (hechas para la capilla x3.5 en (0, 0, 2)) pasadas por DeCapillaVieja(); escala 1.0 (ya esta en el .obj)
        auto dibuja = [&](Model& m, glm::vec3 posVieja)
        {
            glm::mat4 mm = glm::translate(glm::mat4(1.0f), DeCapillaVieja(posVieja));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
            m.Draw(shader);
        };
        dibuja(calavera, glm::vec3(0.0f, 2.1f, 1.8f));
        dibuja(pan, glm::vec3(0.0f, 1.35f, 2.6f));
        dibuja(flor, glm::vec3(-0.9f, 2.32f, 1.8f));  // izquierda de la calavera
        dibuja(flor, glm::vec3(0.9f, 2.32f, 1.8f));   // derecha de la calavera
        dibuja(flor, glm::vec3(-1.0f, 1.5f, 2.6f));   // izquierda del pan
        dibuja(flor, glm::vec3(1.0f, 1.5f, 2.6f));    // derecha del pan

        // 3b. MODELOS PROPIOS DE BLENDER (B-1, B-2, B-5, B-6, B-8, B-9, B-11, B-14). Posiciones PROVISIONALES: B-13 las ajusta.
        // Altar de 3 niveles contra el muro derecho del cuarto (interior x +-1.826), girado -90 grados para que su frente (+z local) mire a -x.
        // Cubre x 0.9..1.8; en esta version no se agrega colision ni se retira la capilla provisional (altar.obj).
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
            auto sobreAltar = [&](Model& m, glm::vec3 loc)
            {
                glm::mat4 mm = glm::translate(glm::mat4(1.0f), altarPos);
                mm = glm::rotate(mm, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                mm = glm::translate(mm, loc);
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mm));
                m.Draw(shader);
            };
            colocar(altar3, altarPos, -90.0f);
            sobreAltar(marco,    glm::vec3( 0.00f, 1.20f, -0.30f));  // nivel superior (y = 1.20), al fondo
            sobreAltar(vela,     glm::vec3(-0.35f, 1.20f, -0.25f));
            sobreAltar(vela,     glm::vec3( 0.35f, 1.20f, -0.25f));
            sobreAltar(botella,  glm::vec3(-0.40f, 0.80f,  0.05f));  // nivel medio (y = 0.80)
            sobreAltar(copalero, glm::vec3( 0.40f, 0.80f,  0.05f));
        }
        // Panteon provisional afuera, entre la fachada y la orilla (z = +24); la ruta central x ~ 0 queda libre
        colocar(cruz,     glm::vec3(-2.5f, 0.0f,  3.0f), 0.0f);
        colocar(tumba2,   glm::vec3(-4.0f, 0.0f,  4.5f), 0.0f);
        colocar(tumba3,   glm::vec3( 4.0f, 0.0f,  5.0f), 0.0f);
        colocar(tumba4,   glm::vec3(-4.5f, 0.0f,  9.0f), 0.0f);
        colocar(tumba1,   glm::vec3( 4.5f, 0.0f, 10.0f), 0.0f);
        colocar(soportes, glm::vec3( 0.0f, 0.0f,  6.0f), 0.0f);  // postes del papel picado (B-7 pendiente)

        // C-4: flores flotando (circulo con velocidad variable y pausas + giro propio + altura de la ola; ver AnimFlores.h)
        glUniform1f(specLoc, 0.1f);
        for (int i = 0; i < 5; i++)
        {
            const float tf = (float)glfwGetTime();
            glm::vec2 pf = PosicionXZ(floresFlotantes[i], tf);
            glm::mat4 mf = MatrizFlorFlotante(floresFlotantes[i], tf, Olas::Altura(pf.x, pf.y, tf), centroFlor);
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mf));
            flor.Draw(shader);
        }
        // 4. SILUETA (D-1/D-2): 10 cubos jerarquicos, caminata de la orilla (z = +24) al altar por KeyFrames con slerp y smoothstep.
        // Cubos provisionales con su propio shader; se sustituyen por Models/silueta/ (D-1)
        tSilueta += deltaTime;
        silueta.Actualizar(tSilueta);
        siluetaShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(siluetaShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(siluetaShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(siluetaVAO);
        for (size_t i = 0; i < silueta.Nodos().size(); i++)
        {
            glm::mat4 mc = silueta.MatrizCubo(i);
            glUniformMatrix4fv(glGetUniformLocation(siluetaShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mc));
            glUniform3fv(glGetUniformLocation(siluetaShader.Program, "color"), 1, glm::value_ptr(silueta.Nodos()[i].color));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

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

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    // Liberacion explicita de la GPU (E-1a, decision del 19/10: cleanup() a mano, no destructores).
    // Va antes de glfwTerminate(), mientras el contexto de OpenGL sigue vivo.
    altar.cleanup();
    fachada.cleanup();
    playa.cleanup();
    mar.cleanup();
    calavera.cleanup();
    pan.cleanup();
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
    cruz.cleanup();
    copalero.cleanup();
    siluetaShader.cleanup();  // D-1/D-2
    shader.cleanup();
    lampShader.cleanup();
    glDeleteVertexArrays(1, &siluetaVAO);
    glDeleteVertexArrays(1, &cuboVAO);
    glDeleteBuffers(1, &cuboVBO);

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