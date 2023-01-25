#include "object3D.h"
#include "Tema2.h"

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object3D::CreateStreet(const std::string& name, glm::vec3 color, vector<glm::vec3>& treePos, vector<glm::vec3>& routeEnemy1, vector<glm::vec3>& routeEnemy2) {
	Mesh* street = new Mesh(name);
    vector<glm::vec3> points = {
        glm::vec3(-7.46, 0, 1.82),
        glm::vec3(-7.12, 0, 2.26),
        glm::vec3(-6.46, 0, 2.64),
        glm::vec3(-5.6, 0, 2.92),
        glm::vec3(-5, 0, 3),
        glm::vec3(-4.5, 0, 2.98),
        glm::vec3(-4, 0, 3),
        glm::vec3(-3.64, 0, 2.8),
        glm::vec3(-3.1, 0, 2.58),
        glm::vec3(-2.78, 0, 2.4),
        glm::vec3(-2.42, 0, 2.18),
        glm::vec3(-2, 0, 2),
        glm::vec3(-1.52, 0, 1.76),
        glm::vec3(-1.02, 0, 1.6),
        glm::vec3(-0.62, 0, 1.46),
        glm::vec3(0, 0, 1.28),
        glm::vec3(0.54, 0, 1.32),
        glm::vec3(1.14, 0, 1.56),
        glm::vec3(1.86, 0, 1.92),
        glm::vec3(2.22, 0, 2.16),
        glm::vec3(2.8, 0, 2.42),
        glm::vec3(3.4, 0, 2.56),
        glm::vec3(3.94, 0, 2.56),
        glm::vec3(4.46, 0, 2.48),
        glm::vec3(4.78, 0, 2.42),
        glm::vec3(5.22, 0, 2.28),
        glm::vec3(5.66, 0, 2.14),
        glm::vec3(6.04, 0, 1.76),
        glm::vec3(6.18, 0, 1.26),
        glm::vec3(6.4, 0, 0.72),
        glm::vec3(6.44, 0, -0.24),
        glm::vec3(6.2, 0, -0.68),
        glm::vec3(6, 0, -1),
        glm::vec3(5.52, 0, -1.32),
        glm::vec3(5.1, 0, -1.6),
        glm::vec3(4.6, 0, -1.9),
        glm::vec3(4.14, 0, -2.1),
        glm::vec3(3.68, 0, -2.28),
        glm::vec3(3.3, 0, -2.36),
        glm::vec3(2.82, 0, -2.36),
        glm::vec3(2.16, 0, -2.2),
        glm::vec3(1.72, 0, -1.86),
        glm::vec3(1.46, 0, -1.68),
        glm::vec3(1.08, 0, -1.56),
        glm::vec3(0.6, 0, -1.48),
        glm::vec3(0.28, 0, -1.52),
        glm::vec3(-0.42, 0, -1.6),
        glm::vec3(-0.74, 0, -1.74),
        glm::vec3(-1, 0, -2),
        glm::vec3(-1.34, 0, -2.36),
        glm::vec3(-1.78, 0, -2.64),
        glm::vec3(-2, 0, -3),
        glm::vec3(-2.32, 0, -3.2),
        glm::vec3(-2.98, 0, -3.5),
        glm::vec3(-3.54, 0, -3.56),
        glm::vec3(-4.14, 0, -3.56),
        glm::vec3(-4.9, 0, -3.56),
        glm::vec3(-5.56, 0, -3.44),
        glm::vec3(-5.9, 0, -3.24),
        glm::vec3(-6.3, 0, -2.92),
        glm::vec3(-6.58, 0, -2.6),
        glm::vec3(-7, 0, -2),
        glm::vec3(-7.38, 0, -1.6),
        glm::vec3(-7.74, 0, -1.12),
        glm::vec3(-7.96, 0, -0.68),
        glm::vec3(-8.08, 0, -0.22),
        glm::vec3(-8.14, 0, 0.4),
        glm::vec3(-8, 0, 1),
        glm::vec3(-7.66, 0, 1.56),
    };

    glm::vec3 P, D, R, A, P1, P2, tree1, tree2, enemy1, enemy2;
    glm::vec3 P_next, D_next, R_next, A_next, P1_next, P2_next;
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    unsigned int iterator = 0;
    P1 = points[0];
    P2 = points[1];
    D = P2 - P1;
    
    P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));

    R = P1 + 0.45f * P;
    A = P1 - 0.40f * P;

    enemy1 = P1 + 0.25f * P;
    enemy1 *= 2.9f;
    enemy2 = P1 - 0.20f * P;
    enemy2 *= 2.9f;
    routeEnemy1.push_back(enemy1);
    routeEnemy2.push_back(enemy2);

    vertices.push_back(VertexFormat(R, color)); // iterator
    vertices.push_back(VertexFormat(A, color));  // iterator + 1
    for (int i = 1; i < points.size() - 1; i++) {
        P1_next = points[i];
        P2_next = points[i + 1];

        D_next = P2_next - P1_next;
        P_next = glm::normalize(glm::cross(D_next, glm::vec3(0, 1, 0)));
        R_next = P1_next + 0.45f * P_next;
        A_next = P1_next - 0.40f * P_next;
        
        tree1 = P1_next + 0.70f * P_next;
        tree2 = P1_next - 0.65f * P_next;

        enemy1 = P1_next + 0.25f * P_next;
        enemy1 *= 2.9f;
        enemy2 = P1_next - 0.20f * P_next;
        enemy2 *= 2.9f;

        routeEnemy1.push_back(enemy1);
        routeEnemy2.push_back(enemy2);

        int randomValue = rand() % 120;
        if (randomValue >= 0 && randomValue <= 30)
            treePos.push_back(tree1);
        else
        if (randomValue > 30 && randomValue <= 60)
            treePos.push_back(tree2);
        else if(randomValue <= 100 && randomValue > 60) {
            treePos.push_back(tree1);
            treePos.push_back(tree2);
        }
        
        vertices.push_back(VertexFormat(R_next, color)); // iterator + 2
        vertices.push_back(VertexFormat(A_next, color)); // iterator + 3


        indices.push_back(iterator);
        indices.push_back(iterator + 1);
        indices.push_back(iterator + 3);

        indices.push_back(iterator);
        indices.push_back(iterator + 2);
        indices.push_back(iterator + 3);
        iterator += 2;

        P1 = P1_next;
        P2 = P2_next;
        D = D_next;
        P = P_next;
        R = R_next;
        A = A_next;
    }
    indices.push_back(iterator);
    indices.push_back(iterator + 1);
    indices.push_back(1);

    indices.push_back(iterator);
    indices.push_back(0);
    indices.push_back(1);

    
    //map->SetDrawMode(GL_LINE_LOOP);
    street->InitFromData(vertices, indices);
    return street;
}

Mesh* object3D::CreateGrass(const std::string& name, glm::vec3 color) {
    Mesh* grass = new Mesh(name);
    glm::vec3 corner = glm::vec3(-30, -0.1, -30);
    vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(60, 0, 0), color),
        VertexFormat(corner + glm::vec3(60, 0, 60), color),
        VertexFormat(corner + glm::vec3(0, 0 , 60), color)
    };
    vector<unsigned int> indices = { 0, 1, 2, 0, 3, 2 };

    grass->InitFromData(vertices, indices);
    return grass;
}

Mesh* object3D::CreateTree(const std::string& name, glm::vec3 treeCorner) {
    Mesh* tree = new Mesh(name);
    glm::vec3 color1 = glm::vec3(0.545f, 0.271f, 0.075f);
    glm::vec3 color2 = glm::vec3(0.f, 0.392f, 0.f);
    vector<VertexFormat> vertices = {
        VertexFormat(treeCorner + glm::vec3(0, 0, 0.35), color1),
        VertexFormat(treeCorner + glm::vec3(0.35, 0, 0.35), color1),
        VertexFormat(treeCorner + glm::vec3(0, 1, 0.35), color1),
        VertexFormat(treeCorner + glm::vec3(0.35, 1, 0.35), color1),
        VertexFormat(treeCorner + glm::vec3(0, 0, 0), color1),
        VertexFormat(treeCorner + glm::vec3(0.35, 0, 0), color1),
        VertexFormat(treeCorner + glm::vec3(0, 1, 0), color1),
        VertexFormat(treeCorner + glm::vec3(0.35, 1, 0), color1),

        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0, 0, 0.85), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0.85, 0, 0.85), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0, 0.6, 0.85), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0.85, 0.6, 0.85), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0, 0, 0), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0.85, 0, 0), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0, 0.6, 0), color2),
        VertexFormat(treeCorner + glm::vec3(-0.25, 1, -0.25) + glm::vec3(0.85, 0.6, 0), color2),
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4,

        8,9,10,
        9,12,10,
        10,11,15,
        10,15,14,
        9,15,11,
        9,13,15,
        14,15,12,
        15,13,12,
        8,12,9,
        9,12,13,
        10,14,12,
        8,10,12,
    };
    tree->InitFromData(vertices, indices);
    return tree;
}

Mesh* object3D::CreateCar(const std::string& name, glm::vec3 color) {
    Mesh* car = new Mesh(name);

    vector<VertexFormat> vertices = {
        /*VertexFormat(glm::vec3(-0.75, -0.35, 0.35),color),
        VertexFormat(glm::vec3(0.75, -0.35, 0.35),color),
        VertexFormat(glm::vec3(-0.75, 0.35, 0.35),color),
        VertexFormat(glm::vec3(0.75, 0.35, 0.35),color),
        VertexFormat(glm::vec3(-0.75, -0.35, -0.35),color),
        VertexFormat(glm::vec3(0.75, -0.35, -0.35),color),
        VertexFormat(glm::vec3(-0.75 , 0.35, -0.35),color),
        VertexFormat(glm::vec3(0.75, 0.35, -0.35),color),*/
        VertexFormat(glm::vec3(-0.40, -0.15, 0.20),color),
        VertexFormat(glm::vec3(0.40, -0.15, 0.20),color),
        VertexFormat(glm::vec3(-0.40, 0.15, 0.20),color),
        VertexFormat(glm::vec3(0.40, 0.15, 0.20),color),
        VertexFormat(glm::vec3(-0.40, -0.15, -0.20),color),
        VertexFormat(glm::vec3(0.40, -0.15, -0.20),color),
        VertexFormat(glm::vec3(-0.40 , 0.15, -0.20),color),
        VertexFormat(glm::vec3(0.40, 0.15, -0.20),color),
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4,
    };

    car->InitFromData(vertices, indices);
    return car;
}