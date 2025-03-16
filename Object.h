#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"

#define VERTEX_RGB 0
#define VERTEX_NORMAL_UV 1

class Object {
public:
	Object();

	void Draw(GLint modelLoc);

	void loadMesh(const char* path);

	void sphere(glm::vec4 center, float radius, unsigned int resolution);

	void circle(glm::vec4 center, float radius, unsigned int resolution);

	void greatCircle(glm::vec4 pointA, glm::vec4 pointB, unsigned int resolution);
	void greatArc(glm::vec4 start, glm::vec4 end);

	void debugPrintVertexData(size_t start, size_t end);

private:
	void setupBuffers();
	void clear();

	void linkToBufferObjects();

	unsigned int vertex_data_type = VERTEX_RGB;
	std::vector<GLfloat> vertex_data;

	unsigned int element_type = GL_TRIANGLES;
	std::vector<GLuint> indices;

	VAO vao;
	VBO vbo;
	EBO ebo;

	glm::mat4 transform = glm::mat4(1.0f);
};