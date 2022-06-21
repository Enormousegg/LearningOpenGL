#pragma once

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class shader
{
public:
	unsigned int ID;//程序ID

	shader(const char* vertexPath, const char* fragmentPath, const char* geoPath);//构造函数读取并构建着色器
	shader(const char* vertexPath, const char* fragmentPath);//构造函数读取并构建着色器

	void use();//激活程序
	//uniform工具函数
	void setBool(const string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, const float x, const float y, const float z) const;
};

