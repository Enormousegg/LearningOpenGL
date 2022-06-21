#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"


class Model
{
public:
	std::vector<Mesh> meshes;
	std::string directory;
	vector<Texture> textures_loaded;
	bool gammaCorrection;

	Model(const char* path)
	{
		loadModel(path);
	}
	void Draw(shader& shader);
private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
	unsigned int TextureFromFile(const std::string& name, const std::string dir);
};

