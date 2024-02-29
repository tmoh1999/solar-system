#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"


#include <vector>
#include <math.h>

#include <iostream>
#include "Shader.h"


#include "DirectionalLight.h";
#include "Material.h"
#include "PointLight.h"
#include "Mesh.h"
#include "Texture.h"
#include "Satellite.h"




using namespace std;
using namespace glm;


// Shaders

vector<Shader> shaderList;
vector<Mesh*> meshList;
Shader sunShader;
Shader directionalShadowShader;
Shader omniShadowShader;


static const char* vShader = "./SimpleVertexShader.vertexshader";

static const char* fShader = "./SimpleFragmentShader.fragmentshader";

//Lights
DirectionalLight *mainLight;
PointLight *pointLights[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;

Material shinyMaterial;
Material dullMaterial;

Texture brickTexture;
Texture dirtTexture;

GLfloat AmbientItensity=3.5f;
GLfloat DiffuseItensity=7.0f;
bool isAmbient = false;
bool isDiffuse = true;




//planets
Satellite earth;
Satellite sun;
Satellite jupiter;
Satellite neptune;
Satellite mars;
Satellite mercury;
Satellite saturn;
Satellite uranus;
Satellite venus;
Satellite sat;
Satellite roci;
Satellite moon;
Satellite gog;

//Unité Scale Earth
float AuEearthDist = 20.0f;
float AuEearthDiam = 5.0f;
float speed = 0.0010f;
float sun_rad=100.0f;

//size Vertexs for orbits
const int nbr = 100;

// View uniforms and matrixs
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,  uniformOmniLightPos = 0, uniformFarPlane = 0;
mat4 View;
mat4 viewMatrix;
mat4 Projection;

//skybox variables
Shader* skyShader;
GLuint textureId;
vector<string> skyboxFaces;
int width, height, bitDepth;
GLuint  VAOSkyBox, VBOSkyBox, IBOSkyBox,
numOfVerticesSkyBox, numOfIndicesSkyBox;

// Mesh Setup
unsigned int skyboxIndices[] = {
    // front
    0, 1, 2,
    2, 1, 3,
    // right
    2, 3, 5,
    5, 3, 7,
    // back
    5, 7, 4,
    4, 7, 6,
    // left
    4, 6, 0,
    0, 6, 1,
    // top
    4, 0, 5,
    5, 0, 2,
    // bottom
    1, 6, 3,
    3, 6, 7
};

struct STRVertex skyboxVertices[] = {
    {vec3(-1.0f, 1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    {vec3(1.0f, 1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    {vec3(1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},

    {vec3(-1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    {vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
    {vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)}

};



// Start Camera


// keyboard vars
bool keys[1024];

// Mouse vars

double lastX;
double lastY;

// diffrence between current position of mouse and last
double xChange;
double yChange;

// to avoid violently turns of camera
bool mouseFirstMove = true;

// Camera vars

vec3 position = vec3(-234.414307, 117.888565, -98.686676);
vec3 front = vec3(0.0f, 0.0f, -1.0f);
vec3 r;
vec3 up;
vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

// neck turn
double ya = 25.1122f; // turn head left and right
double pit = -13.5111f; // turn head up and down

GLfloat mouvementSpeed = 5.0f * 10; // for key
GLfloat turnSpeed = 0.1f; // for mouse

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void keyboardControl() {

    GLfloat velocity = mouvementSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }

    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        position -= r * velocity;

    }

    if (keys[GLFW_KEY_D])
    {
        position += r * velocity;
    }
    if (keys[GLFW_KEY_KP_1])
    {
        isAmbient = true;
        isDiffuse = false;
        cout << "*** Ambient Itensity Modification is Actif." << endl;
    }
    if (keys[GLFW_KEY_KP_2])
    {
        isAmbient = false;
        isDiffuse = true;
        cout << "*** Diffuse Itensity Modification is Actif." << endl;
    }
    if (keys[GLFW_KEY_KP_ADD])
    {

        if (isAmbient) {
            AmbientItensity += 0.2;
            if (AmbientItensity >= 40.0)
                AmbientItensity = 40.0;
            cout << "      New Ambient Itensity : " << AmbientItensity << endl;
        }
        if (isDiffuse) {
            DiffuseItensity += 0.2;
            if (DiffuseItensity >= 40.0)
                DiffuseItensity = 40.0;
            cout << "      New Diffuse Itensity : " << DiffuseItensity << endl;
        }

        pointLights[0]->SetAmbientItensity(AmbientItensity);
        pointLights[0]->SetDiffuseItensity(DiffuseItensity);
    }
    if (keys[GLFW_KEY_KP_SUBTRACT])
    {
        if (isAmbient) {
            AmbientItensity -= 0.2;
            if (AmbientItensity < 0.0)
                AmbientItensity = 0.0;
            cout << "      New Ambient Itensity : " << AmbientItensity << endl;
        }
        if (isDiffuse) {
            DiffuseItensity -= 0.2;
            if (DiffuseItensity < 0.0)
                DiffuseItensity = 0.0;
            cout << "      New Diffuse Itensity : " << DiffuseItensity << endl;
        }
        pointLights[0]->SetAmbientItensity(AmbientItensity);
        pointLights[0]->SetDiffuseItensity(DiffuseItensity);
    }

}

void mouseControl() {

    xChange *= turnSpeed;
    yChange *= turnSpeed;

    ya += xChange;
    pit += yChange;

    if (pit > 44.0f)
    {
        pit = 44.0f;
    }

    if (pit < -44.0f)
    {
        pit = -44.0f;
    }

}

// to change angles when moving
void updateCamera() {
    front.x = cos(radians(ya)) * cos(radians(pit));
    front.y = sin(radians(pit));
    front.z = sin(radians(ya)) * cos(radians(pit));
    front = normalize(front);

    r = normalize(cross(front, worldUp));
    up = normalize(cross(r, front));
}

// key callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
            //printf("Pressed %d\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
            //printf("Released %d\n", key);
        }
    }
}

// mouse callback
static void cursor_position(GLFWwindow* window, double xPos, double yPos)
{
    if (mouseFirstMove) {
        lastX = xPos;
        lastY = yPos;
        mouseFirstMove = false;
    }

    xChange = xPos - lastX;
    yChange = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

}







void Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
    
    sunShader = Shader();
    sunShader.CreateFromFiles("Sun.vert", "Sun.frag");
    
    skyShader = new Shader();
    skyShader->CreateFromFiles("skybox.vert", "skybox.frag");

    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("Directional_shadow_map.vert", "Directional_shadow_map.frag");
    omniShadowShader.CreateFromFiles("omni_directional_shadow_map.vert", "omni_directional_shadow_map.geom", "omni_directional_shadow_map.frag");
}



void CreateMesh(GLuint* VAO, GLuint* VBO, GLuint* IBO, STRVertex* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{


    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    glGenBuffers(1, IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
    glEnableVertexAttribArray(1);


    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, normal));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderMesh(GLuint* VAO, GLuint* IBO, GLuint indexCount)
{
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



void LoadModels() {
    //planets
    sun = Satellite(0, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, sun_rad,
        0.0f, 0.f, 0.f, 0.f, 0.f);
    sun.LoadModel("Models/sun.obj");

    earth = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 1.2, AuEearthDiam,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed);
    earth.LoadModel("Models/Globe.obj");



    jupiter = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 5.2f, AuEearthDiam * 11.2f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 0.4);
    jupiter.LoadModel("Models/jupiter.obj");

    mars = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 1.8f, AuEearthDiam * 0.53f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 0.7);
    mars.LoadModel("Models/mars.obj");

    mercury = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 0.38f, AuEearthDiam * 0.38f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 5);
    mercury.LoadModel("Models/mercury.obj");

    neptune = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 30.1f, AuEearthDiam * 3.9f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 0.005);
    neptune.LoadModel("Models/neptune.obj");


    saturn = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 9.5f, AuEearthDiam * 9.5f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 0.2);
    saturn.LoadModel("Models/saturn.obj");

    uranus = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 19.2f, AuEearthDiam * 4.0f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 0.15);
    uranus.LoadModel("Models/uranus.obj");

    venus = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 0.70f, AuEearthDiam * 0.95f,
        sun.getDiametre(), 45.0f, 45.0f, 0.001f, speed * 2);
    venus.LoadModel("Models/venus.obj");


    moon = Satellite(2, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDiam * 0.3, AuEearthDiam * 0.3,
        earth.getDiametre(), 45.0f, 60.0f, 0.001f, speed * 2);
    moon.LoadModel("models/Moon.obj");




    sat = Satellite(2, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDiam * 0.1, AuEearthDiam * 0.2,
        earth.getDiametre(), 45.0f, 20.0f, 0.001f, 0.01f);
    sat.LoadModel("models/satellite.obj");


    gog = Satellite(1, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDist * 3.f, AuEearthDiam * 1.f,
        sun.getDiametre(), 0.0f, 45.0f, 0.001f, speed);
    gog.LoadModel("models/gog.obj");

    roci = Satellite(2, 20.f, glm::vec3(0.0f, 0.0f, 0.0f), AuEearthDiam * 0.6, AuEearthDiam * 0.5f,
        jupiter.getDiametre(), 45.0f, -45.0f, 0.001f, speed);
    roci.LoadModel("Models/roci.obj");

    // Orbits
    earth.InitOrbit(nbr, earth.getDistance(), 0.0f);
    jupiter.InitOrbit(nbr, jupiter.getDistance(), 0.0f);
    mars.InitOrbit(nbr, mars.getDistance(), 0.0f);
    neptune.InitOrbit(nbr, neptune.getDistance(), 0.0f);
    mercury.InitOrbit(nbr, mercury.getDistance(), 0.0f);
    saturn.InitOrbit(nbr, saturn.getDistance(), 0.0f);
    uranus.InitOrbit(nbr, uranus.getDistance(), 0.0f);
    venus.InitOrbit(nbr, venus.getDistance(), 0.0f);
    sat.InitOrbit(nbr, sat.getDistance(), 45.0f);
    moon.InitOrbit(nbr, moon.getDistance(), 0.0f);
    roci.InitOrbit(nbr, roci.getDistance(), -45.0f);
    gog.InitOrbit(nbr, gog.getDistance(), 0.0f);

}
void RenderSun() {
   
    uniformModel = sunShader.GetModelLocation();
    uniformProjection = sunShader.GetProjectionLocation();
    uniformView = sunShader.GetViewLocation();
    sunShader.UseShader();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(View));
    mat4 Model;
    Model = mat4(1.0f);
    Model = sun.SatelliteMouvement();
    sun.setUniforms(uniformModel, &Model[0][0]);
    sunShader.setTexture(5);
    for (int i = 0; i < sun.getTexture().size(); i++) {
        sun.getTexture().at(i)->SetTextureUnit(5);
    }

    sunShader.Validate();
    sun.RenderModel();
    
}
void RenderOrbits() {
    mat4 Model;
    Model = mat4(1.0f);
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &Model[0][0]);

    earth.RenderOrbit();
    jupiter.RenderOrbit();
    mercury.RenderOrbit();
    venus.RenderOrbit();
    mars.RenderOrbit();
    uranus.RenderOrbit();
    saturn.RenderOrbit();
    neptune.RenderOrbit();
    gog.RenderOrbit();
    
    Model = mat4(1.0f);
    Model = glm::translate(Model, earth.getPosition());
    Model = glm::rotate(Model, glm::radians(sat.getOrbitDegree()), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &Model[0][0]);
    sat.RenderOrbit();

    Model = mat4(1.0f);
    Model = glm::translate(Model, earth.getPosition());
    Model = glm::rotate(Model, glm::radians(moon.getOrbitDegree()), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &Model[0][0]);
    moon.RenderOrbit();

    Model = mat4(1.0f);
    Model = glm::translate(Model, jupiter.getPosition());
    Model = glm::rotate(Model, glm::radians(roci.getOrbitDegree()), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &Model[0][0]);
    roci.RenderOrbit();


}
void RenderModels() {
    mat4 Model;




    Model = earth.SatelliteMouvement();
    earth.setUniforms(uniformModel, &Model[0][0]);  
    earth.RenderModel();

    sat.setPlanetPosition(earth.getPosition());
    Model = sat.SatelliteMouvement();
    sat.setUniforms(uniformModel, &Model[0][0]);
    sat.RenderModel();




    moon.setPlanetPosition(earth.getPosition());
    Model = moon.SatelliteMouvement();
    moon.setUniforms(uniformModel, &Model[0][0]);
    moon.RenderModel();


    Model = mercury.SatelliteMouvement();
    mercury.setUniforms(uniformModel, &Model[0][0]);
    mercury.RenderModel();

    Model = venus.SatelliteMouvement();
    venus.setUniforms(uniformModel, &Model[0][0]);
    venus.RenderModel();


    Model = mars.SatelliteMouvement();
    mars.setUniforms(uniformModel, &Model[0][0]);
    mars.RenderModel();




    Model = jupiter.SatelliteMouvement();
    jupiter.setUniforms(uniformModel, &Model[0][0]);
    jupiter.RenderModel();
    


    roci.setPlanetPosition(jupiter.getPosition());
    Model = roci.SatelliteMouvement();
    roci.setUniforms(uniformModel, &Model[0][0]);
    roci.RenderModel();

    Model = saturn.SatelliteMouvement();
    saturn.setUniforms(uniformModel, &Model[0][0]);
   
    saturn.RenderModel();


    Model = uranus.SatelliteMouvement();
    uranus.setUniforms(uniformModel, &Model[0][0]);
    uranus.RenderModel();

    Model = neptune.SatelliteMouvement();
    neptune.setUniforms(uniformModel, &Model[0][0]);
    neptune.RenderModel();

    Model = gog.SatelliteMouvement();
    gog.setUniforms(uniformModel, &Model[0][0]);
    gog.RenderModel();


}


void DirectionelShadowMapPass(DirectionalLight* light) {

    directionalShadowShader.UseShader();
    glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());
    light->getShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    uniformModel = directionalShadowShader.GetModelLocation();
    mat4 tmp = light->calculateLightTransform();
     directionalShadowShader.setDirectionalLightTransform(&tmp);
     directionalShadowShader.Validate();
     RenderModels();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void OmniShadowMapPass(PointLight* light)
{
    omniShadowShader.UseShader();
    glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());

    
    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

    light->getShadowMap()->Write();

    glClear(GL_DEPTH_BUFFER_BIT);

    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());

    omniShadowShader.Validate();
    RenderModels();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int InitBackground() {




    skyboxFaces.push_back("Textures/Skybox/starfield_rt.tga");
    skyboxFaces.push_back("Textures/Skybox/starfield_lf.tga");
    skyboxFaces.push_back("Textures/Skybox/starfield_up.tga");
    skyboxFaces.push_back("Textures/Skybox/starfield_dn.tga");
    skyboxFaces.push_back("Textures/Skybox/starfield_bk.tga");
    skyboxFaces.push_back("Textures/Skybox/starfield_ft.tga");






    textureId = 0;

    // Texture Setup
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    width = 0; height = 0; bitDepth = 0;

    for (size_t i = 0; i < 6; i++)
    {
        unsigned char* texData = stbi_load(skyboxFaces[i].c_str(), &width, &height, &bitDepth, 0);
        if (!texData)
        {
            printf("Failed to find: %s\n", skyboxFaces[i].c_str());
            return 0;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        stbi_image_free(texData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    VAOSkyBox = 0, VBOSkyBox = 0, IBOSkyBox = 0,
        numOfVerticesSkyBox = 8, numOfIndicesSkyBox = 36;
    CreateMesh(&VAOSkyBox, &VBOSkyBox, &IBOSkyBox, skyboxVertices,
        skyboxIndices, numOfVerticesSkyBox, numOfIndicesSkyBox);

    return 1;

}
void RenderBackground() {
    viewMatrix = mat4(mat3(View));
    glDepthMask(GL_FALSE);
    
    uniformModel = skyShader->GetModelLocation();
    uniformProjection = skyShader->GetProjectionLocation();
    uniformView = skyShader->GetViewLocation();


    skyShader->UseShader();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, &Projection[0][0]);
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, &viewMatrix[0][0]);
    
    

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    skyShader->Validate();

    RenderMesh(&VAOSkyBox, &IBOSkyBox, numOfIndicesSkyBox);

    glDepthMask(GL_TRUE);
}


void RenderScene() {




    glViewport(0, 0, 1366, 768);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    RenderBackground();

    
   

    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();


    uniformEyePosition = shaderList[0].GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();


    shaderList[0].UseShader();

    shaderList[0].SetDirectionalLight(mainLight);
     shaderList[0].SetPointLights(*pointLights, pointLightCount,4,0);

    mat4 tmp = mainLight->calculateLightTransform();
    shaderList[0].setDirectionalLightTransform(&tmp);

    mainLight->getShadowMap()->Read(GL_TEXTURE3);
     shaderList[0].setTexture(2);
     shaderList[0].setDirectionalShadowMap(3);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(View));
    glUniform3f(uniformEyePosition, position.x, position.y, position.z);


    
    shaderList[0].Validate();
   RenderModels();
   RenderSun();
   RenderOrbits();
}




void InitLights() {
    delete mainLight;
    
    delete pointLights[0];
    pointLights[0] = nullptr;
    mainLight = nullptr;
    pointLightCount = 0;
    mainLight = new DirectionalLight(2048, 2048,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, -15.0f, -10.0f);

    pointLights[0] = new PointLight(1024, 1024,
        0.1f, 1500.0f,
        1.0f, 1.0f, 1.0f,
        AmbientItensity, DiffuseItensity,
        0.0f, 0.0f, 0.0f,
        0.03f, 0.001f, 0.001f);
    pointLightCount++;
}

int main(int argc, char* argv[]) {
   
    if (!glfwInit()) {
        printf("Could not initialize glfw.\n");
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    GLFWwindow* window;
    window = glfwCreateWindow(1000, 500, "Projet OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);




    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, Resize);

    // init keys array
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    // handle key callback
    glfwSetKeyCallback(window, key_callback);

    // handle mouse callback
    glfwSetCursorPosCallback(window, cursor_position);

    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    CreateShaders();


    Projection = perspective(radians(45.0f), 16.0f / 9.0f, 0.1f, 1500.0f);


   InitLights();


    InitBackground();

    

    LoadModels();

 

 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window))
    {


         View = lookAt(position, position + front, up);
       
         double now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();


        mouseControl();
        updateCamera();
        keyboardControl();


//        DirectionelShadowMapPass(&mainLight);
        for (size_t i = 0; i < pointLightCount; i++)
        {
            OmniShadowMapPass(pointLights[i]);
        }
        RenderScene();
        

        
        
        glUseProgram(0);


        glfwSwapBuffers(window);      
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}