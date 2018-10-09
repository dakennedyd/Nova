
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#include "Mesh.h"
#include "Init.h"
#include "graphics/opengl/RendererInit.h"
#include "math/Vector.h"
//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

namespace Nova
{
Mesh::Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
           const Vec3 &color, bool isDoubleSided)
    : mSize(static_cast<GLsizei>(vertices.size())),
      mNumIndices(static_cast<GLsizei>(indices.size()))
{
    mState.doubleSided = isDoubleSided;
    constructVertices(vertices, indices);
    constructVAO();
}
Mesh::Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
           const std::vector<GLfloat> &normals, const Vec3 &color, bool isDoubleSided)
    : mSize(static_cast<GLsizei>(vertices.size())),
      mNumIndices(static_cast<GLsizei>(indices.size()))
{
    mState.doubleSided = isDoubleSided;
    constructVertices(vertices, indices);
    constructNormals(normals);
    constructVAO();
}
Mesh::Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
           const std::vector<GLfloat> &normals, const std::vector<GLfloat> &textureCoordinates,
           const Vec3 &color, bool isDoubleSided)
    : mSize(static_cast<GLsizei>(vertices.size())),
      mNumIndices(static_cast<GLsizei>(indices.size()))
{
    mState.doubleSided = isDoubleSided;
    constructVertices(vertices, indices);
    constructNormals(normals);
    constructTextureCoordinates(textureCoordinates);
    constructVAO();
}

/*Mesh Mesh::operator=(const Mesh & otherMesh)
{
        glGenBuffers(1, &mVAO);

        otherMesh.bind();
        void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        bind();
        glBufferData(GL_ARRAY_BUFFER, otherMesh.getNumVertices() * 3 * sizeof(GLfloat), data,
GL_STREAM_DRAW);

        otherMesh.bind();
        glUnmapBuffer(GL_ARRAY_BUFFER);
        return *this;
}*/

Mesh::Mesh(Mesh &&otherMesh)
    : mVAO(otherMesh.mVAO), mSize(otherMesh.mSize), mNumIndices(otherMesh.mNumIndices),
      mState(std::move(otherMesh.mState)), mColor(std::move(otherMesh.mColor))
{
    otherMesh.mVAO = -1;
}

void Mesh::bind() const { glBindVertexArray(mVAO); }
void Mesh::unBind() const { glBindVertexArray(0); }

void Mesh::setColor(const Vec3 &color) { mColor = color; }

const Vec3 &Mesh::getColor() const { return mColor; }

const GLsizei Mesh::getNumVertices() const
{
    return mSize / 3; // WARNING: i assume the mesh is always composed of triangles!
}

const GLsizei Mesh::getNumIndices() const { return mNumIndices; }

const GLsizei Mesh::getMode() const { return mState.mode; }

/*void Mesh::setData(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
{
        destroyData();
        constructVertices(vertices, indices);
        constructVAO();
}

void Mesh::setData(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices,
        const std::vector<GLfloat>& normals)
{
        destroyData();
        constructVertices(vertices, indices);
        constructNormals(normals);
        constructVAO();
}

void Mesh::setData(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices,
        const std::vector<GLfloat>& normals, const std::vector<GLfloat>& textureCoordinates)
{
        destroyData();
        constructVertices(vertices, indices);
        constructNormals(normals);
        constructTextureCoordinates(textureCoordinates);
        constructVAO();
}*/

Mesh Mesh::makeBox(const float length, const float width, const float height,
                   const std::vector<GLfloat> &textureCoordinates)
{
    float l = length / 2.0f;
    float w = width / 2.0f;
    float h = height / 2.0f;

    return Mesh(
        // vertices
        std::vector<GLfloat>{// front
                             -w, -h, l, w, -h, l, w, h, l, -w, h, l,
                             // top
                             -w, h, l, w, h, l, w, h, -l, -w, h, -l,
                             // back
                             w, -h, -l, -w, -h, -l, -w, h, -l, w, h, -l,
                             // bottom
                             -w, -h, -l, w, -h, -l, w, -h, l, -w, -h, l,
                             // left
                             -w, -h, -l, -w, -h, l, -w, h, l, -w, h, -l,
                             // right
                             w, -h, l, w, -h, -l, w, h, -l, w, h, l},
        // indices
        std::vector<GLuint>{// front
                            0, 1, 2, 2, 3, 0,
                            // top
                            4, 5, 6, 6, 7, 4,
                            // back
                            8, 9, 10, 10, 11, 8,
                            // bottom
                            12, 13, 14, 14, 15, 12,
                            // left
                            16, 17, 18, 18, 19, 16,
                            // right
                            20, 21, 22, 22, 23, 20},
        // normals
        std::vector<GLfloat>{// front
                             0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                             // top
                             0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
                             // back
                             0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
                             // bottom
                             0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,
                             // left
                             -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
                             // right
                             1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
        // textures coords
        std::vector<GLfloat>{0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                             1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
                             0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                             1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0});
}

Mesh Mesh::makeIcosahedron(const float size)
{
    const float x = 0.525731112119133606f / 2.0f * size;
    const float z = 0.850650808352039932f / 2.0f * size;
    // const float z = 0.5f*radius;
    // const float x = z*0.6f*radius;

    return Mesh(
        std::vector<GLfloat>{-x,   0.0f, z,    x,   0.0f, z,    -x,   0.0f, -z,   x,    0.0f, -z,
                             0.0f, z,    x,    0.0, z,    -x,   0.0f, -z,   x,    0.0f, -z,   -x,
                             z,    x,    0.0f, -z,  x,    0.0f, z,    -x,   0.0f, -z,   -x,   0.0f},
        std::vector<GLuint>{0, 1, 4, 0, 4, 9, 9,  4, 5, 4,  8, 5, 4,  1,  8,  8, 1, 10, 8,  10,
                            3, 5, 8, 3, 5, 3, 2,  2, 3, 7,  7, 3, 10, 7,  10, 6, 7, 6,  11, 11,
                            6, 0, 0, 6, 1, 6, 10, 1, 9, 11, 0, 9, 2,  11, 9,  5, 2, 7,  11, 2},
        std::vector<GLfloat>{0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f,
                             1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
                             0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f});
}

Mesh Mesh::makeSkyBoxMesh()
{
    return Mesh(
        std::vector<GLfloat>{
            // front
            -1.0, -1.0, 1.0, // 0 bottom left
            1.0, -1.0, 1.0,  // 1 bottom right
            1.0, 1.0, 1.0,   // 2 top right
            -1.0, 1.0, 1.0,  // 3 top left
            // back
            -1.0, -1.0, -1.0, // 4
            1.0, -1.0, -1.0,  // 5
            1.0, 1.0, -1.0,   // 6
            -1.0, 1.0, -1.0,  // 7
        },
        std::vector<GLuint>{
            0, 2, 1, 2, 0, 3, // front
            1, 6, 5, 6, 1, 2, // top
            7, 5, 6, 5, 7, 4, // back
            4, 3, 0, 3, 4, 7, // bottom
            4, 1, 5, 1, 4, 0, // left
            3, 6, 2, 6, 3, 7, // right
        });
}

Mesh Mesh::makeIcoSphere(const float radius, const int iterations)
{
    const float x = 0.525731112119133606f / 2.0f * radius;
    const float z = 0.850650808352039932f / 2.0f * radius;

    std::vector<GLfloat> vertices{-x, 0.0f, z, x,  0.0f, z,  -x, 0.0f, -z, x,  0.0f, -z, 0.0f, z,
                                  x,  0.0,  z, -x, 0.0f, -z, x,  0.0f, -z, -x, z,    x,  0.0f, -z,
                                  x,  0.0f, z, -x, 0.0f, -z, -x, 0.0f};
    std::vector<GLuint> indices{0, 1, 4, 0, 4, 9, 9,  4, 5, 4,  8, 5, 4,  1,  8,  8, 1, 10, 8,  10,
                                3, 5, 8, 3, 5, 3, 2,  2, 3, 7,  7, 3, 10, 7,  10, 6, 7, 6,  11, 11,
                                6, 0, 0, 6, 1, 6, 10, 1, 9, 11, 0, 9, 2,  11, 9,  5, 2, 7,  11, 2};

    for (int i = 0; i < iterations; i++)
    {
        size_t verticesSize = vertices.size();
        size_t indicesSize = indices.size();
        for (size_t indicesCounter = 0; indicesCounter < indicesSize; indicesCounter += 3)
        {
            int v1 = indices[indicesCounter], v2 = indices[indicesCounter + 1],
                v3 = indices[indicesCounter + 2];
            Vec3 a(vertices[v1 * 3], vertices[v1 * 3 + 1], vertices[v1 * 3 + 2]);
            Vec3 b(vertices[v2 * 3], vertices[v2 * 3 + 1], vertices[v2 * 3 + 2]);
            Vec3 c(vertices[v3 * 3], vertices[v3 * 3 + 1], vertices[v3 * 3 + 2]);

            Vec3 ab = a + b;
            ab.normalizeSelf();
            ab = ab / 2.0f * radius;
            Vec3 bc = b + c;
            bc.normalizeSelf();
            bc = bc / 2.0f * radius;
            Vec3 ca = c + a;
            ca.normalizeSelf();
            ca = ca / 2.0f * radius;

            vertices.push_back(ab.getX());
            vertices.push_back(ab.getY());
            vertices.push_back(ab.getZ());
            vertices.push_back(bc.getX());
            vertices.push_back(bc.getY());
            vertices.push_back(bc.getZ());
            vertices.push_back(ca.getX());
            vertices.push_back(ca.getY());
            vertices.push_back(ca.getZ());

            indices.push_back(static_cast<GLuint>(v1));
            indices.push_back(static_cast<GLuint>(verticesSize / 3));     // ab
            indices.push_back(static_cast<GLuint>(verticesSize / 3 + 2)); // ca

            indices.push_back(static_cast<GLuint>(verticesSize / 3));     // ab
            indices.push_back(static_cast<GLuint>(verticesSize / 3 + 1)); // bc
            indices.push_back(static_cast<GLuint>(verticesSize / 3 + 2)); // ca

            indices.push_back(static_cast<GLuint>(verticesSize / 3)); // ab
            indices.push_back(static_cast<GLuint>(v2));
            indices.push_back(static_cast<GLuint>(verticesSize / 3 + 1)); // bc

            indices.push_back(static_cast<GLuint>(verticesSize / 3 + 2)); // ca
            indices.push_back(static_cast<GLuint>(verticesSize / 3 + 1)); // bc
            indices.push_back(static_cast<GLuint>(v3));

            verticesSize += 9;
        }
        indices.erase(indices.begin(), indices.begin() + indicesSize);
    }

    return Mesh{vertices, indices};
}

Mesh Mesh::makeQuad(const float width, const float height,
                    const std::vector<GLfloat> &textureCoordinates)
{
    return Mesh(std::vector<float>{0.5f * width, 0.5f * height, 0.0f,   //  0 Top Right
                                   0.5f * width, -0.5f * height, 0.0f,  //  1 Bottom Right
                                   -0.5f * width, -0.5f * height, 0.0f, // 2 Bottom Left
                                   -0.5f * width, 0.5f * height, 0.0f}, // 3 Top Left
                std::vector<unsigned int>{0, 1, 3, 1, 2, 3},
                std::vector<float>{0.0f, 0.0f, -1.0f,  //  0 Top Right
                                   0.0f, 0.0f, -1.0f,  //  1 Bottom Right
                                   0.0f, 0.0f, -1.0f,  // 2 Bottom Left
                                   0.0f, 0.0f, -1.0f}, // 3 Top Left
                textureCoordinates);
}

void Mesh::destroyData()
{
    // bind();
    glDeleteBuffers(1, &mState.verticesVBO);
    glDeleteBuffers(1, &mState.EBO);
    if (mState.enabledVertexAttributes[TEX_ATTRIB_LOC]) // does it have texture coordinates?
    {
        glDeleteBuffers(1, &mState.textureCoordsVBO);
        // mState.enabledVertexAttributes[TEX_ATTRIB_LOC] = false;
    }
    if (mState.enabledVertexAttributes[NORM_ATTRIB_LOC]) // does it have normals?
    {
        glDeleteBuffers(1, &mState.normalsVBO);
        // mState.enabledVertexAttributes[NORM_ATTRIB_LOC] = false;
    }
    disableVertexAttributes();
    glDeleteVertexArrays(1, &mVAO);
}

void Mesh::enableVertexAttributes()
{
    for (GLuint i = 0; i < 16; ++i)
    {
        if (mState.enabledVertexAttributes[i])
        {
            glEnableVertexAttribArray(i);
        }
    }
}

void Mesh::disableVertexAttributes()
{
    for (GLuint i = 0; i < 16; ++i)
    {
        if (mState.enabledVertexAttributes[i])
        {
            glDisableVertexAttribArray(i);
        }
    }
}
void Mesh::constructVertices(const std::vector<GLfloat> &vertices,
                             const std::vector<GLuint> &indices)
{
    glGenBuffers(1, &mState.verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mState.verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, mSize * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    mState.enabledVertexAttributes[POS_ATTRIB_LOC] = true;

    glGenBuffers(1, &mState.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mState.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumIndices * sizeof(GLuint), indices.data(),
                 GL_STATIC_DRAW);
}
void Mesh::constructNormals(const std::vector<GLfloat> &normals)
{
    glGenBuffers(1, &mState.normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mState.normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, mSize * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    mState.enabledVertexAttributes[NORM_ATTRIB_LOC] = true;
}
void Mesh::constructTextureCoordinates(const std::vector<GLfloat> &textureCoordinates)
{
    glGenBuffers(1, &mState.textureCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mState.textureCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(GLfloat),
                 textureCoordinates.data(), GL_STATIC_DRAW);
    mState.enabledVertexAttributes[TEX_ATTRIB_LOC] = true;
}
void Mesh::constructVAO()
{
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO); // bind the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mState.EBO);
    glBindBuffer(GL_ARRAY_BUFFER, mState.verticesVBO);
    glVertexAttribPointer(POS_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (mState.enabledVertexAttributes[TEX_ATTRIB_LOC]) // does vao has texture?
    {
        glBindBuffer(GL_ARRAY_BUFFER, mState.textureCoordsVBO);
        glVertexAttribPointer(TEX_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (mState.enabledVertexAttributes[NORM_ATTRIB_LOC]) // does vao has normals?
    {
        glBindBuffer(GL_ARRAY_BUFFER, mState.normalsVBO);
        glVertexAttribPointer(NORM_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    enableVertexAttributes();

    glBindVertexArray(0); // unbind the VAO
}
} // namespace Nova