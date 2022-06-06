#pragma once

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class shader
{
public:
	unsigned int ID;//����ID

	shader(const char* vertexPath, const char* fragmentPath);//���캯����ȡ��������ɫ��

	void use();//�������
	//uniform���ߺ���
	void setBool(const string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

