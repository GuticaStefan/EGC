#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;

namespace object3D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateStreet(const std::string& name, glm::vec3 color, vector<glm::vec3>& treePos, vector<glm::vec3>& routeEnemy1, vector<glm::vec3>& routeEnemy2);
    Mesh* CreateGrass(const std::string& name, glm::vec3 color);
    Mesh* CreateTree(const std::string& name, glm::vec3 treeCorner);
    Mesh* CreateCar(const std::string& name, glm::vec3 color);
}
