// Std. Includes
#include <string>
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

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
Camera camera(glm::vec3(0.0f, 2.0f, 6.0f));
bool keys[1024];
GLfloat lastX = 640, lastY = 360;  // centro de la ventana de 1280x720
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Nierika Studio - Recorrido virtual: Dia de Muertos en Mazunte", nullptr, nullptr);

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

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

    // Load models
    Model altar((char*)"Models/altar/altar.obj");
    Model fachada((char*)"Models/fachada/fachada.obj");
    Model playa((char*)"Models/playa/playa.obj");
    Model mar((char*)"Models/mar/mar.obj");
    // X-3: extras de D1 (Maya), sin tarea propia; escala real ya aplicada en el .obj
    Model calavera((char*)"Models/calavera/calavera.obj");
    Model pan((char*)"Models/pan/pan.obj");
    Model flor((char*)"Models/flor/flor.obj");

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

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 300.0f);  // far plane a 300 m (A-4)

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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
        mar.Draw(shader);

        // 1. FACHADA (escala real en el .obj desde A-2: planta exterior de 3.93 x 2.62 m, arco de 2.11 m)
        glm::mat4 modelFachada(1.0f);
        modelFachada = glm::translate(modelFachada, glm::vec3(0.0f, 0.0f, 0.0f));
        modelFachada = glm::rotate(modelFachada, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        float escalaFachada = 1.0f;
        modelFachada = glm::scale(modelFachada, glm::vec3(escalaFachada, escalaFachada, escalaFachada));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFachada));
        glUniform1f(specLoc, 0.1f);
        fachada.Draw(shader);

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

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    // Libera lo que se creo aqui (el resto de la GPU se libera en E-1 con cleanup())
    glDeleteVertexArrays(1, &cuboVAO);
    glDeleteBuffers(1, &cuboVBO);

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

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