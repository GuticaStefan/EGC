#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object2D::CreateWing(
    const std::string& name,
    glm::vec3 triangleTop,
    float length,
    glm::vec3 color,
    bool fill)
{
    Mesh* wing = new Mesh(name);
    glm::vec3 middleBase = triangleTop - glm::vec3(2*length/3 - length/6, 0, 0);
    std::vector<VertexFormat> vertices = {
        VertexFormat(middleBase - glm::vec3(length/8, 0, 0), color),
        VertexFormat(middleBase + glm::vec3(length/8, 0, 0), color),
        VertexFormat(middleBase + glm::vec3(0, length/2, 0), color),
    };
    
    std::vector<unsigned int> indices = {0, 1, 2};

    if (!fill) {
        wing->SetDrawMode(GL_LINE_LOOP);
    }
    wing->InitFromData(vertices, indices);
    return wing;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 triangleTop,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 top = triangleTop;


    std::vector<VertexFormat> vertices;
    int numberOfSides = 100;
    std::vector<unsigned int> indices;
    float radius = length;
    const int numberOfVertices = numberOfSides + 2;
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
    
    for (int i = 0; i < numberOfSides; i++) {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(i * 2 * M_PI / numberOfSides), radius* sin(i * 2 * M_PI / numberOfSides), 0), color));
        indices.push_back(i);
    }
    
    indices.push_back(numberOfSides);
    indices.push_back(1);
    Mesh* circle = new Mesh(name);
    

    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateTriangle(
    const std::string &name,
    glm::vec3 triangleTop,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 top = triangleTop;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(top, color),
        VertexFormat(top - glm::vec3(length, 0, 0) + glm::vec3(0, length / 5, 0), color),
        VertexFormat(top - glm::vec3(length, 0, 0) - glm::vec3(0, length / 5, 0), color)
      
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    } else {
    //    // Draw 2 triangles. Add the remaining 2 indices
        /*indices.push_back(0);
        indices.push_back(2);*/
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

