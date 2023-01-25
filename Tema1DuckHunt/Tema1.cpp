#include "lab_m1/Tema1DuckHunt/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1DuckHunt/transform2D.h"
#include "lab_m1/Tema1DuckHunt/object2D.h"

using namespace std;
using namespace m1;

bool pressTrigger = false, shooted = false;

float xShoot, yShoot;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    GameOver = false;
    glm::ivec2 resolution = window->GetResolution();
    window->HidePointer();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    glm::vec3 corner = glm::vec3(0, 0, 0);
    float side = 150;

    cx = 50;
    cy = 50;
    elapsedTime = 0;
    
    translateX = 0;
    translateY = 0;
    xStart = rand() % 900 + 150; yStart = rand() % 100 + 70;
    xHead = xStart;  yHead = yStart; // coord cap rata

    scaleX = 1;
    scaleY = 1;
    speed = 150;
    
    falling = false;
    escaping = false;
    shootedDucks = 0;

    directionAngle = (rand() % (int((M_PI) * 100))) / 100.0;

    while ((directionAngle >= 0 && directionAngle <= M_PI / 6) || (directionAngle >= M_PI / 2 - M_PI / 6 && directionAngle <= M_PI / 2 + M_PI / 6)
        || (directionAngle <= M_PI && directionAngle >= M_PI - M_PI / 6))
        directionAngle = (rand() % (int ((M_PI)* 100) ) ) / 100.0;

    
    rotationAngle = 0;
    scaleScore = 0;
    nrBullets = 3; scaleBullet1 = 1; scaleBullet2 = 1; scaleBullet3 = 1;
    nrLifes = 3; scaleLife1 = 1; scaleLife2 = 1; scaleLife3 = 1;
    // Initialize angularStep
    angularStepWings = 0;

    Mesh* birdBody = object2D::CreateTriangle("birdBody", corner, side, glm::vec3(0.2f, 0.098f, 0), true);
    AddMeshToList(birdBody);

    Mesh* wing1 = object2D::CreateWing("wing1", corner, side, glm::vec3(0.2f, 0.098f, 0), true);
    AddMeshToList(wing1);

    Mesh* wing2 = object2D::CreateWing("wing2", corner, side, glm::vec3(0.2f, 0.098f, 0), true);
    AddMeshToList(wing2);

    Mesh* birdHead = object2D::CreateCircle("birdHead", corner, 27, glm::vec3(0.051, 0.173, 0), true);
    AddMeshToList(birdHead);

    Mesh* duckbill = object2D::CreateTriangle("duckbill", corner, 40, glm::vec3(0.992, 0.925, 0), true);
    AddMeshToList(duckbill);

    Mesh* lifePoint1 = object2D::CreateCircle("lifePoint1", corner, 20, glm::vec3(1, 0, 0), true);
    AddMeshToList(lifePoint1);
    Mesh* lifePoint2 = object2D::CreateCircle("lifePoint2", corner, 20, glm::vec3(1, 0, 0), true);
    AddMeshToList(lifePoint2);
    Mesh* lifePoint3 = object2D::CreateCircle("lifePoint3", corner, 20, glm::vec3(1, 0, 0), true);
    AddMeshToList(lifePoint3);

    Mesh* grass = object2D::CreateRectangle("grass", corner, 1400, 200, glm::vec3(0, 0.392, 0), true); // dark green
    AddMeshToList(grass);

    Mesh* bullet1 = object2D::CreateRectangle("bullet1", corner, 20, 45, glm::vec3(0.412, 0.412, 0.412), true);
    AddMeshToList(bullet1);
    Mesh* bullet2 = object2D::CreateRectangle("bullet2", corner, 20, 45, glm::vec3(0.412, 0.412, 0.412), true);
    AddMeshToList(bullet2);
    Mesh* bullet3 = object2D::CreateRectangle("bullet3", corner, 20, 45, glm::vec3(0.412, 0.412, 0.412), true);
    AddMeshToList(bullet3);

    Mesh* currentScore = object2D::CreateRectangle("currentScore", corner, 100, 30, glm::vec3(0, 0, 0.502), true);
    AddMeshToList(currentScore);
    Mesh* scoreFrame = object2D::CreateRectangle("scoreFrame", corner, 100, 30, glm::vec3(0, 0, 0.502), false);
    AddMeshToList(scoreFrame);

    Mesh* shootingFrame = object2D::CreateRectangle("shootingFrame", corner, side + 29, 164, glm::vec3(0, 0.749, 1), false); // mocassin
    AddMeshToList(shootingFrame);

    Mesh* aim1 = object2D::CreateCircle("aim1", corner, 13, glm::vec3(1, 0, 0), true); // mocassin
    AddMeshToList(aim1);
    Mesh* aim2 = object2D::CreateCircle("aim2", corner, 6, glm::vec3(0, 0.749, 1), true); // mocassin
    AddMeshToList(aim2);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    
    glClearColor(0, 0.749, 1, 1); // deep sky blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
   
}


void Tema1::Update(float deltaTimeSeconds)
{
    

    if (GameOver)return;

    if(falling == false && escaping == false)
        elapsedTime += deltaTimeSeconds;

    if ((nrBullets == 0 && falling == false && escaping == false) || elapsedTime >= 5.5) {
        escaping = true;
        nrLifes--;
        elapsedTime = 0;
        if (nrLifes < 3) {
            scaleLife1 = 0;
            if (nrLifes < 2) {
                scaleLife2 = 0;
                if (nrLifes < 1) {
                    scaleLife3 = 0;
                }
            }
        }

        directionAngle = M_PI / 2;
    }

    if (pressTrigger == true) {
        nrBullets--;
        if (nrBullets < 3) {
            scaleBullet1 = 0;
            if (nrBullets < 2) {
                scaleBullet2 = 0;
                if (nrBullets < 1) {
                    scaleBullet3 = 0;
                }
            }
        }

        if (shooted == true) {
            if (scaleScore < 1 && falling == false && escaping == false)
                scaleScore += 0.01f;
            falling = true;
            shootedDucks++;
            if (shootedDucks == 5) {
                shootedDucks = 0;
                speed += 50;
            }
            directionAngle = 3 * M_PI / 2;           

        }
        pressTrigger = false;
    }
    

    if (falling == false && escaping == false) {
        translateX += deltaTimeSeconds * cos(directionAngle) * speed;
        translateY += deltaTimeSeconds * sin(directionAngle) * speed;
    }
    else {
        translateX += deltaTimeSeconds * cos(directionAngle) * 600;
        translateY += deltaTimeSeconds * sin(directionAngle) * 600;
    }
    
    new_xHead = xHead + translateX;
    new_yHead = yHead + translateY;

    
    
    if (new_xHead < -1 || new_xHead > 1281) { // stanga dreapta
        directionAngle = M_PI - directionAngle;
        if (xHead + translateX < 50)translateX = 50 - xHead;
        if (yHead + translateY < 50)translateY = 50 - yHead;
    }
    if (new_yHead < -1 || new_yHead > 721) { // sus jos
        if (falling == false && escaping == false) {
            directionAngle = 2 * M_PI - directionAngle;
            if (xHead + translateX < 50)translateX = 50 - xHead;
            if (yHead + translateY < 50)translateY = 50 - yHead;
        }
        else if(falling == true) {
            falling = false;
            xStart = rand() % 900 + 150; yStart = rand() % 100 + 70;
            directionAngle = (rand() % (int((M_PI) * 100))) / 100.0;
            xHead = xStart; yHead = yStart;
            // culoare aleatoare a ratei
            {
                float r = (rand() % 100) / 100.0;
                float g = (rand() % 100) / 100.0;
                float b = (rand() % 100) / 100.0;
                glm::vec3 corner = glm::vec3(0, 0, 0);
                float side = 150;
                Mesh* birdBody = object2D::CreateTriangle("birdBody", corner, side, glm::vec3(r, g, b), true);
                AddMeshToList(birdBody);

                Mesh* wing1 = object2D::CreateWing("wing1", corner, side, glm::vec3(r, g, b), true);
                AddMeshToList(wing1);

                Mesh* wing2 = object2D::CreateWing("wing2", corner, side, glm::vec3(r, g, b), true);
                AddMeshToList(wing2);
            }
            while ((directionAngle >= 0 && directionAngle <= M_PI / 6) || (directionAngle >= M_PI / 2 - M_PI / 6 && directionAngle <= M_PI / 2 + M_PI / 6)
                || (directionAngle <= M_PI && directionAngle >= M_PI - M_PI / 6))
                directionAngle = (rand() % (int((M_PI) * 100))) / 100.0;
            deltaTimeSeconds = 0;
            translateX = 0;
            translateY = 0;
            nrBullets = 3;
            scaleBullet1 = scaleBullet2 = scaleBullet3 = 1;
            elapsedTime = 0;
        }
        else if(escaping == true) {
            if (nrLifes <= 0) {
                GameOver = true;
                return;
            }
            escaping = false;
            xStart = rand() % 900 + 150; yStart = rand() % 100 + 70;
            directionAngle = (rand() % (int((M_PI) * 100))) / 100.0;
            xHead = xStart; yHead = yStart;
            // culoare aleatoare a ratei
            {
                float r = (rand() % 100) / 100.0;
                float g = (rand() % 100) / 100.0;
                float b = (rand() % 100) / 100.0;
                glm::vec3 corner = glm::vec3(0, 0, 0);
                float side = 150;
                Mesh* birdBody = object2D::CreateTriangle("birdBody", corner, side, glm::vec3(r, g, b), true);
                AddMeshToList(birdBody);

                Mesh* wing1 = object2D::CreateWing("wing1", corner, side, glm::vec3(r, g, b), true);
                AddMeshToList(wing1);

                Mesh* wing2 = object2D::CreateWing("wing2", corner, side, glm::vec3(r, g, b), true);
                AddMeshToList(wing2);
            }
            while ((directionAngle >= 0 && directionAngle <= M_PI / 6) || (directionAngle >= M_PI / 2 - M_PI / 6 && directionAngle <= M_PI / 2 + M_PI / 6)
                || (directionAngle <= M_PI && directionAngle >= M_PI - M_PI / 6))
                directionAngle = (rand() % (int((M_PI) * 100))) / 100.0;
            deltaTimeSeconds = 0;
            translateX = 0;
            translateY = 0;
            scaleBullet1 = scaleBullet2 = scaleBullet3 = 1;
            nrBullets = 3;
            elapsedTime = 0;
        }
    }

    if(falling == false)
        angularStepWings += deltaTimeSeconds * 0.5;

    if (angularStepWings > M_PI / 8)angularStepWings = 0;

    // tinta
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateXMOUSE, translateYMOUSE);

        RenderMesh2D(meshes["aim2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["aim1"], shaders["VertexColor"], modelMatrix);
        
    }

    // scor
    {
        modelMatrix = glm::mat3(1);
        
        
        modelMatrix *= transform2D::Translate(1150, 680);
        modelMatrix *= transform2D::Scale(scaleScore, 1);

        RenderMesh2D(meshes["currentScore"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1150, 680);
        RenderMesh2D(meshes["scoreFrame"], shaders["VertexColor"], modelMatrix);
    }

    // gloante
    {
        modelMatrix = glm::mat3(1);
        modelMatrix = transform2D::Translate(65, 600);
        modelMatrix *= transform2D::Scale(scaleBullet1, scaleBullet1);
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix = transform2D::Translate(35 , 600);
        modelMatrix *= transform2D::Scale(scaleBullet2, scaleBullet2);
        RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix = transform2D::Translate(5, 600);
        modelMatrix *= transform2D::Scale(scaleBullet3, scaleBullet3);
        RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], modelMatrix);
    }

    // iarba
    {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);
    }


    // punctele de viata
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(115, 690);
        modelMatrix *= transform2D::Scale(scaleLife1, scaleLife1);
        RenderMesh2D(meshes["lifePoint1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(70, 690);
        modelMatrix *= transform2D::Scale(scaleLife2, scaleLife2);
        RenderMesh2D(meshes["lifePoint2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(25, 690);
        modelMatrix *= transform2D::Scale(scaleLife3, scaleLife3);
        RenderMesh2D(meshes["lifePoint3"], shaders["VertexColor"], modelMatrix);
    }

    // chenar shooting
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(xStart, yStart);
        modelMatrix *= transform2D::Rotate(directionAngle);
        modelMatrix *= transform2D::Translate(-xStart, -yStart);

        modelMatrix *= transform2D::Translate(xStart - 131, yStart - 83);
        RenderMesh2D(meshes["shootingFrame"], shaders["VertexColor"], modelMatrix);
    }

    // cap rata
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateX, translateY);

        modelMatrix *= transform2D::Translate(xStart, yStart);
        modelMatrix *= transform2D::Rotate(directionAngle);
        modelMatrix *= transform2D::Translate(-xStart, -yStart);
        
        modelMatrix *= transform2D::Translate(xStart, yStart); 
        
        RenderMesh2D(meshes["birdHead"], shaders["VertexColor"], modelMatrix);
    }
    


    // corp rata
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX, translateY); 
        modelMatrix *= transform2D::Translate(xStart, yStart);
        modelMatrix *= transform2D::Rotate(directionAngle);
        modelMatrix *= transform2D::Translate(-xStart, -yStart);
        
        modelMatrix *= transform2D::Translate(xStart + 20, yStart); 
        
        RenderMesh2D(meshes["birdBody"], shaders["VertexColor"], modelMatrix);
        
    }

    //aripa 1
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX, translateY); 
        modelMatrix *= transform2D::Translate(xStart, yStart);
        modelMatrix *= transform2D::Rotate(directionAngle);
        modelMatrix *= transform2D::Translate(-xStart, -yStart);
        
        modelMatrix *= transform2D::Translate(xStart - 70, yStart);
        modelMatrix *= transform2D::Rotate(angularStepWings);
        modelMatrix *= transform2D::Translate(-(xStart - 70), -(yStart));
        modelMatrix *= transform2D::Translate(xStart + 10, yStart + 6);
        
        RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelMatrix);
        
    }
    
    
    //aripa 2
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(xStart, yStart);
        modelMatrix *= transform2D::Rotate(directionAngle);
        modelMatrix *= transform2D::Translate(-xStart, -yStart);
        
        modelMatrix *= transform2D::Translate(xStart - 60, yStart);
        modelMatrix *= transform2D::Rotate(-angularStepWings);
        modelMatrix *= transform2D::Translate(-(xStart - 60), -(yStart));
        modelMatrix *= transform2D::Translate(xStart - 140, yStart - 8);
        
        modelMatrix *= transform2D::Rotate(M_PI);
        
        RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelMatrix);
    }
    
    // cioc rata
    {
        modelMatrix = glm::mat3(1);
        
        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(xStart, yStart);
        modelMatrix *= transform2D::Rotate(directionAngle);
        modelMatrix *= transform2D::Translate(-xStart, -yStart);
         
        modelMatrix *= transform2D::Translate(xStart + 50, yStart - 7); 
        
        modelMatrix *= transform2D::Rotate(-0.1f);  
        
        RenderMesh2D(meshes["duckbill"], shaders["VertexColor"], modelMatrix);
    }
    
    

    shooted = false;
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    GameOver = false;
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    translateXMOUSE = mouseX;

    if (window->GetResolution().y == 720)
        translateYMOUSE = window->GetResolution().y - mouseY;
    else
        translateYMOUSE = window->GetResolution().y - mouseY; 
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2 && nrBullets > 0) {
        pressTrigger = true;
        xShoot = mouseX;
        yShoot = window->GetResolution().y - mouseY;
       
        xShoot = xShoot - xStart - translateX;
        yShoot = yShoot - yStart - translateY;
        // rotesc tinta cu -unghiul cu care rotesc chenarul, iar daca se gaseste in
        // chenarul nerotit, inseamna ca tinta se gaseste in chenarul rotit
        float xShootTrans = xShoot * cos(- directionAngle) - yShoot * sin(- directionAngle);
        float yShootTrans = xShoot * sin(- directionAngle) + yShoot * cos(- directionAngle);
        xShootTrans += xStart + translateX;
        yShootTrans += yStart + translateY;
        xShoot += xStart + translateX;
        yShoot += yStart + translateY;
        if (xShootTrans >= (translateX + xStart - 131) && xShootTrans <= (translateX + 179 + xStart - 131) 
            && yShootTrans >= (translateY + yStart - 83) && yShootTrans <= (translateY + 164 + yStart - 83) ) {
            if(escaping == false && falling == false)
                shooted = true;
        }
        
    }
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
