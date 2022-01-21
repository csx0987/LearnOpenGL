// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <cmath>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "renderer.h"
#include "texture.h"

// matrix
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lighting
struct Light
{
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

struct BaseLight
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirLight : BaseLight // 平行光，暂只有一个
{
    glm::vec3 direction;
};

struct PointLight : BaseLight
{
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight : BaseLight
{
    glm::vec3 position;
    glm::vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

// box
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPosition[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};

glm::vec3 pointLightAmbient[] = {
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.05f, 0.05f, 0.05f),
};

glm::vec3 pointLightDiffuse[] = {
    glm::vec3(0.8f, 0.8f, 0.8f),
    glm::vec3(0.8f, 0.8f, 0.8f),
    glm::vec3(0.8f, 0.8f, 0.8f),
};

glm::vec3 pointLightSpecular[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
};

float pointLightConstant[] = {
    1.0f, 1.0f, 1.0f};

float pointLightLinear[] = {
    0.09f, 0.09f, 0.09f};

float pointLightQuadratic[] = {
    0.032f, 0.032f, 0.032f};

#if defined(__APPLE__)
const char *cubeVsPath = "../shaders/cube.vs";
const char *cubeFsPath = "../shaders/cube.fs";
const char *lightCubeVsPath = "../shaders/light_cube.vs";
const char *lightCubeFsPath = "../shaders/light_cube.fs";
const char *modelVsPath = "../shaders/model.vs";
const char *modelFsPath = "../shaders/model.fs";
const char *diffuseMapPath = "../resources/textures/container2.png";
const char *specularMapPath = "../resources/textures/container2_specular.png";
const char *emissiveMapPath = "../resources/textures/matrix.jpeg";
const char *objPath = "../resources/models/nanosuit/nanosuit.obj";
// const char *imgContainerPath = "../resources/textures/container.jpg";
// const char *imgAwesomefacePath = "../resources/textures/awesomeface.png";
#else
const char *cubeVsPath = "../../../shaders/cube.vs";
const char *cubeFsPath = "../../../shaders/cube.fs";
const char *lightCubeVsPath = "../../../shaders/light_cube.vs";
const char *lightCubeFsPath = "../../../shaders/light_cube.fs";
const char *modelVsPath = "../../../shaders/model.vs";
const char *modelFsPath = "../../../shaders/model.fs";
const char *diffuseMapPath = "../../../resources/textures/container2.png";
const char *specularMapPath = "../../../resources/textures/container2_specular.png";
const char *emissiveMapPath = "../../../resources/textures/matrix.jpeg";
const char *objPath = "../../../resources/models/nanosuit/nanosuit.obj";
// const char *imgContainerPath = "../../../resources/textures/container.jpg";
// const char *imgAwesomefacePath = "../../../resources/textures/awesomeface.png";
#endif

int main(int, char **)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable vsync 垂直同步

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to init GLAD\n");
        return -1;
    }

    // 编译shader
    // unsigned int shaderProgram;
    Shader cubeShader(cubeVsPath, cubeFsPath);
    Shader lightCubeShader(lightCubeVsPath, lightCubeFsPath);
    Shader modelShader(modelVsPath, modelFsPath);

    // VBO VAO EBO
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5,

        6, 7, 8,
        9, 10, 11,

        12, 13, 14,
        15, 16, 17,

        18, 19, 20,
        21, 22, 23,

        24, 25, 26,
        27, 28, 29,

        30, 31, 32,
        33, 34, 35};

    VertexArray cubeVertexArray;
    VertexBufferLayout cubeVertexBufferLayout;

    cubeVertexBufferLayout.Push(GL_FLOAT, 3, false); // 顶点位置
    cubeVertexBufferLayout.Push(GL_FLOAT, 3, true);  // 法线
    cubeVertexBufferLayout.Push(GL_FLOAT, 2, false); // uv

    VertexBuffer cubeVertexBuffer(vertices, sizeof(vertices));
    IndexBuffer cubeIndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    cubeVertexArray.AddBuffer(cubeVertexBuffer, cubeVertexBufferLayout);
    cubeIndexBuffer.Bind();

    VertexArray lightVertexArray;
    VertexBufferLayout lightVertexBufferLayout;
    lightVertexBufferLayout.Push(GL_FLOAT, 3, false); // 顶点位置
    lightVertexBufferLayout.Push(GL_FLOAT, 3, false);
    lightVertexBufferLayout.Push(GL_FLOAT, 2, false);

    lightVertexArray.AddBuffer(cubeVertexBuffer, cubeVertexBufferLayout);
    cubeIndexBuffer.Bind();

    glBindVertexArray(0);

    // 载入贴图
    Texture diffuseMap = Texture(diffuseMapPath);
    Texture specularMap = Texture(specularMapPath);
    Texture emissiveMap = Texture(emissiveMapPath);

    // 载入模型
    Model ourModel(objPath);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = false;
    glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImVec4 specularFact = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float shininessFact = 36.0f;

    Light light;
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    light.position = glm::vec3(1.2f, 1.0f, 2.0f);
    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.cutOff = glm::cos(glm::radians(12.5f));
    light.outerCutOff = glm::cos(glm::radians(17.5f));

    DirLight dirLight;
    dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    dirLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    dirLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

    PointLight pointLights[3];
    for (unsigned int i = 0; i < 3; i++)
    {
        pointLights[i].position = pointLightPosition[i];
        pointLights[i].ambient = pointLightAmbient[i];
        pointLights[i].diffuse = pointLightDiffuse[i];
        pointLights[i].specular = pointLightSpecular[i];
        pointLights[i].constant = pointLightConstant[i];
        pointLights[i].linear = pointLightLinear[i];
        pointLights[i].quadratic = pointLightQuadratic[i];
    }

    SpotLight spotLight;
    spotLight.position = camera.Position;
    spotLight.direction = camera.Front;
    spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    spotLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.constant = 1.0f;
    spotLight.linear = 0.09f;
    spotLight.quadratic = 0.032f;
    spotLight.cutOff = glm::cos(glm::radians(12.5f));
    spotLight.outerCutOff = glm::cos(glm::radians(15.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BACK);

    // OpenGL 基本信息
    const GLubyte *vendorName = glGetString(GL_VENDOR);
    const GLubyte *glVersion = glGetString(GL_VERSION);
    const GLubyte *rendererName = glGetString(GL_RENDERER);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("VendorName: %s\n", vendorName);
    printf("Renderer Device: %s\n", rendererName);
    printf("GLSL Version: %s\n", glslVersion);
    printf("OpenGL Version: %s\n", glVersion);

    // 初始化渲染器
    Renderer renderer;
    renderer.SetBgColor(clear_color);
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 渲染代码
        renderer.SetBgColor(clear_color);
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state

        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // Rendering
        cubeShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

        cubeShader.setVec3("viewPos", camera.Position);

        // material
        cubeShader.setInt("material.diffuse", 0);
        cubeShader.setInt("material.specular", 1);
        cubeShader.setInt("material.emissive", 2);
        cubeShader.setFloat("material.shininess", shininessFact);

        spotLight.position = camera.Position;
        spotLight.direction = camera.Front;

        // light
        // Direction Light
        cubeShader.setVec3("dirLight.direction", dirLight.direction);
        cubeShader.setVec3("dirLight.ambient", dirLight.ambient);
        cubeShader.setVec3("dirLight.diffuse", dirLight.diffuse);
        cubeShader.setVec3("dirLight.specular", dirLight.specular);

        // Point Light
        const char *attr1 = "position";
        const char *attr2 = "ambient";
        const char *attr3 = "diffuse";
        const char *attr4 = "specular";
        const char *attr5 = "constant";
        const char *attr6 = "linear";
        const char *attr7 = "quadratic";
        char prefix[100]; // temp;
        for (unsigned int i = 0; i < 3; i++)
        {
            sprintf(prefix, "%s%d%s", "pointLights[", i, "].");
            char temp[100];

            sprintf(temp, "%s%s", prefix, attr1);
            cubeShader.setVec3(temp, pointLights[i].position);
            sprintf(temp, "%s%s", prefix, attr2);
            cubeShader.setVec3(temp, pointLights[i].ambient);
            sprintf(temp, "%s%s", prefix, attr3);
            cubeShader.setVec3(temp, pointLights[i].diffuse);
            sprintf(temp, "%s%s", prefix, attr4);
            cubeShader.setVec3(temp, pointLights[i].specular);
            sprintf(temp, "%s%s", prefix, attr5);
            cubeShader.setFloat(temp, pointLights[i].constant);
            sprintf(temp, "%s%s", prefix, attr6);
            cubeShader.setFloat(temp, pointLights[i].linear);
            sprintf(temp, "%s%s", prefix, attr7);
            cubeShader.setFloat(temp, pointLights[i].quadratic);
        }

        // Spot Light
        cubeShader.setVec3("spotLight.position", spotLight.position);
        cubeShader.setVec3("spotLight.direction", spotLight.direction);
        cubeShader.setVec3("spotLight.ambient", spotLight.ambient);
        cubeShader.setVec3("spotLight.diffuse", spotLight.diffuse);
        cubeShader.setVec3("spotLight.specular", spotLight.specular);
        cubeShader.setFloat("spotLight.constant", spotLight.constant);
        cubeShader.setFloat("spotLight.linear", spotLight.linear);
        cubeShader.setFloat("spotLight.quadratic", spotLight.quadratic);
        cubeShader.setFloat("spotLight.cutOff", spotLight.cutOff);
        cubeShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);

        // texture
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, diffuseMap);
        diffuseMap.Bind(0);

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, specularMap);
        specularMap.Bind(1);

        // glActiveTexture(GL_TEXTURE2);
        // glBindTexture(GL_TEXTURE_2D, emissiveMap);
        emissiveMap.Bind(2);

        // cubeVertexArray.Bind();

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);
            glm::mat4 invModel = glm::inverse(model);
            cubeShader.setMat4("invModel", invModel);

            // glDrawArrays(GL_TRIANGLES, 0, 36);
            // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            renderer.Draw(cubeVertexArray, cubeIndexBuffer, cubeShader);
        }

        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // lightVertexArray.Bind();
        for (unsigned int i = 0; i < 3; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLights[i].position);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.setMat4("model", model);
            // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            renderer.Draw(lightVertexArray, cubeIndexBuffer, lightCubeShader);
        }

        // 渲染模型
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        processInput(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            glfwSetScrollCallback(window, NULL);
            firstMouse = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetScrollCallback(window, scroll_callback);
            firstMouse = true;
        }
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
