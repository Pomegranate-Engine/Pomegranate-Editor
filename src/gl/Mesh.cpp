//
// Created by rlebg on 4/21/2024.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Vec3> face_normals)
{
    this->vertices = vertices;
    this->indices = indices;
    this->face_normals = face_normals;

    generate_buffers();
}

void Mesh::generate_buffers() {
    //Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind VAO
    glBindVertexArray(VAO);

    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Convert vertices to float array
    std::vector<float> data;
    for (const Vertex& v : vertices) {
        data.push_back(v.position.x);
        data.push_back(v.position.y);
        data.push_back(v.position.z);
        data.push_back(v.normal.x);
        data.push_back(v.normal.y);
        data.push_back(v.normal.z);
        data.push_back(v.tex_coords.x);
        data.push_back(v.tex_coords.y);
        data.push_back(v.color.x);
        data.push_back(v.color.y);
        data.push_back(v.color.z);
        data.push_back(v.color.w);
    }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    //Bind EBO with face normals
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    //Set vertex attributes
    //Position
    glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
    //Normal
    glVertexAttribPointer(VERTEX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NORMAL);
    //Tex coords
    glVertexAttribPointer(VERTEX_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_TEXCOORD);
    //Color
    glVertexAttribPointer(VERTEX_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_COLOR);

    //Unbind VAO
    glBindVertexArray(0);
}

void Mesh::draw()
{
    //Bind VAO
    glBindVertexArray(VAO);

    //Draw
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    //Unbind VAO
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
