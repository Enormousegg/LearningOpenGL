#include <glad/glad.h>//GLAD的头文件包含了正确的OpenGL头文件，所以需要在其它依赖于OpenGL的头文件之前包含GLAD
#include <GLFW/glfw3.h>

#include<iostream>

#include "shader.h"
#include "stbImg/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f;

int main()
{
	//glfw初始化，以及设置
	glfwInit();//调用glfwInit函数来初始化GLFW
	/*glfwWindowHint配置GLFW，第一个参数代表选项的名称，我们可以从很多以GLFW_开头的枚举值中选择；
	第二个参数接受一个整型，用来设置这个选项的值*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//创建glfw窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JackLearnOPENGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//将回调函数绑定至window

	//glad，读取所有OPENGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//定义着色器
	shader myshader("D:/jack_work/OpenGL/MyLearning/GLFW/texture.vs", "D:/jack_work/OpenGL/MyLearning/GLFW/texture.fs");

	//建立顶点数据，并将数据发送至显卡
	/*float vertices[] = {
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};*/

	//float vertices[] = {
	//	// positions          // colors           // 纹理坐标
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	//};

	float vertices[] = {
		// positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.1f, 1.1f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.1f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.1f  // top left 
	};


	unsigned int indices[] = {
		0, 1, 3, // 第一个三角形
		1, 2, 3,// 第二个三角形
	};
	unsigned int VBO;//缓冲区对象名称的储存数组
	unsigned int VAO;//创建VAO
	unsigned int EBO;

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);//生成缓冲区对象的名称，此处生成一个缓冲对象，存入VBO数组，此处只有一个对象，故为无符号整数
	glGenVertexArrays(1, &VAO);//生成VAO对象
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);;//绑定VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO);//把生成的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //调用glBufferData函数，它会把之前定义的顶点数据复制到VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性指针,告诉OpenGL该如何解析顶点数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//启用属性，使得顶点着色器可以访问数据,0为着色器gsls代码中layout设置

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//设置颜色属性
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//设置纹理属性
	glEnableVertexAttribArray(2);

	//生成纹理
	unsigned int texture0;
	glGenTextures(1, &texture0);//生成纹理对象
	glBindTexture(GL_TEXTURE_2D, texture0);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 设置环绕参数，GL_REPEAT (默认重复纹理图像)，此处对S轴设置
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//对T轴
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//设置过滤参数，此处对缩小模式下设置为多级渐远纹理
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大模式下设置为线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 设置环绕参数，GL_REPEAT (默认重复纹理图像)，此处对S轴设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//对T轴
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//设置过滤参数，此处对缩小模式下设置为多级渐远纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//放大模式下设置为线性过滤
	//加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);//加载图片时，翻转y轴
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);//加载纹理图片
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 设置环绕参数，GL_REPEAT (默认重复纹理图像)，此处对S轴设置
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//对T轴
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//设置过滤参数
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大模式下设置为线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 设置环绕参数，GL_REPEAT (默认重复纹理图像)，此处对S轴设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//对T轴
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//设置过滤参数，此处对缩小模式下设置为多级渐远纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//放大模式下设置为线性过滤
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	////定义旋转矩阵
	//glm::mat4 trans;
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	myshader.use();
	glUniform1i(glGetUniformLocation(myshader.ID, "texture0"), 0);
	myshader.setInt("texture1", 1);
	////将旋转矩阵赋值给着色器中的uniform变量
	//unsigned int transformLoc = glGetUniformLocation(myshader.ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//glBindBuffer(GL_ARRAY_BUFFER, 0);//设置GL_ARRAY_BUFFER的绑定为空

	//glBindVertexArray(0);//解绑VAO

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//设置线框模式绘制

	while (!glfwWindowShouldClose(window))//渲染循环
	{
		processInput(window);//检测是否按下esc按键，若是将glfwWindowShouldClose设置为true

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清屏所用颜色
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);//激活纹理对象，若纹理对象只有一个则其自动激活，一般有16个，编号0-15
		glBindTexture(GL_TEXTURE_2D, texture0);//绑定纹理对象
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		myshader.use();//激活新创建的程序对象

		myshader.setFloat("mixValue", mixValue);

		//double  timeValue = glfwGetTime();//随时间渐变的颜色
		//float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.5f, 1.0f);

		//时间渐变旋转
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//将旋转矩阵赋值给着色器中的uniform变量
		unsigned int transformLoc = glGetUniformLocation(myshader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//带索引的渲染图元函数
		//glDrawArrays(GL_TRIANGLES, 0, 3);//使用当前激活的着色器，之前定义的顶点属性配置，和VBO的顶点数据（通过VAO间接绑定）来绘制图元

		//第二个图形
		trans = glm::mat4(1.0f); // reset it to identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]); // this time take the matrix value array's first element as its memory pointer value

		// now with the uniform matrix being replaced with new transformations, draw it again.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glUseProgram(shaderProgram2);//激活新创建的程序对象
		//glBindVertexArray(VAO1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//因为使用的是双缓存GLUT_DOUBLE，所以这里必须要交换缓存才会显示，它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上，单缓存存在闪烁问题
		glfwPollEvents(); //glfwPollEvents函数检查有没有触发什么事件（比如键盘输入）、更新窗口状态，并调用对应的回调函数
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

/*窗口回调函数*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//输入控制函数
void processInput(GLFWwindow* window)
{
	//检查用户是否按下了返回键(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//glfwGetKey函数需要一个窗口以及一个按键作为输入
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
}