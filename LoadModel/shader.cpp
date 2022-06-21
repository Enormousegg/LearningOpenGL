#include "shader.h"

shader::shader(const char* vertexPath, const char* fragmentPath)
{
	//1.���ļ�·���л�ȡ����/Ƭ����ɫ��
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		//���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		//��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//�ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		//ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	
	const char* vShaderCode = vertexCode.c_str();//c��������string����stringת��Ϊָ�����ָ�볣����Ϊconst
	const char* fShaderCode = fragmentCode.c_str();

	//������ɫ��
	unsigned int vertex, fragment;
	int success;
	char infolog[512];

	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//��ӡ���ܴ��ڵı������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
	}
	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;
	}

	//��ɫ������
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//��ӡ���Ӵ���
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << endl;
	}

	//ɾ����ɫ���������Ѿ����ӵ����ǵĳ����У��Ѿ�������Ҫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void shader::use()
{
	glUseProgram(ID);
}

void shader::setBool(const string& name, bool value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setMat4(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1 ,GL_FALSE ,glm::value_ptr(value));
	//glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void shader::setVec3(const std::string& name, const float x, const float y, const float z) const
{
	glm::vec3 tmp = glm::vec3(x, y, z);
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &tmp[0]);
}

/*ʹ��ʾ��
Shader ourShader("path/to/shaders/shader.vs", "path/to/shaders/shader.fs");
...
while(...)
{
	ourShader.use();
	ourShader.setFloat("someUniform", 1.0f);
	DrawStuff();
}*/

/*//������ɫ����ش���˵��-����
	unsigned int vertexShader;//��ɫ����������
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//������ɫ�����󲢸�ֵ��GL_VERTEX_SHADER��ʾ��ɫ������Ϊ������ɫ��
	glShaderSource(vertexShader, 1, &vertexShader1Source, NULL);//��ɫ��Դ�븽�ӵ���ɫ�������ϣ��ڶ�������ΪԴ���ַ�������
	glCompileShader(vertexShader);//����

	//�������Ƿ�ɹ�
	int  success;
	char infoLog[512];//���������ַ�����
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//�����Ƿ����ɹ�
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Ƭ����ɫ������
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//GL_FRAGMENT_SHADER��ʾ����ΪƬ����ɫ��
	glShaderSource(fragmentShader, 1, &fragmentShader4Source, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//�����Ƿ����ɹ�
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//��������ɫ����������Ϊһ����ɫ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();//������ɫ���������
	//����ɫ����������ɫ���������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//������ɫ��

	//�ж������Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error:shaderProgram linked failed" << std::endl;
	}
	//ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/