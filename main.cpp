#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "sphereWithTexture.h"
#include "spotLight.h"
#include "cube.h"
#include "curve.h"
#include "stb_image.h"
#include "cylinder.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;


//initial values
float sunAngle = 0.0f;
float sunRotSpeed = 0.02f;

float planet1Angle = 0.0f;
float planet1RotSpeed = 0.04f;
float planet1AroundAngle = 0.0f;
float planet1AroundSpeed = 0.02f;
float planet2RotSpeed = 0.02f;
float planet2AroundSpeed = 0.04f;
float ufoRotSpeed = 0.5f;

bool gameMode = false;
float UFOControlX = 0.0f;
float UFOControlSpeed = 0.01f;

bool shoot = false;
bool once = false;
bool game_over = false;
//point
int points = 0;

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight1(
    0.0f, 10.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.005f,   //k_c
    0.005f,  //k_l
    0.0032f //k_q
);

DirectionLight directlight2(
    0.0f, 10.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.005f,   //k_c
    0.005f,  //k_l
    0.0032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(5.0f,  2.5f,  5.0f),
    glm::vec3(5.0f,  2.5f,  -5.0f),
    glm::vec3(-5.0f,  2.5f,  5.0f),
    glm::vec3(-5.0f,  2.5f,  -5.0f),

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.2f, 0.2f, 0.2f,     // ambient (increased from 0.05 to 0.2)
    1.2f, 1.2f, 1.2f,     // diffuse (increased from 0.8 to 1.2)
    1.5f, 1.5f, 1.5f,     // specular (increased from 1.0 to 1.5)
    1.0f,   // k_c (constant attenuation, unchanged)
    0.07f,  // k_l (linear attenuation, slightly reduced for better brightness over distance)
    0.017f, // k_q (quadratic attenuation, reduced for wider brightness area)
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.2f, 0.2f, 0.2f,     // ambient (increased from 0.05 to 0.2)
    1.2f, 1.2f, 1.2f,     // diffuse (increased from 0.8 to 1.2)
    1.5f, 1.5f, 1.5f,     // specular (increased from 1.0 to 1.5)
    1.0f,   // k_c (constant attenuation, unchanged)
    0.07f,  // k_l (linear attenuation, slightly reduced for better brightness over distance)
    0.017f, // k_q (quadratic attenuation, reduced for wider brightness area)
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.2f, 0.2f, 0.2f,     // ambient (increased from 0.05 to 0.2)
    1.2f, 1.2f, 1.2f,     // diffuse (increased from 0.8 to 1.2)
    1.5f, 1.5f, 1.5f,     // specular (increased from 1.0 to 1.5)
    1.0f,   // k_c (constant attenuation, unchanged)
    0.07f,  // k_l (linear attenuation, slightly reduced for better brightness over distance)
    0.017f, // k_q (quadratic attenuation, reduced for wider brightness area)
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.2f, 0.2f, 0.2f,     // ambient (increased from 0.05 to 0.2)
    1.2f, 1.2f, 1.2f,     // diffuse (increased from 0.8 to 1.2)
    1.5f, 1.5f, 1.5f,     // specular (increased from 1.0 to 1.5)
    1.0f,   // k_c (constant attenuation, unchanged)
    0.07f,  // k_l (linear attenuation, slightly reduced for better brightness over distance)
    0.017f, // k_q (quadratic attenuation, reduced for wider brightness area)
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float speed = 0.003;


std::vector<float> hill = {
-0.1700, 2.5650, 5.1000,
-0.3050, 2.4050, 5.1000,
-0.4900, 2.1700, 5.1000,
-0.7150, 2.0850, 5.1000,
-0.8550, 1.7050, 5.1000,
-0.9150, 1.4200, 5.1000,
-1.3400, 1.0350, 5.1000,
-1.5600, 0.6750, 5.1000,
-1.8650, 0.2000, 5.1000,
-1.9450, -0.1300, 5.1000,
-1.9600, -0.1950, 5.1000,
};

std::vector<float> pond = {
-1.4800, 0.9000, 5.1000,
-1.6500, 0.6950, 5.1000,
-1.7550, 0.5950, 5.1000,
-1.8650, 0.4600, 5.1000,
-1.9400, 0.3350, 5.1000,
-1.9650, 0.2250, 5.1000,
-1.9650, 0.1150, 5.1000,
-1.9350, 0.0700, 5.1000,
-1.9000, 0.0650, 5.1000,
};
std::vector<float> water = {

        -0.0050, 1.7650, 5.1000,
    -1.8150, 1.5700, 5.1000,
    0.0100, 1.5750, 5.1000,
};

std::vector<float> special_tree = {
    // Base of the cylinder
   0.2, 0, 0,
   0.2, 1, 0,
   0.1732, 1, 0,
   0.1732, 0, 0,
   0.1732, -1, 0,
   0.2, -1, 0,
   0.2, 0, 0,

   // Top of the cylinder
   0.2, 0, 1,
   0.2, 1, 1,
   0.1732, 1, 1,
   0.1732, 0, 1,
   0.1732, -1, 1,
   0.2, -1, 1,
   0.2, 0, 1




};

std::vector<float> tree_head = {
    -0.0050, 2.5650, 5.1000,
-0.2250, 2.4400, 5.1000,
-0.6300, 2.3150, 5.1000,
-0.9750, 2.1900, 5.1000,
-1.3150, 1.9650, 5.1000,
-1.1650, 1.8850, 5.1000,
-0.9550, 1.8900, 5.1000,
-0.5800, 1.8900, 5.1000,
-0.1550, 1.8700, 5.1000,
-0.0600, 1.8300, 5.1000,
-0.4650, 1.5600, 5.1000,
-0.8200, 1.4200, 5.1000,
-1.2250, 1.2050, 5.1000,
-1.5500, 0.9450, 5.1000,
-1.6750, 0.7250, 5.1000,
-1.7050, 0.5350, 5.1000,
-1.6850, 0.4050, 5.1000,
-1.4500, 0.3000, 5.1000,
-1.2300, 0.3000, 5.1000,
-0.8050, 0.3000, 5.1000,
};

void ponds(Shader& lightingShader, glm::mat4 alTogether, Curve& pond, Curve& pond_water) {

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -.35f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, .5f, .1f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    pond.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -.21f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.169f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    pond_water.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));
}

void make_hill(Shader& lightingShader, glm::mat4 alTogether, Curve& hill, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_hill = glm::mat4(1.0f);
    model_for_hill = glm::translate(model_for_hill, position);
    model_for_hill = glm::rotate(model_for_hill, glm::radians(angle), angle_axis);
    model_for_hill = glm::scale(model_for_hill, glm::vec3(0.2f, 0.4f, 0.2f));
    model_for_hill = alTogether * model_for_hill;
    hill.draw(lightingShader, model_for_hill, glm::vec3(1.0f, 0.0f, 0.0f));
}

void make_tree(Shader& lightingShader, glm::mat4 alTogether, Curve& tree, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_tree = glm::mat4(1.0f);
    model_for_tree = glm::translate(model_for_tree, position);
    model_for_tree = glm::rotate(model_for_tree, glm::radians(angle), angle_axis);
    model_for_tree = glm::scale(model_for_tree, glm::vec3(0.3f, 0.4f, 0.3f));
    model_for_tree = alTogether * model_for_tree;
    tree.draw(lightingShader, model_for_tree, glm::vec3(0.65f, 0.33f, 0.17f)); // brown
}

void make_tree_head(Shader& lightingShader, glm::mat4 alTogether, Curve& treeHead, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_tree_head = glm::mat4(1.0f);
    model_for_tree_head = glm::translate(model_for_tree_head, position);
    model_for_tree_head = glm::rotate(model_for_tree_head, glm::radians(angle), angle_axis);
    model_for_tree_head = glm::scale(model_for_tree_head, glm::vec3(0.15f, 0.2f, 0.15f));
    model_for_tree_head = alTogether * model_for_tree_head;
    treeHead.draw(lightingShader, model_for_tree_head, glm::vec3(0.3f, 0.8f, 0.1f)); // Greenish tree head color
}

void make_ponds(Shader& lightingShaderWithTexture, glm::mat4 alTogether, Curve& Pond, Curve& Pond_water, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_pond = glm::mat4(1.0f);
    model_for_pond = glm::translate(model_for_pond, position);
    model_for_pond = glm::rotate(model_for_pond, glm::radians(angle), angle_axis);
    model_for_pond = glm::scale(model_for_pond, glm::vec3(1.5f, 0.5f, 1.5f));
    model_for_pond = alTogether * model_for_pond;
    ponds(lightingShaderWithTexture, model_for_pond, Pond, Pond_water);
}


void Jupiter(Shader& lightingShaderWithTexture, Shader& lightingShader, glm::mat4 altogether, SphereWithTexture& jupiter, Sphere jupiterWithoutTex, Curve& hill, Curve & tree , Curve & treeHead)
{
    //jupiter
    glm::mat4 modelMatrixForContainer_j = glm::mat4(1.0f);
    modelMatrixForContainer_j = glm::translate(modelMatrixForContainer_j, glm::vec3(0.0f, 0.0f, 0.0f));
    //modelMatrixForContainer_j = glm::rotate(modelMatrixForContainer_j, glm::radians(spin_of_earth), glm::vec3(.0f, 1.0f, 0.0f));
    modelMatrixForContainer_j = altogether * modelMatrixForContainer_j;
    jupiter.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForContainer_j);


    //hill

    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(1.0f, 1.0f, 1.0f), 60.0f, glm::vec3(0.0f, 0.0f, -1.0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(1.0f, 1.0f, -1.0f), -30.0f, glm::vec3(1.0f, .0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(-1.0f, 0.2f, -1.5f), -50.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    // Tree 1 
    make_tree(lightingShaderWithTexture, altogether, tree , glm::vec3(2.0f, 0.5f, 0.5f), 80.0f, glm::vec3(0.0f, 0.0f, -1.0f));

    make_tree_head(lightingShaderWithTexture, altogether, treeHead, glm::vec3(2.0f, 0.5f, 0.5f), 80.0f, glm::vec3(0.0f, 0.0f, -1.0f));



    // tree 2
    
    make_tree(lightingShaderWithTexture, altogether, tree, glm::vec3(-0.1f, 1.8f, -1.1f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    make_tree_head(lightingShaderWithTexture, altogether, treeHead, glm::vec3(-0.1f, 1.8f, -1.1f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));

}

void Jupiter1(Shader& lightingShaderWithTexture, Shader& lightingShader, glm::mat4 altogether, SphereWithTexture& jupiter, Sphere jupiterWithoutTex, Curve& Pond, Curve& Pond_water, Curve& hill)
{
    //jupiter
    glm::mat4 modelMatrixForContainer_j = glm::mat4(1.0f);
    modelMatrixForContainer_j = glm::translate(modelMatrixForContainer_j, glm::vec3(0.0f, 0.0f, 0.0f));
    //modelMatrixForContainer_j = glm::rotate(modelMatrixForContainer_j, glm::radians(spin_of_earth), glm::vec3(.0f, 1.0f, 0.0f));
    modelMatrixForContainer_j = altogether * modelMatrixForContainer_j;
    jupiter.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForContainer_j);

    //pond
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-0.3f, 1.0f, 1.75f), -70.0f, glm::vec3(-1.0f, 1.0f, .0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(1.5f, -1.0f, 1.0f), 120.0f, glm::vec3(1.0f, .0f, -1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(.6f, .2f, 2.0f), -90.0f, glm::vec3(-1.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-1.32f, .57f, -1.57f), -65.0f, glm::vec3(1.0f, .0f, -1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(1.562f, .1f, 1.32f), 120.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-1.62f, -.2f, 1.32f), 100.0f, glm::vec3(1.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(1.25f, -.57f, -1.57f), -100.0f, glm::vec3(1.0f, .0f, 1.0f));

    //hill
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(1.0f, 1.0f, -1.0f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(1.0f, .2f, -1.0f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(1.57f, .57f, .57f), 90.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(-1.57f, .57f, .57f), 90.0f, glm::vec3(.0f, 1.0f, 1.0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(-1.57f, .57f, -.57f), -90.0f, glm::vec3(1.0f, -1.0f, .0f));
}


void drawUFO(Shader& lightingShaderWithTexture, glm::mat4 altogether, SphereWithTexture ufo, SphereWithTexture ufo_base) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
    glm::mat4 modelMatrixForUFO = altogether * translateMatrix * scaleMatrix;
    ufo.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForUFO);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -0.25f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.05f, 0.5f));
    glm::mat4 modelMatrixForUFOBody = altogether * translateMatrix * scaleMatrix;
    ufo_base.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForUFOBody);

}



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sunWithoutTex = Sphere();


    Sphere jupiterWithoutTex = Sphere();

    jupiterWithoutTex.ambient = glm::vec3(1.0, 1.0, 1.0);
    jupiterWithoutTex.diffuse = glm::vec3(1.0, 1.0, 1.0);


    Sphere point = Sphere();
    point.setRadius(0.001f);
    float inc2 = 0.1f;

    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");


    string diffuseMapPath = "sun.jpg";
    string specularMapPath = "sun.jpg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    SphereWithTexture sunWithTex = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap, specMap, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath_v = "venus.jpeg";
    string specularMapPath_v = "venus.jpeg";

    unsigned int diffMap_v = loadTexture(diffuseMapPath_v.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_v = loadTexture(specularMapPath_v.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    SphereWithTexture venus = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_v, specMap_v, 0.0f, 0.0f, 1.0f, 1.0f);



    string diffuseMapPath_j = "Saturn2.jpg";
    string specularMapPath_j = "Saturn2.jpg";

    unsigned int diffMap_j = loadTexture(diffuseMapPath_j.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_j = loadTexture(specularMapPath_j.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    SphereWithTexture jupiter = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_j, specMap_j, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath_m = "mars.jpg";
    string specularMapPath_m = "mars.jpg";

    unsigned int diffMap_m = loadTexture(diffuseMapPath_m.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_m = loadTexture(specularMapPath_m.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    SphereWithTexture mars = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_m, specMap_m, 0.0f, 0.0f, 1.0f, 1.0f);



    string diffuseMapPath_ur = "uranus.jpg";
    string specularMapPath_ur = "uranus.jpg";

    unsigned int diffMap_ur = loadTexture(diffuseMapPath_ur.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_ur = loadTexture(specularMapPath_ur.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    SphereWithTexture uranus = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_ur, specMap_ur, 0.0f, 0.0f, 1.0f, 1.0f);





    string diffuseMapPath_h = "hill.jpg";
    string specularMapPath_h = "hill.jpg";
    unsigned int diffMap_h = loadTexture(diffuseMapPath_h.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_h = loadTexture(specularMapPath_h.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve  Hill = Curve(hill, diffMap_h, specMap_h, 5);





    string diffuseMapPath_s = "space.jpg";
    string specularMapPath_s = "space.jpg";
    unsigned int diffMap_s = loadTexture(diffuseMapPath_s.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_s = loadTexture(specularMapPath_s.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    Cube cube = Cube(diffMap_s, specMap_s, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath_tree = "tree.jpg";
    string specularMapPath_tree = "tree.jpg";
    unsigned int diffMap_tree = loadTexture(diffuseMapPath_tree.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_tree = loadTexture(specularMapPath_tree.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve SpecialTree = Curve(special_tree, diffMap_tree, specMap_tree, 5);


    string diffuseMapPath_treeHead = "tree2.jpg";
    string specularMapPath_treeHead = "tree2.jpg";
    unsigned int diffMap_treeHead = loadTexture(diffuseMapPath_treeHead.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_treeHead = loadTexture(specularMapPath_treeHead.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve TreeHead = Curve(tree_head, diffMap_treeHead, specMap_treeHead, 5);


    //pond
    string diffuse_pond_Path = "mud.jpg";
    string specular_pond_Path = "mud.jpg";
    unsigned int diffuse_pond = loadTexture(diffuse_pond_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specular_pond = loadTexture(specular_pond_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve Pond = Curve(pond, diffuse_pond, specular_pond, 5);

    string diffuse_pond_water = "water.png";
    string specular_pond_water = "water.png";
    unsigned int diffuse_pond_w = loadTexture(diffuse_pond_water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specular_pond_w = loadTexture(specular_pond_water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  Pond_water = Curve(water, diffuse_pond_w, specular_pond_w, 5);


    //---UFO---//
    string diffuseMapPath_U = "silverUFO.jpg";
    string specularMapPath_U = "silverUFO.jpg";

    unsigned int diffMap_u = loadTexture(diffuseMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_u = loadTexture(specularMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    SphereWithTexture ufo = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_u, specMap_u, 0.0f, 0.0f, 1.0f, 1.0f);




    diffuseMapPath_U = "ufo2.jpg";
    specularMapPath_U = "ufo2.jpg";
    diffMap_u = loadTexture(diffuseMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap_u = loadTexture(specularMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SphereWithTexture ufo_base = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_u, specMap_u, 0.0f, 0.0f, 1.0f, 1.0f);


    //asteroid-texture.jpg
    diffuseMapPath_U = "asteroid-texture.jpg";
    specularMapPath_U = "asteroid-texture.jpg";
    diffMap_u = loadTexture(diffuseMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap_u = loadTexture(specularMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SphereWithTexture asteroid = SphereWithTexture(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 4.0f, diffMap_u, specMap_u, 0.0f, 0.0f, 1.0f, 1.0f);


    //gameOver
    //gameOver.jpg
    diffuseMapPath_U = "gameOver.jpg";
    specularMapPath_U = "gameOver.jpg";
    diffMap_u = loadTexture(diffuseMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap_u = loadTexture(specularMapPath_U.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube gameOver = Cube(diffMap_u, specMap_u, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //numbers
    string numPath = "digits/0.png";
    unsigned int diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube zero = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/1.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube one = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/2.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube two = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/3.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube three = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/4.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube four = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/5.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube five = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/6.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube six = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/7.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seven = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/8.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube eight = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/9.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube nine = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube score_block[10] = { zero,one,two,three,four,five,six,seven,eight,nine };








    Sphere bullet = Sphere();
    bullet.ambient = glm::vec3(1.0f, 0.0f, 0.0f);

    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------


    const int minCoord = -150;
    const int maxCoord = 150;
    const int numStars = 500;

    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(minCoord, maxCoord), disY(minCoord/3, maxCoord/3), rangeAstX(0.0f,6.0f) ;
    
    vector<vector<float>>star_pos;
    for (int i = 0;i < numStars;i++)
    {
        vector<float>pos;
        double x = dis(gen);
        double y = disY(gen);
        double z = dis(gen);
        pos.push_back(x);
        pos.push_back(y);
        pos.push_back(z);

        star_pos.push_back(pos);
    }

    //asteroid init
    float ast_z = 11.0f;
    float ast_speed = 0.01f;
    float ast_x = rangeAstX(gen);

    //bullet
    float bullet_z = 0.0f;
    float bullet_speed = 0.03f;
    float bullet_x = 0.0f;

    //hit
    float hit_x = 0.0f;
    float hit_z = 0.0f;
    bool hit = false;

    float dx[] = { 1,-1,0,0,1,1,-1,-1 };
    float dz[] = { 0,0,1,-1,1,-1,1,-1 };
    float broken_posX[] = { 0,0,0,0,0,0,0,0 };
    float broken_posZ[] = { 0,0,0,0,0,0,0,0 };
    float broken_speed = 0.1f;


    

    //camera.Position = glm::vec3(0.0, 0.0, 50.0);
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight1.setUpDirectionLight(lightingShader);
        directlight2.setUpDirectionLight(lightingShader);
        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);



        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);


        
        

        

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);


        if (!gameMode)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(sunAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 modelMatrixForSphere = translateMatrix * rotateYMatrix * scaleMatrix;
            sunWithTex.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForSphere);


            sunAngle += sunRotSpeed;

            if (sunAngle > 360.0) sunAngle = 0.0f;


            //planet -1 

            glm::mat4 translateToPivot, translateFromPivot;
            glm::vec4 translateToPivotOrg, translateFromPivotOrg;
            glm::vec3 pivot;

            pivot = glm::vec3(0.0f, 0.0f, 0.0f);

            translateToPivotOrg = scaleMatrix * glm::vec4(-pivot, 0.0f);
            translateFromPivotOrg = scaleMatrix * glm::vec4(pivot, 0.0f);
            translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
            translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));




            translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 0.0f, -6.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(planet1Angle), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 rotateAroundSun = glm::rotate(identityMatrix, glm::radians(planet1AroundAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 modelMatrixForJupitar = translateFromPivot * rotateAroundSun * translateToPivot * translateMatrix * rotateYMatrix * scaleMatrix;
            Jupiter1(lightingShaderWithTexture, lightingShader, modelMatrixForJupitar, venus, jupiterWithoutTex, Pond, Pond_water, Hill);

            //planet1Angle += planet1RotSpeed;
            //planet1AroundAngle += planet1AroundSpeed;
            planet1Angle += planet1RotSpeed;
            planet1AroundAngle += planet1AroundSpeed;
            if (planet1Angle > 360.0) planet1Angle = 0.0f;
            if (planet1AroundAngle > 360.0) planet1AroundAngle = 0.0f;

            //planet -2 

            translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0f, 0.0f, 8.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(planet1Angle), glm::vec3(0.0f, 1.0f, 0.0f));

            //glm::mat4 rotateAroundSun = glm::rotate(identityMatrix, glm::radians(planet1AroundAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 modelMatrixForJupitar1 = translateFromPivot * rotateAroundSun * translateToPivot * translateMatrix * rotateYMatrix * scaleMatrix;
            Jupiter(lightingShaderWithTexture, lightingShader, modelMatrixForJupitar1, jupiter, jupiterWithoutTex, Hill, SpecialTree, TreeHead);

            planet1Angle += planet2RotSpeed;
            planet1AroundAngle += planet2AroundSpeed;
            if (planet1Angle > 360.0) planet1Angle = 0.0f;
            if (planet1AroundAngle > 360.0) planet1AroundAngle = 0.0f;

            //planet -3 

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-14.0f, 0.0f, 12.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(planet1Angle), glm::vec3(0.0f, 1.0f, 0.0f));

            //glm::mat4 rotateAroundSun = glm::rotate(identityMatrix, glm::radians(planet1AroundAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 modelMatrixForJupitar2 = translateFromPivot * rotateAroundSun * translateToPivot * translateMatrix * rotateYMatrix * scaleMatrix;
            Jupiter1(lightingShaderWithTexture, lightingShader, modelMatrixForJupitar2, mars, jupiterWithoutTex, Pond, Pond_water, Hill);

            planet1Angle += planet1RotSpeed;
            planet1AroundAngle += planet1AroundSpeed;
            if (planet1Angle > 360.0) planet1Angle = 0.0f;
            if (planet1AroundAngle > 360.0) planet1AroundAngle = 0.0f;

            //planet -4

            translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, -16.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(planet1Angle), glm::vec3(0.0f, 1.0f, 0.0f));

            //glm::mat4 rotateAroundSun = glm::rotate(identityMatrix, glm::radians(planet1AroundAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 modelMatrixForJupitar3 = translateFromPivot * rotateAroundSun * translateToPivot * translateMatrix * rotateYMatrix * scaleMatrix;
            Jupiter(lightingShaderWithTexture, lightingShader, modelMatrixForJupitar3, uranus, jupiterWithoutTex, Hill, SpecialTree, TreeHead);

            planet1Angle += planet2RotSpeed;
            planet1AroundAngle += planet2AroundSpeed;
            if (planet1Angle > 360.0) planet1Angle = 0.0f;
            if (planet1AroundAngle > 360.0) planet1AroundAngle = 0.0f;

            //UFO

            translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, 3.0f, -10.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(planet1Angle), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 rotateAroundSunufo = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 modelMatrixForUFO = translateFromPivot * rotateAroundSunufo * translateToPivot * translateMatrix * rotateYMatrix * scaleMatrix;
            drawUFO(lightingShaderWithTexture, modelMatrixForUFO, ufo, ufo_base);
            planet1Angle += ufoRotSpeed;
            if (planet1Angle > 360.0) planet1Angle = 0.0f;



            for (int i = 0;i < numStars;i++) {
                float x = star_pos[i][0];
                float y = star_pos[i][1];
                float z = star_pos[i][2];

                translateMatrix = glm::translate(identityMatrix, glm::vec3(x, y, z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
                glm::mat4 modelMatrixForStars = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelMatrixForStars, 1.0f, 1.0f, 1.0f);
            }


            ast_z = 11.0f;
            UFOControlX = 0.0f;

        }



        if (gameMode)
        {
            


            float dis_destroy = sqrt((3.0f + UFOControlX - ast_x) * (3.0f + UFOControlX - ast_x) + (0.0f - ast_z) * (0.0f - ast_z));

            if (dis_destroy < 1.5f) game_over = true;

            if (game_over)
            {
                ast_z = 11.0f;
                UFOControlX = 0.0f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 4.8f, -1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-9.0f, -1.0f, 6.3f));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-35.6931f), glm::vec3(1.0f, 0.0f, 0.0f));
                glm::mat4 modelForGameOver = translateMatrix * rotateXMatrix * scaleMatrix;
                gameOver.drawCubeWithTexture(lightingShaderWithTexture, modelForGameOver);
            }
            else
            {
                //-----------UFO--------------//
                translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f + UFOControlX, 5.0f, 0.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
                glm::mat4 modelMatrixForUFO = translateMatrix * scaleMatrix;
                drawUFO(lightingShaderWithTexture, modelMatrixForUFO, ufo, ufo_base);

                ast_z -= ast_speed;
                if (ast_z < -1.0f)
                {
                    ast_z = 11.0f;
                    ast_x = rangeAstX(gen);
                }

                translateMatrix = glm::translate(identityMatrix, glm::vec3(ast_x, 5.0f, ast_z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
                glm::mat4 modelMatrixForAsteroid = translateMatrix * scaleMatrix;
                asteroid.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForAsteroid);

                if (shoot)
                {
                    //
                    float dis = sqrt((bullet_x - ast_x) * (bullet_x - ast_x) + (bullet_z - ast_z) * (bullet_z - ast_z));

                    //hit
                    if (dis < 0.5f) {
                        points++;
                        ast_z = 11.0f;
                        ast_x = rangeAstX(gen);

                        hit = true;
                        hit_x = bullet_x;
                        hit_z = bullet_z;


                        shoot = false;
                        bullet_z = 0.0f;
                        for (int i = 0;i < 8;i++)
                        {
                            broken_posX[i] = 0.0f;
                            broken_posZ[i] = 0.0f;
                        }
                    }





                    if (!once)
                    {
                        bullet_x = 3.0f + UFOControlX;
                        once = true;
                    }
                    bullet_z += bullet_speed;
                    if (bullet_z > 11.0f) {
                        shoot = false;
                        bullet_z = 0.0f;
                        hit = false;
                        for (int i = 0;i < 8;i++)
                        {
                            broken_posX[i] = 0.0f;
                            broken_posZ[i] = 0.0f;
                        }
                    }


                    translateMatrix = glm::translate(identityMatrix, glm::vec3(bullet_x, 5.0f, bullet_z));
                    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
                    glm::mat4 modelMatrixForBullet = translateMatrix * scaleMatrix;
                    bullet.drawSphere(lightingShader, modelMatrixForBullet);
                }

                if (hit) {
                    for (int i = 0;i < 8;i++)
                    {
                        broken_posX[i] += dx[i] * broken_speed;
                        broken_posZ[i] += dz[i] * broken_speed;
                        translateMatrix = glm::translate(identityMatrix, glm::vec3(hit_x + broken_posX[i], 5.0f, hit_z + broken_posZ[i]));
                        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
                        glm::mat4 modelMatrixForBrokenAsteroid = translateMatrix * scaleMatrix;
                        asteroid.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForBrokenAsteroid);
                    }
                }
            }



            

            int Arr[3];
            Arr[0] = 0;
            Arr[1] = 0;
            Arr[2] = 0;
            int ff = points;
            int inddd = 2;
            while (ff > 0) {
                Arr[inddd] = ff % 10;
                ff /= 10;
                inddd--;
            }


            float scr_x = 2.5f;
            float scr_y = 9.5f;
            float scr_z = 2.0f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(scr_x, scr_y, scr_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.3f, 0.03f, -0.3f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-54.6931f), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 modelForScore11 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[Arr[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore11);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(scr_x + 0.3f, scr_y, scr_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.3f, 0.03f, -0.3f));
            glm::mat4 modelForScore22 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[Arr[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore22);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(scr_x + 0.6f, scr_y, scr_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.3f, 0.03f, -0.3f));
            glm::mat4 modelForScore33 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[Arr[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore33);


        }
        



        //2.60279 12.2383 -2.36488
        //camera pitch : -54.6931
        /*cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
        cout << "camera pitch: " << camera.Pitch << endl;*/


        






        // also draw the lamp object(s)
        //ourShader.use();
        //ourShader.setMat4("projection", projection);
        //ourShader.setMat4("view", view);
        //float cr, cg, cb;
        //if (pointLightOn)
        //{
        //    cr = 0.8f;
        //    cg = 0.8f;
        //    cb = 0.8f;
        //}
        //else
        //{
        //    cr = 0.0f;
        //    cg = 0.0f;
        //    cb = 0.0f;
        //}
        //// point lights

        //glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 0; i < 4; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //    ourShader.setMat4("model", model);
        //    ourShader.setVec3("color", glm::vec3(cr, cg, cb));
        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //}


        

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!gameMode && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (!gameMode && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (!gameMode && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    }
    if (!gameMode && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (!gameMode && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (!gameMode && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }


    if (!gameMode && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_U, deltaTime);
    }
    if (!gameMode && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_D, deltaTime);
    }

    if (!gameMode && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        gameMode = true;
        camera.Position = glm::vec3(2.60279f, 12.2383f, - 2.36488f);
        camera.Pitch = -54.6931f;
        pointlight1.position = glm::vec3(3.0f, 10.0f, 0.0f);

        camera.ProcessKeyboard(BACKWARD, deltaTime);
        camera.ProcessKeyboard(LEFT, 10*deltaTime);
        //2.60279 12.2383 -2.36488
        //camera pitch : -54.6931
    }


    if (gameMode && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        UFOControlX += UFOControlSpeed;
    }
    if (gameMode && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        UFOControlX -= UFOControlSpeed;
    }
    if (!shoot && gameMode && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        shoot = true;
        once = false;

    }

    if (game_over && gameMode && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        game_over = false;
        points = 0;
    }


    if (gameMode && glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        gameMode = false;

        camera.Position = glm::vec3(0.0f, 1.0f, -6.5f);
        camera.Pitch = 0.0f;
        pointlight1.position = glm::vec3(5.0f, 2.5f, 5.0f);
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight1.turnOff();
            directlight2.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight1.turnOff();
            directlight2.turnOff();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight1.turnAmbientOff();
            directlight2.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight1.turnAmbientOn();
            directlight2.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight1.turnDiffuseOff();
            directlight2.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight1.turnDiffuseOn();
            directlight2.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight1.turnSpecularOff();
            directlight2.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight1.turnSpecularOn();
            directlight2.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}