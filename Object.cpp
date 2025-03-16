#include "Object.h"

Object::Object(){
}

void Object::clear() {
    vertex_data.clear();
    indices.clear();
}

void Object::linkToBufferObjects() {
    vao.Bind();
    vbo.Bind();
    size_t vertexSize = vertex_data.size() * sizeof(GLfloat);
    size_t indexSize = indices.size() * sizeof(GLuint);
    vbo.SetBufferData(vertex_data.data(), vertexSize);

    // Set up vertex attributes
    // Position attribute (x,y,z,w) - 4 floats
    vao.linkAttrib(vbo, 0, 4, GL_FLOAT, 10 * sizeof(GLfloat), (void*)0);

    // Normal attribute (x,y,z,w) - 4 floats
    vao.linkAttrib(vbo, 1, 4, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

    // Texture coordinate attribute (u,v) - 2 floats
    vao.linkAttrib(vbo, 2, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));

    // Now bind the EBO after setting up the attributes
    ebo.Bind();
    ebo.SetBufferData(indices.data(), indexSize);
}

void Object::Draw(GLint modelLoc) {
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

    vao.Bind();
    glDrawElements(element_type, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

void Object::loadMesh(const char* path) {
    clear();

	Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate |aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (!pScene || !pScene->mRootNode) {
        printf("Error parsing '%s': '%s'\n", path, importer.GetErrorString());
        return;
    }

    for (unsigned int meshIndex = 0; meshIndex < pScene->mNumMeshes; meshIndex++) {
        aiMesh* mesh = pScene->mMeshes[meshIndex];

        // Loop through all vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            // Position
            vertex_data.push_back(mesh->mVertices[i].x);
            vertex_data.push_back(mesh->mVertices[i].y);
            vertex_data.push_back(mesh->mVertices[i].z);
            vertex_data.push_back(1.0f);

            // Normal (if available)
            if (mesh->HasNormals()) {
                vertex_data.push_back(mesh->mNormals[i].x);
                vertex_data.push_back(mesh->mNormals[i].y);
                vertex_data.push_back(mesh->mNormals[i].z);
                vertex_data.push_back(0.0f);
            }
            else {
                vertex_data.push_back(0.0f);
                vertex_data.push_back(0.0f);
                vertex_data.push_back(0.0f);
                vertex_data.push_back(0.0f);
            }

            // Texture Coordinates (if available, using first set)
            if (mesh->HasTextureCoords(0)) {
                vertex_data.push_back(mesh->mTextureCoords[0][i].x);
                vertex_data.push_back(mesh->mTextureCoords[0][i].y);
            }
            else {
                vertex_data.push_back(0.0f);
                vertex_data.push_back(0.0f);
            }
        }

        // Loop through faces (Assimp guarantees triangulated faces)
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }
        //printf("mNumFaces: %du, mNumVertices: %du.\n",mesh->mNumFaces,mesh->mNumVertices);
    }

    linkToBufferObjects();
    //printf("Loaded mesh with %du vertices and %du indices.", vertex_data.size() / 10, indices.size());
}

void Object::greatArc(glm::vec4 start, glm::vec4 end) {
}

void Object::debugPrintVertexData(size_t start, size_t end) {
    std::copy(vertex_data.begin() + start, vertex_data.begin() + end,
        std::ostream_iterator<float>(std::cout, ", "));
    std::cout << std::endl;
}