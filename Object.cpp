#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "Object.h"

Object::Object(){
}

void Object::Delete() {
    if (uses_texture) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    };
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
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
    
    vao.linkAttrib(vbo, 0, 4, GL_FLOAT, 10 * sizeof(GLfloat), (void*)0);
    // Normal attribute (x,y,z,w) - 4 floats
    vao.linkAttrib(vbo, 1, 4, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
    // Texture coordinate attribute (u,v) - 2 floats
    vao.linkAttrib(vbo, 2, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
    // Now bind the EBO after setting up the attributes
    ebo.Bind();
    ebo.SetBufferData(indices.data(), indexSize);
}

void Object::loadTexture(const std::string& path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {

        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        uses_texture = true;
        texture_path = path;
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
        uses_texture = false;
    }
    stbi_image_free(data);
}


void Object::Draw(GLint modelLoc, GLint samplerLoc, GLint textureboolLoc, GLint diffuseLoc) {
    if (uses_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else {
        glUniform3f(diffuseLoc, diffuse_color.r, diffuse_color.g, diffuse_color.b);
    }
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform1i(samplerLoc, 0);
    glUniform1i(textureboolLoc, uses_texture);
    vao.Bind();
    glDrawElements(element_type, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

void Object::loadMesh(const char* path) {
    clear();
    element_type = GL_TRIANGLES;

    bool already_loaded_a_texture = false;

	Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate |aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

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
        if (!already_loaded_a_texture) {
            aiMaterial* material = pScene->mMaterials[mesh->mMaterialIndex];

            //LOADTEXTURES
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString str;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                std::filesystem::path texPath(str.C_Str());
                std::string fileName = texPath.filename().string();
                std::string texturePath = "Textures\\" + fileName;
                uses_texture = true;
                loadTexture(texturePath);

            }
            else {
                aiColor3D color(1.0f, 1.0f, 1.0f);
                material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                diffuse_color = glm::vec3(color.r, color.g, color.b); // Store fallback
                uses_texture = false;
            }
        }
    }

    linkToBufferObjects();
    //printf("Loaded mesh with %du vertices and %du indices.", vertex_data.size() / 10, indices.size());
}

void Object::greatArc(glm::vec4 start, glm::vec4 end, glm::vec3 color) {
    clear();
    if (uses_texture) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    uses_texture = false;
    diffuse_color = color;
    element_type = GL_LINES;

    vertex_data.push_back(start.x);
    vertex_data.push_back(start.y);
    vertex_data.push_back(start.z);
    vertex_data.push_back(start.w);
    //Normals = vertex for great circles
    vertex_data.push_back(start.x);
    vertex_data.push_back(start.y);
    vertex_data.push_back(start.z);
    vertex_data.push_back(start.w);
    //Junk UV coords
    vertex_data.push_back(0.f);
    vertex_data.push_back(0.f);

    vertex_data.push_back(end.x);
    vertex_data.push_back(end.y);
    vertex_data.push_back(end.z);
    vertex_data.push_back(end.w);
    //Normals = vertex for great circles
    vertex_data.push_back(end.x);
    vertex_data.push_back(end.y);
    vertex_data.push_back(end.z);
    vertex_data.push_back(end.w);
    //Junk UV coords
    vertex_data.push_back(0.f);
    vertex_data.push_back(0.f);

    indices.push_back(0);
    indices.push_back(1);
    linkToBufferObjects();
}

void Object::debugPrintVertexData(size_t start, size_t end) {
    std::copy(vertex_data.begin() + start, vertex_data.begin() + end,
        std::ostream_iterator<float>(std::cout, ", "));
    std::cout << std::endl;
}