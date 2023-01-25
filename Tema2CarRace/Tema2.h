#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Tema2lab_camera.h"
namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };
        
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        void RenderScene(int vPort);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int camera);

        bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b);
        bool PointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

        bool checkDistance(glm::vec3 p1, glm::vec3 p2, glm::vec3 enemyCenter);
        bool checkCollision(glm::vec3 p1, glm::vec3 p2);

    protected:
        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        implemented::Tema2Camera *camera1, *camera2;
        glm::mat4 projectionMatrix1, projectionMatrix2;
        bool renderCameraTarget;
        GLenum polygonMode;
        ViewportArea miniViewportArea;

        //float posCarX, posCarZ;
    };
}   // namespace m1

