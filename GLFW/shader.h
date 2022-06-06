#pragma once

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class shader
{
public:
	unsigned int ID;//程序ID

	shader(const char* vertexPath, const char* fragmentPath);//构造函数读取并构建着色器

	void use();//激活程序
	//uniform工具函数
	void setBool(const string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

