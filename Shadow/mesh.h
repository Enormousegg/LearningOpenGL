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
	glm::vec3 Position;//��������
	glm::vec3 Normal;//���㷨��
	glm::vec2 TexCoords;//��������
	glm::vec3 Tangent;//����
	glm::vec3 Bitangent;// ������
	int m_BoneIDs[MAX_BONE_INFLUENCE];//bone indexes which will influence this vertex
	float m_Weights[MAX_BONE_INFLUENCE];//weights from each bone
};

struct Texture {
	unsigned int id;//����id
	std::string type;//��������
	std::string path;//����·��
};


class Mesh
{
public:
	//��������
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;
	//����
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		:vertices(vertices), indices(indices), textures(textures)
	{
		setupMesh();
	}
	void Draw(shader shader);
private:
	//��Ⱦ����
	unsigned int VBO, EBO;
	//����
	void setupMesh();
};

