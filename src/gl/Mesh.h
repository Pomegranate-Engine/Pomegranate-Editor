#ifndef POMEGRANATE_ENGINE_MESH_H
#define POMEGRANATE_ENGINE_MESH_H
#ifdef USE_OPENGL
#include<SDL3/SDL.h>
#include<glew/glew.h>
#include<GL/GL.h>
#include<GL/GLU.h>
#include<Pomegranate/vec_math.h>
using namespace Pomegranate;

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 tex_coords;
    Vec4 color;
};

enum VertexAttributeLocation {
    VERTEX_ATTRIBUTE_POSITION = 0,
    VERTEX_ATTRIBUTE_NORMAL = 1,
    VERTEX_ATTRIBUTE_TEXCOORD = 2,
    VERTEX_ATTRIBUTE_COLOR = 3
};

class Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Vec3> face_normals;
    unsigned int VAO, VBO, EBO;
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Vec3> face_normals);
    void generate_buffers();
    void draw();
    ~Mesh();
};

#endif
#endif //POMEGRANATE_ENGINE_MESH_H
