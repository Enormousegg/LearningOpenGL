#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stbImg/stb_image.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;//顶点坐标
	glm::vec3 Normal;//顶点法线
	glm::vec2 TexCoords;//纹理坐标
	glm::vec3 Tangent;//切线
	glm::vec3 Bitangent;// 副切线
	int m_BoneIDs[MAX_BONE_INFLUENCE];//bone indexes which will influence this vertex
	float m_Weights[MAX_BONE_INFLUENCE];//weights from each bone
};

struct Texture {
	unsigned int id;//纹理id
	std::string type;//纹理类型
	std::string path;//纹理路径
};


class Mesh
{
public:
	//网格数据
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;
	//函数
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		:vertices(vertices), indices(indices), textures(textures)
	{
		setupMesh();
	}
	void Draw(shader shader);
private:
	//渲染数据
	unsigned int VBO, EBO;
	//函数
	void setupMesh();
};

