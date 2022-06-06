#pragma once

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

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
	unsigned int ID;//����ID

	shader(const char* vertexPath, const char* fragmentPath);//���캯����ȡ��������ɫ��

	void use();//�������
	//uniform���ߺ���
	void setBool(const string& name, const bool value) const;
	void setInt(const std::string& name, const int value) const;
	void setFloat(const std::string& name, const float value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
};

