#include "mesh.h"
#include "gFunction.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    shader.use();
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));

        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        shader.setInt((name + number), i);
        GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }

    // draw mesh
    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));

    // always good practice to set everything back to defaults once configured.
    GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::setupMesh()
{
    // create buffers/arrays
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glGenBuffers(1, &EBO));

    GLCall(glBindVertexArray(VAO));
    // load data into vertex buffers
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW)));

    // set the vertex attribute pointers
    // vertex Positions
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0));
    // vertex normals
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal)));
    // vertex texture coords
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords)));
    // vertex tangent
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent)));
    // vertex bitangent
    GLCall(glEnableVertexAttribArray(4));
    GLCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent)));
    // ids
    GLCall(glEnableVertexAttribArray(5));
    GLCall(glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs)));

    // weights
    GLCall(glEnableVertexAttribArray(6));
    GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights)));
    GLCall(glBindVertexArray(0));
}
