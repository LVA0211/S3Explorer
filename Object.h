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
	Object(const std::string& name);

	Object() = default;

	~Object();

	std::string name;

	void Delete();

	void Draw(GLint modelLoc, GLint model_scaleLoc, GLint samplerLoc, GLint texture_boolLoc, GLint diffuseLoc, GLint project_normal_to_sphere_boolLoc);

	void loadMesh(const char* path);
	/// <summary>
	/// Set the vertices+indices manually.
	/// </summary>
	/// <param name="vertex_data">Vertex data in the format: vert position (4 floats), vert normal (4 floats), UV coords (2 floats)</param>
	/// <param name="element_type">OpenGL element type to draw. E.g. GL_TRIANGLES</param>
	/// <param name="indices">Indices as an array of uints</param>
	/// <param name="diffuse_color">Color of mesh</param>
	/// <param name="project_normals_to_sphere">set to true if normals exist in a 3D subspace (w=0), false otherwise</param>
	void fromArray(std::vector<GLfloat>&& vertex_data, unsigned int element_type, std::vector<GLuint>&& indices, glm::vec3 diffuse_color, bool project_normals_to_sphere);

	void sphere(glm::vec4 center, float radius, unsigned int resolution);

	void circle(glm::vec4 center, float radius, unsigned int resolution);

	void greatCircle(glm::vec4 pointA, glm::vec4 pointB, unsigned int resolution);
	void greatArc(glm::vec4 start, glm::vec4 end, glm::vec3 color);

	void debugPrintVertexData(size_t start, size_t end);


	glm::vec4 scale = glm::vec4(1.f);
	glm::mat4 transform = glm::mat4(1.0f);
	
	void rotateGlobal(glm::vec4 direction, float angle);
	void rotateBasisAxis(int axis1, int axis2, float angle);

	glm::vec3 spherical_coords = glm::vec3(0.f);
	void setSphericalCoordinates(float a, float b, float c);
	void updateTransformFromSphericalCoordinates();
	void updateSphericalCoordinatesFromTransform();

	void setScaleForUnitSphericalSphere(float sphericalRadius);
	
	std::function<void(Object&, float)> animationFunc;
	bool visible = true;

private:

	void clear();
	void linkToBufferObjects();
	void loadTexture(const std::string& path);


	bool uses_texture = false;
	bool project_normals_to_sphere = false;
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