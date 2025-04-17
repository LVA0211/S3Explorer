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
#include <filesystem>
#include <functional>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"

#define VERTEX_RGB 0
#define VERTEX_NORMAL_UV 1

class Object {
public:
	Object();
	~Object();

	void Delete();

	void Draw(GLint modelLoc, GLint modelScaleLoc, GLint samplerLoc, GLint textureboolLoc, GLint diffuseLoc);


	void loadMesh(const char* path);
	void fromArray(std::vector<GLfloat>&& vertex_data, unsigned int element_type, std::vector<GLuint>&& indices, glm::vec3 diffuse_color);

	void sphere(glm::vec4 center, float radius, unsigned int resolution);

	void circle(glm::vec4 center, float radius, unsigned int resolution);

	void greatCircle(glm::vec4 pointA, glm::vec4 pointB, unsigned int resolution);
	void greatArc(glm::vec4 start, glm::vec4 end, glm::vec3 color);

	void debugPrintVertexData(size_t start, size_t end);


	glm::vec4 scale = glm::vec4(1.f);
	glm::mat4 transform = glm::mat4(1.0f);

	void setTransformFromSpherical(float a, float b, float c);
	void setScaleForUnitSphericalSphere(float sphericalRadius);
	
	std::function<void(Object&, float)> animationFunc;
private:
	void setupBuffers();
	void clear();
	void linkToBufferObjects();
	void loadTexture(const std::string& path);

	bool uses_texture = false;
	glm::vec3 diffuse_color = glm::vec3(0.f);
	//(X,Y,Z,W, nX,nY,nZ,nW, U,V)
	std::vector<GLfloat> vertex_data;

	unsigned int element_type = GL_TRIANGLES;
	std::vector<GLuint> indices;

	GLuint textureID = 0;
	std::string texture_path;

	VAO vao;
	VBO vbo;
	EBO ebo;
};