#include <glad/glad.h>//GLAD��ͷ�ļ���������ȷ��OpenGLͷ�ļ���������Ҫ������������OpenGL��ͷ�ļ�֮ǰ����GLAD
#include <GLFW/glfw3.h>

#include<iostream>

#include "shader.h"
#include "stbImg/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f;

int main()
{
	//glfw��ʼ�����Լ�����
	glfwInit();//����glfwInit��������ʼ��GLFW
	/*glfwWindowHint����GLFW����һ����������ѡ������ƣ����ǿ��ԴӺܶ���GLFW_��ͷ��ö��ֵ��ѡ��
	�ڶ�����������һ�����ͣ������������ѡ���ֵ*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//����glfw����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JackLearnOPENGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//���ص���������window

	//glad����ȡ����OPENGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//������ɫ��
	shader myshader("D:/jack_work/OpenGL/MyLearning/GLFW/texture.vs", "D:/jack_work/OpenGL/MyLearning/GLFW/texture.fs");

	//�����������ݣ��������ݷ������Կ�
	/*float vertices[] = {
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};*/

	//float vertices[] = {
	//	// positions          // colors           // ��������
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
		0, 1, 3, // ��һ��������
		1, 2, 3,// �ڶ���������
	};
	unsigned int VBO;//�������������ƵĴ�������
	unsigned int VAO;//����VAO
	unsigned int EBO;

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);//���ɻ�������������ƣ��˴�����һ��������󣬴���VBO���飬�˴�ֻ��һ�����󣬹�Ϊ�޷�������
	glGenVertexArrays(1, &VAO);//����VAO����
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);;//��VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO);//�����ɵĻ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //����glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ö�������ָ��,����OpenGL����ν�����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//�������ԣ�ʹ�ö�����ɫ�����Է�������,0Ϊ��ɫ��gsls������layout����

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//������ɫ����
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//������������
	glEnableVertexAttribArray(2);

	//��������
	unsigned int texture0;
	glGenTextures(1, &texture0);//�����������
	glBindTexture(GL_TEXTURE_2D, texture0);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// ���û��Ʋ�����GL_REPEAT (Ĭ���ظ�����ͼ��)���˴���S������
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//��T��
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//���ù��˲������˴�����Сģʽ������Ϊ�༶��Զ����
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ�ģʽ������Ϊ���Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// ���û��Ʋ�����GL_REPEAT (Ĭ���ظ�����ͼ��)���˴���S������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//��T��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//���ù��˲������˴�����Сģʽ������Ϊ�༶��Զ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//�Ŵ�ģʽ������Ϊ���Թ���
	//���ز���������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);//����ͼƬʱ����תy��
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);//��������ͼƬ
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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// ���û��Ʋ�����GL_REPEAT (Ĭ���ظ�����ͼ��)���˴���S������
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//��T��
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//���ù��˲���
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ�ģʽ������Ϊ���Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// ���û��Ʋ�����GL_REPEAT (Ĭ���ظ�����ͼ��)���˴���S������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//��T��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//���ù��˲������˴�����Сģʽ������Ϊ�༶��Զ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//�Ŵ�ģʽ������Ϊ���Թ���
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

	////������ת����
	//glm::mat4 trans;
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	myshader.use();
	glUniform1i(glGetUniformLocation(myshader.ID, "texture0"), 0);
	myshader.setInt("texture1", 1);
	////����ת����ֵ����ɫ���е�uniform����
	//unsigned int transformLoc = glGetUniformLocation(myshader.ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//glBindBuffer(GL_ARRAY_BUFFER, 0);//����GL_ARRAY_BUFFER�İ�Ϊ��

	//glBindVertexArray(0);//���VAO

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����߿�ģʽ����

	while (!glfwWindowShouldClose(window))//��Ⱦѭ��
	{
		processInput(window);//����Ƿ���esc���������ǽ�glfwWindowShouldClose����Ϊtrue

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//��������������ɫ
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);//��������������������ֻ��һ�������Զ����һ����16�������0-15
		glBindTexture(GL_TEXTURE_2D, texture0);//���������
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		myshader.use();//�����´����ĳ������

		myshader.setFloat("mixValue", mixValue);

		//double  timeValue = glfwGetTime();//��ʱ�佥�����ɫ
		//float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.5f, 1.0f);

		//ʱ�佥����ת
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//����ת����ֵ����ɫ���е�uniform����
		unsigned int transformLoc = glGetUniformLocation(myshader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//����������ȾͼԪ����
		//glDrawArrays(GL_TRIANGLES, 0, 3);//ʹ�õ�ǰ�������ɫ����֮ǰ����Ķ����������ã���VBO�Ķ������ݣ�ͨ��VAO��Ӱ󶨣�������ͼԪ

		//�ڶ���ͼ��
		trans = glm::mat4(1.0f); // reset it to identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]); // this time take the matrix value array's first element as its memory pointer value

		// now with the uniform matrix being replaced with new transformations, draw it again.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glUseProgram(shaderProgram2);//�����´����ĳ������
		//glBindVertexArray(VAO1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//��Ϊʹ�õ���˫����GLUT_DOUBLE�������������Ҫ��������Ż���ʾ��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϣ������������˸����
		glfwPollEvents(); //glfwPollEvents���������û�д���ʲô�¼�������������룩�����´���״̬�������ö�Ӧ�Ļص�����
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

/*���ڻص�����*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//������ƺ���
void processInput(GLFWwindow* window)
{
	//����û��Ƿ����˷��ؼ�(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//glfwGetKey������Ҫһ�������Լ�һ��������Ϊ����
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