#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>
#include <string>


#include "shaderClass.h"

#define VERTEX_RGB 0
#define VERTEX_NORMAL_UV 1

class Object {
public:	
	Object();

	void Draw();

	static Object loadglTF(const char* path);

	static Object sphere(glm::vec4 center, float radius, unsigned int resolution);

	static Object circle(glm::vec4 center, float radius, unsigned int resolution);

	static Object greatCircle(glm::vec4 pointA, glm::vec4 pointB, unsigned int resolution);
	static Object greatArc(glm::vec4 start, glm::vec4 end, unsigned int resolution);
private:
	unsigned int vertex_data_type = VERTEX_RGB;
	std::vector<GLfloat> vertex_data;

	unsigned int element_type = GL_TRIANGLES;
	std::vector<GLuint> indices;

	glm::mat4 model_matrix = glm::mat4(1.0f);
};