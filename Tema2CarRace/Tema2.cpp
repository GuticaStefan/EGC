#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/Tema2/Tema2transform3D.h"
#include "lab_m1/Tema2/object3D.h"

using namespace std;
using namespace m1;

vector<glm::vec3> treePos, routeEnemy1, routeEnemy2;
float xEnemy1, zEnemy1, pantaEnemy1;
float xEnemy2, zEnemy2, pantaEnemy2;

int index1Enemy1, index2Enemy1, index1Enemy2, index2Enemy2;

Mesh* street;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

bool Tema2::SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b) {
    glm::vec3 cp1 = glm::cross(b - a, p1 - a);
    glm::vec3 cp2 = glm::cross(b - a, p2 - a);
    if (glm::dot(cp1, cp2) >= 0)return true;
    return false;
}

bool Tema2::PointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b))return true;
    return false;
}

bool Tema2::checkDistance(glm::vec3 p1, glm::vec3 p2, glm::vec3 enemyCenter) {
    float distanceToEnemy = sqrt(pow(p1.x - enemyCenter.x, 2)  + pow(p1.z - enemyCenter.z, 2));
    float distanceToNextPoint = sqrt(pow(p1.x - p2.x, 2) + pow(p1.z - p2.z, 2));
    if (distanceToEnemy >= distanceToNextPoint)return true;
    return false;
}

bool Tema2::checkCollision(glm::vec3 p1, glm::vec3 p2) {
    float distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.z - p2.z, 2));
    if (distance < 0.3 + 0.3)return true;
    return false;
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int camera)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    if (camera == 1) {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera1->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix1));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }
    else {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera2->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix2));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }
    

    mesh->Render();
}

void Tema2::Init()
{
    polygonMode = GL_FILL;
    ToggleGroundPlane();
    renderCameraTarget = true;
    camera1 = new implemented::Tema2Camera();
    camera1->Set(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    projectionMatrix1 = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    

    street = object3D::CreateStreet("street", glm::vec3(0.f, 0.f, 0.f), treePos, routeEnemy1, routeEnemy2);
    AddMeshToList(street);
    

    Mesh* grass= object3D::CreateGrass("grass", glm::vec3(0.f, 1.f, 0.f));
    AddMeshToList(grass);

    int length = treePos.size();

    Mesh* tree[150];

    for (int i = 0; i < treePos.size(); i++) {
        string aux = "tree" + to_string(i);
        tree[i] = object3D::CreateTree(aux,treePos[i]* 2.9f);
        
        AddMeshToList(tree[i]);
    }

    Mesh* car = object3D::CreateCar("car", glm::vec3(0.f, 0.808f, 0.82f));
    AddMeshToList(car);

    Mesh* enemy1 = object3D::CreateCar("enemy1", glm::vec3(0.545f, 0.f, 0.f));
    AddMeshToList(enemy1);

    Mesh* enemy2 = object3D::CreateCar("enemy2", glm::vec3(1.f, 0.549f, 0.f));
    AddMeshToList(enemy2);
    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(950, 10, resolution.x / 4.f, resolution.y / 4.f);
    camera2 = new implemented::Tema2Camera();
    camera2->Set(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    projectionMatrix2 = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    xEnemy1 = routeEnemy1[routeEnemy1.size() - 1].x; zEnemy1 = routeEnemy1[routeEnemy1.size() - 1].z;
    xEnemy2 = routeEnemy2[routeEnemy2.size() - 1].x; zEnemy2 = routeEnemy2[routeEnemy2.size() - 1].z;
    index1Enemy1 = index1Enemy2 = routeEnemy1.size() - 1;
    index2Enemy1 = index2Enemy2 = routeEnemy1.size() - 2;
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.f, 0.749f, 1.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::RenderScene(int vPort) {
    
    float posCarX = translateX ;
    float posCarZ = translateZ + 4.3;

    


    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(2.9f, 0, 2.9f);
    RenderMesh(meshes["street"], shaders["VertexColor"], modelMatrix, vPort);

    modelMatrix = glm::mat4(1);
    RenderMesh(meshes["grass"], shaders["VertexColor"], modelMatrix, vPort);

    modelMatrix = glm::mat4(1);
    RenderMesh(meshes["tree"], shaders["VertexColor"], modelMatrix, vPort);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xEnemy1, 0.15, zEnemy1);
    modelMatrix *= transform3D::RotateOY(-atan(pantaEnemy1) );
    RenderMesh(meshes["enemy1"], shaders["VertexColor"], modelMatrix, vPort);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(xEnemy2, 0.15, zEnemy2);
    modelMatrix *= transform3D::RotateOY(-atan(pantaEnemy2));
    RenderMesh(meshes["enemy2"], shaders["VertexColor"], modelMatrix, vPort);



    for (int i = 0; i < treePos.size(); i++) {
        string aux = "tree" + to_string(i);
        modelMatrix = glm::mat4(1);
        RenderMesh(meshes[aux], shaders["VertexColor"], modelMatrix, vPort);
    }
    modelMatrix = glm::mat4(1);
    
    if (vPort == 1) {
        modelMatrix *= transform3D::Translate(posCarX, 0.15, posCarZ);
    }
    else {
        modelMatrix *= transform3D::Translate(posCarX, 0.15, posCarZ);
    }
    //modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    
    RenderMesh(meshes["car"], shaders["VertexColor"], modelMatrix, vPort);
    if(vPort == 1)
    camera1->Set(glm::vec3(translateX - 1.8, 0.85, translateZ + 4.3), glm::vec3(translateX, 0.15, translateZ + 4.3), glm::vec3(0, 1, 0));
    else
        camera2->Set(glm::vec3(translateX - 0.01, 5, translateZ + 4.3), glm::vec3(translateX, 0.15, translateZ + 4.3), glm::vec3(0, 1, 0));

    if(vPort == 1)
    camera1->RotateThirdPerson_OY(angularStepOY);



}

void Tema2::Update(float deltaTimeSeconds)
{
    
    if (checkDistance(routeEnemy1[index1Enemy1], routeEnemy1[index2Enemy1], glm::vec3(xEnemy1, 0, zEnemy1))) {
        index1Enemy1--;
        index2Enemy1--;
        if (index1Enemy1 == 0) {
            index2Enemy1 = routeEnemy1.size() - 1;
        }
        if (index1Enemy1 == -1) {
            index1Enemy1 = routeEnemy1.size() - 1;
        }
        

        xEnemy1 = routeEnemy1[index1Enemy1].x; zEnemy1 = routeEnemy1[index1Enemy1].z;
        }
    pantaEnemy1 = (routeEnemy1[index1Enemy1].z - routeEnemy1[index2Enemy1].z) / (routeEnemy1[index1Enemy1].x - routeEnemy1[index2Enemy1].x);
    if (routeEnemy1[index2Enemy1].z < 0) {
        xEnemy1 += deltaTimeSeconds * cos(atan(pantaEnemy1)) * 3;
        zEnemy1 += deltaTimeSeconds * sin(atan(pantaEnemy1)) * 3;
    }
    else {
        xEnemy1 -= deltaTimeSeconds * cos(atan(pantaEnemy1)) * 3;
        zEnemy1 -= deltaTimeSeconds * sin(atan(pantaEnemy1)) * 3;
    }


    if (checkDistance(routeEnemy2[index1Enemy2], routeEnemy2[index2Enemy2], glm::vec3(xEnemy2, 0, zEnemy2))) {
       index1Enemy2--;
       index2Enemy2--;
       if (index1Enemy2 == 0) {
           index2Enemy2 = routeEnemy2.size() - 1;
       }
       if (index1Enemy2 == -1) {
           index1Enemy2 = routeEnemy2.size() - 1;
       }
       xEnemy2 = routeEnemy2[index1Enemy2].x; zEnemy2 = routeEnemy2[index1Enemy2].z;
    }
    pantaEnemy2 = (routeEnemy2[index1Enemy2].z - routeEnemy2[index2Enemy2].z) / (routeEnemy2[index1Enemy2].x - routeEnemy2[index2Enemy2].x);
    if (routeEnemy2[index2Enemy2].z < 0) {
        xEnemy2 += deltaTimeSeconds * cos(atan(pantaEnemy2)) * 5;
        zEnemy2 += deltaTimeSeconds * sin(atan(pantaEnemy2)) * 5;
    }
    else {
        xEnemy2 -= deltaTimeSeconds * cos(atan(pantaEnemy2)) * 5;
        zEnemy2 -= deltaTimeSeconds * sin(atan(pantaEnemy2)) * 5;
    }
   


    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene(1);
    //DrawCoordinateSystem();
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    projectionMatrix2 = glm::ortho(-7.f, 7.f, -5.f, 5.f, 0.1f, 200.f);
    // TODO(student): render the scene again, in the new viewport
    //DrawCoordinateSystem();
    RenderScene(2);
}

void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float carSpeed = 3.5;
    
    if (window->KeyHold(GLFW_KEY_W)) {
        float next_posCarX = translateX  + (deltaTime + 0.1) * sin(angularStepOY + M_PI / 2) * carSpeed;
        float next_posCarZ = translateZ + 4.3 + (deltaTime + 0.1)* cos(angularStepOY + M_PI / 2) * carSpeed;

        for (int i = 0; i < street->indices.size() - 2; i += 3) {
            if (PointInTriangle(glm::vec3(next_posCarX, 0, next_posCarZ), street->vertices[street->indices[i]].position * 2.9f ,
                street->vertices[street->indices[i + 1]].position * 2.9f, street->vertices[street->indices[i + 2]].position * 2.9f)) {
                
                if (checkCollision(glm::vec3(next_posCarX, 0, next_posCarZ), glm::vec3(xEnemy1, 0, zEnemy1)) == false &&
                    checkCollision(glm::vec3(next_posCarX, 0, next_posCarZ), glm::vec3(xEnemy2, 0, zEnemy2)) == false) {
                    translateZ += deltaTime * cos(angularStepOY + M_PI / 2) * carSpeed;
                    translateX += deltaTime * sin(angularStepOY + M_PI / 2) * carSpeed;
                }
                
                break;
            }
       

        }
    
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        float next_posCarX = translateX - (deltaTime - 0.1) * sin(angularStepOY + M_PI / 2) * 2;
        float next_posCarZ = translateZ + 4.3 - (deltaTime - 0.1)* cos(angularStepOY + M_PI / 2) * 2;
        for (int i = 0; i < street->indices.size() - 2; i += 3) {
      
            if (PointInTriangle(glm::vec3(next_posCarX , 0, next_posCarZ ), street->vertices[street->indices[i]].position * 2.9f,
                street->vertices[street->indices[i + 1]].position * 2.9f, street->vertices[street->indices[i + 2]].position * 2.9f)) {
                if (checkCollision(glm::vec3(next_posCarX, 0, next_posCarZ), glm::vec3(xEnemy1, 0, zEnemy1)) == false &&
                    checkCollision(glm::vec3(next_posCarX, 0, next_posCarZ), glm::vec3(xEnemy2, 0, zEnemy2)) == false) {
                    translateZ -= deltaTime * cos(angularStepOY + M_PI / 2) * 2;
                    translateX -= deltaTime * sin(angularStepOY + M_PI / 2) * 2;
                }
                

                break;
            }
        }
      
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        angularStepOY += deltaTime;
        
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        angularStepOY -= deltaTime;
        
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    
    // TODO(student): Add viewport movement and scaling logic

   

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
