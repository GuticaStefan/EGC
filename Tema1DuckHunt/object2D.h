#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 triangleTop, float length, glm::vec3 color, bool fill );
    Mesh* CreateCircle(const std::string& name, glm::vec3 triangleTop, float radius, glm::vec3 color, bool fill);
    Mesh* CreateWing(const std::string& name, glm::vec3 triangleTop, float length, glm::vec3 color, bool fill);
}
