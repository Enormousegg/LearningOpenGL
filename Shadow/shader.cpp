#include "shader.h"

shader::shader(const char* vertexPath, const char* fragmentPath, const char* geoPath)
{
	//1.从文件路径中获取顶点/片段着色器
	string vertexCode;
	string fragmentCode;
	string geoCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream geoFile;

	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	geoFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		geoFile.open(geoPath);
		stringstream vShaderStream, fShaderStream,geoStream;
		//读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		geoStream << geoFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		geoFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geoCode = geoStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	
	const char* vShaderCode = vertexCode.c_str();//c语言中无string，将string转化为指向其的指针常量，为const
	const char* fShaderCode = fragmentCode.c_str();
	const char* geomeCode = geoCode.c_str();

	//编译着色器
	unsigned int vertex, fragment,geome;
	int success;
	char infolog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//打印可能存在的编译错误
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
	}
	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;
	}
	//几何着色器
	geome = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geome, 1, &geomeCode, NULL);
	glCompileShader(geome);
	glGetShaderiv(geome, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geome, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;
	}

	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geome);
	glLinkProgram(ID);
	//打印连接错误
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << endl;
	}

	//删除着色器，它们已经连接到我们的程序中，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geome);
}

shader::shader(const char* vertexPath, const char* fragmentPath)
{
	//1.从文件路径中获取顶点/片段着色器
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	const char* vShaderCode = vertexCode.c_str();//c语言中无string，将string转化为指向其的指针常量，为const
	const char* fShaderCode = fragmentCode.c_str();

	//编译着色器
	unsigned int vertex, fragment;
	int success;
	char infolog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//打印可能存在的编译错误
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
	}
	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;
	}

	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//打印连接错误
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << endl;
	}

	//删除着色器，它们已经连接到我们的程序中，已经不再需要了
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

void shader::setMat4(const std::string& name, const glm::mat4& value) const
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

/*使用示例
Shader ourShader("path/to/shaders/shader.vs", "path/to/shaders/shader.fs");
...
while(...)
{
	ourShader.use();
	ourShader.setFloat("someUniform", 1.0f);
	DrawStuff();
}*/

/*//顶点着色器相关代码说明-备份
	unsigned int vertexShader;//着色器对象名称
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建着色器对象并赋值，GL_VERTEX_SHADER表示着色器类型为顶点着色器
	glShaderSource(vertexShader, 1, &vertexShader1Source, NULL);//着色器源码附加到着色器对象上，第二个参数为源码字符串数量
	glCompileShader(vertexShader);//编译

	//检测编译是否成功
	int  success;
	char infoLog[512];//储存错误的字符数组
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//检验是否编译成功
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器定义
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//GL_FRAGMENT_SHADER表示类型为片段着色器
	glShaderSource(fragmentShader, 1, &fragmentShader4Source, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//检验是否编译成功
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//将所有着色器对象链接为一个着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();//创建着色器程序对象
	//将着色器连接至着色器程序对象
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//连接着色器

	//判断连接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error:shaderProgram linked failed" << std::endl;
	}
	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/