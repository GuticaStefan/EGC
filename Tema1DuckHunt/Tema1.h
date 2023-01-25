#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
         Tema1();
        ~Tema1();

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

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStepWings;
        float xHead, yHead, new_xHead, new_yHead;
        bool shooted;

        float xFrame, yFrame;

        float directionAngle;
        float rotationAngle;

        float xStart, yStart;

        float speed;

        bool falling, escaping;
        
        float translateXMOUSE, translateYMOUSE;
        float scaleScore;
        float nrBullets, nrLifes;
        float scaleBullet1, scaleBullet2, scaleBullet3;
        float scaleLife1, scaleLife2, scaleLife3;
        
        bool GameOver;
        float elapsedTime;
        int shootedDucks;

    };
}   // namespace m1
