#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "stbImg/stb_image.h"

#include "shader.h"
#include "camera.h"
#include "Model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(const std::string path,vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);//开启深度测试
    glDepthFunc(GL_LESS);

    // build and compile shaders
    // -------------------------
    shader myshader("object.vs", "object.fs");
    shader cubeshader("cube.vs", "cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    vector<std::string> faces
    {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    //sky VAO
    unsigned int skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // load textures
    // -------------
    //unsigned int cubeTexture = loadTexture("D:/jack_work/OpenGL/pic/container.jpg");
    unsigned int cubemapTexture = loadCubemap("D:/jack_work/OpenGL/pic/skybox/",faces);
    Model ourModel("D:/jack_work/OpenGL/model/robot/nanosuit.obj");

    // shader configuration
    // --------------------
    myshader.use();
    myshader.setInt("skybox", 0);
    cubeshader.use();
    cubeshader.setInt("skybox", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set uniforms
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // cubes
        myshader.use();
        myshader.setMat4("view", view);
        myshader.setMat4("projection", projection);
        myshader.setVec3("cameraPos", camera.Position);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        myshader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -3.0f, 2.0f));
        float scale = 0.5;
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        myshader.setMat4("model", model);
        ourModel.Draw(myshader);

        //sky
        glDepthFunc(GL_LEQUAL);
        cubeshader.use();
        scale = 10;
        view = glm::scale(view, glm::vec3(scale, scale, scale));
        cubeshader.setMat4("view", view);
        cubeshader.setMat4("projection", projection);
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(const std::string path,vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load((path+faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <map>
//
//#include "stbImg/stb_image.h"
//
//#include "shader.h"
//#include "camera.h"
//#include "Model.h"
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//unsigned int loadTexture(const char* path);
//unsigned int loadTextureAlpha(const char* path);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = (float)SCR_WIDTH / 2.0;
//float lastY = (float)SCR_HEIGHT / 2.0;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);//开启深度测试
//    glDepthFunc(GL_LESS);
//
//    // build and compile shaders
//    // -------------------------
//    shader myshader("object.vs", "object.fs");
//    shader screenshader("bound.vs", "bound.fs");
//
//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
//    float cubeVertices[] = {
//        // Back face
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//        // Front face
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//        // Left face
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//        // Right face
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
//        // Bottom face
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//        // Top face
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
//    };
//    float planeVertices[] = {
//        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//         5.0f, -0.501f,  5.0f,  2.0f, 0.0f,//防止深度冲突，设置平面与正方体有0.001f的间隙
//        -5.0f, -0.501f,  5.0f,  0.0f, 0.0f,
//        -5.0f, -0.501f, -5.0f,  0.0f, 2.0f,
//
//         5.0f, -0.501f,  5.0f,  2.0f, 0.0f,
//        -5.0f, -0.501f, -5.0f,  0.0f, 2.0f,
//         5.0f, -0.501f, -5.0f,  2.0f, 2.0f
//    };
//    // cube VAO
//    unsigned int cubeVAO, cubeVBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glBindVertexArray(0);
//    // plane VAO
//    unsigned int planeVAO, planeVBO;
//    glGenVertexArrays(1, &planeVAO);
//    glGenBuffers(1, &planeVBO);
//    glBindVertexArray(planeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glBindVertexArray(0);
//    //quad VAO
//    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//        // positions   // texCoords
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f,  0.0f, 0.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//
//        -1.0f,  1.0f,  0.0f, 1.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f
//    };
//    unsigned int quadVAO, quadVBO;
//    glGenVertexArrays(1, &quadVAO);
//    glGenBuffers(1, &quadVBO);
//    glBindVertexArray(quadVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//
//    // load textures
//    // -------------
//    unsigned int cubeTexture = loadTexture("D:/jack_work/OpenGL/pic/container.jpg");
//    unsigned int floorTexture = loadTexture("D:/jack_work/OpenGL/pic/metal.jpg");
//
//    // shader configuration
//    // --------------------
//    myshader.use();
//    myshader.setInt("texture1", 0);
//
//    screenshader.use();
//    screenshader.setInt("screenTexture", 0);
//
//    unsigned int framebuffer;
//    glGenFramebuffers(1, &framebuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//    // create a color attachment texture
//    unsigned int textureColorbuffer;
//    glGenTextures(1, &textureColorbuffer);
//    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
//    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
//    unsigned int rbo;
//    glGenRenderbuffers(1, &rbo);
//    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
//    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//        glEnable(GL_DEPTH_TEST);
//
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // set uniforms
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        myshader.use();
//        myshader.setMat4("view", view);
//        myshader.setMat4("projection", projection);
//
//        // floor
//        glBindVertexArray(planeVAO);
//        glBindTexture(GL_TEXTURE_2D, floorTexture);//0号纹理绑定金属纹理图
//        myshader.setMat4("model", glm::mat4(1.0f));
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        // cubes
//        glBindVertexArray(cubeVAO);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, cubeTexture);//绑定大理石贴图
//        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//        myshader.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//        myshader.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glBindVertexArray(0);
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
//        // clear all relevant buffers
//        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        screenshader.use();
//        glBindVertexArray(quadVAO);
//        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteVertexArrays(1, &planeVAO);
//    glDeleteVertexArrays(1, &quadVAO);
//    glDeleteBuffers(1, &cubeVBO);
//    glDeleteBuffers(1, &planeVBO);
//    glDeleteBuffers(1, &quadVBO);
//
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
//
//unsigned int loadTextureAlpha(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}


//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <map>
//
//#include "stbImg/stb_image.h"
//
//#include "shader.h"
//#include "camera.h"
//#include "Model.h"
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//unsigned int loadTexture(const char* path);
//unsigned int loadTextureAlpha(const char* path);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = (float)SCR_WIDTH / 2.0;
//float lastY = (float)SCR_HEIGHT / 2.0;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);//开启深度测试
//    glDepthFunc(GL_LESS);
//    glEnable(GL_BLEND);//开启混合
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    //glEnable(GL_CULL_FACE);//开启面剔除
//
//    // build and compile shaders
//    // -------------------------
//    shader myshader("object.vs", "object.fs");
//    //shader shaderSingleColor("floor.vs", "floor.fs");
//
//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
//    float cubeVertices[] = {
//        // Back face
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//        // Front face
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//        // Left face
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//        // Right face
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
//        // Bottom face
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//        // Top face
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
//    };
//    float planeVertices[] = {
//        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//         5.0f, -0.501f,  5.0f,  2.0f, 0.0f,//防止深度冲突，设置平面与正方体有0.001f的间隙
//        -5.0f, -0.501f,  5.0f,  0.0f, 0.0f,
//        -5.0f, -0.501f, -5.0f,  0.0f, 2.0f,
//
//         5.0f, -0.501f,  5.0f,  2.0f, 0.0f,
//        -5.0f, -0.501f, -5.0f,  0.0f, 2.0f,
//         5.0f, -0.501f, -5.0f,  2.0f, 2.0f
//    };
//    // cube VAO
//    unsigned int cubeVAO, cubeVBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glBindVertexArray(0);
//    // plane VAO
//    unsigned int planeVAO, planeVBO;
//    glGenVertexArrays(1, &planeVAO);
//    glGenBuffers(1, &planeVBO);
//    glBindVertexArray(planeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glBindVertexArray(0);
//    //cao VAO
//    vector<glm::vec3> vegetation
//    {
//        glm::vec3(-1.5f, 0.0f, -0.48f),
//        glm::vec3(1.5f, 0.0f, 0.51f),
//        glm::vec3(0.0f, 0.0f, 0.7f),
//        glm::vec3(-0.3f, 0.0f, -2.3f),
//        glm::vec3(0.5f, 0.0f, -0.6f)
//    };
//
//    float caoVertices[] = {
//        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
//        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
//        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//
//        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
//    };
//    unsigned int caoVAO, caoVBO;
//    glGenVertexArrays(1, &caoVAO);
//    glGenBuffers(1, &caoVBO);
//    glBindVertexArray(caoVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, caoVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(caoVertices), &caoVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
//
//
//    // load textures
//    // -------------
//    unsigned int cubeTexture = loadTexture("D:/jack_work/OpenGL/MyLearning/HighLight/marble.jpg");
//    unsigned int floorTexture = loadTexture("D:/jack_work/OpenGL/pic/metal.jpg");
//    unsigned int caoTexture = loadTextureAlpha("D:/jack_work/OpenGL/pic/blending_transparent_window.png");
//
//    // shader configuration
//    // --------------------
//    myshader.use();
//    myshader.setInt("texture1", 0);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        std::map<float, glm::vec3> sorted;
//        for (unsigned int i = 0; i < vegetation.size(); i++)
//        {
//            float distance = glm::length(camera.Position - vegetation[i]);
//            sorted[distance] = vegetation[i];
//        }
//
//        // render
//        // ------
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!
//
//        // set uniforms
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        myshader.use();
//        myshader.setMat4("view", view);
//        myshader.setMat4("projection", projection);
//
//        // floor
//        glBindVertexArray(planeVAO);
//        glBindTexture(GL_TEXTURE_2D, floorTexture);//0号纹理绑定金属纹理图
//        myshader.setMat4("model", glm::mat4(1.0f));
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        glBindVertexArray(0);
//
//        glEnable(GL_CULL_FACE);
//        // cubes
//        glBindVertexArray(cubeVAO);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, cubeTexture);//绑定大理石贴图
//        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//        myshader.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//        myshader.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        glDisable(GL_CULL_FACE);
//
//        glBindVertexArray(caoVAO);
//        glBindTexture(GL_TEXTURE_2D, caoTexture);//绑定草纹理
//        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
//        {
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, it->second);
//            myshader.setMat4("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//        }
//
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteVertexArrays(1, &planeVAO);
//    glDeleteVertexArrays(1, &caoVAO);
//    glDeleteBuffers(1, &cubeVBO);
//    glDeleteBuffers(1, &planeVBO);
//    glDeleteBuffers(1, &caoVBO);
//
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
//
//unsigned int loadTextureAlpha(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}


//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "stbImg/stb_image.h"
//
//#include "shader.h"
//#include "camera.h"
//#include "Model.h"
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//unsigned int loadTexture(const char* path);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = (float)SCR_WIDTH / 2.0;
//float lastY = (float)SCR_HEIGHT / 2.0;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//    // build and compile shaders
//    // -------------------------
//    shader myshader("object.vs", "object.fs");
//    shader shaderSingleColor("floor.vs", "floor.fs");
//
//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
//    float cubeVertices[] = {
//        // positions          // texture Coords
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//    };
//    float planeVertices[] = {
//        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
//        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//
//         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
//    };
//    // cube VAO
//    unsigned int cubeVAO, cubeVBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glBindVertexArray(0);
//    // plane VAO
//    unsigned int planeVAO, planeVBO;
//    glGenVertexArrays(1, &planeVAO);
//    glGenBuffers(1, &planeVBO);
//    glBindVertexArray(planeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glBindVertexArray(0);
//
//    // load textures
//    // -------------
//    unsigned int cubeTexture = loadTexture("D:/jack_work/OpenGL/MyLearning/HighLight/marble.jpg");
//    unsigned int floorTexture = loadTexture("D:/jack_work/OpenGL/MyLearning/HighLight/metal.jpg");
//
//    // shader configuration
//    // --------------------
//    myshader.use();
//    myshader.setInt("texture1", 0);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!
//
//        // set uniforms
//        shaderSingleColor.use();
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        shaderSingleColor.setMat4("view", view);
//        shaderSingleColor.setMat4("projection", projection);
//
//        myshader.use();
//        myshader.setMat4("view", view);
//        myshader.setMat4("projection", projection);
//
//        // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
//        glStencilMask(0x00);
//        // floor
//        glBindVertexArray(planeVAO);
//        glBindTexture(GL_TEXTURE_2D, floorTexture);
//        myshader.setMat4("model",glm::mat4(1.0f));
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        glBindVertexArray(0);
//
//        // 1st. render pass, draw objects as normal, writing to the stencil buffer
//        // --------------------------------------------------------------------
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);
//        glStencilMask(0xFF);
//        // cubes
//        glBindVertexArray(cubeVAO);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, cubeTexture);
//        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//        myshader.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//        myshader.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
//        // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
//        // the objects' size differences, making it look like borders.
//        // -----------------------------------------------------------------------------------------------------------------------------
//        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//        glStencilMask(0x00);
//        glDisable(GL_DEPTH_TEST);
//        shaderSingleColor.use();
//        float scale = 1.1f;
//        // cubes
//        glBindVertexArray(cubeVAO);
//        glBindTexture(GL_TEXTURE_2D, cubeTexture);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//        model = glm::scale(model, glm::vec3(scale, scale, scale));
//        shaderSingleColor.setMat4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(scale, scale, scale));
//        shaderSingleColor.setMat4("model", model);
//
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glBindVertexArray(0);
//        glStencilMask(0xFF);
//        glStencilFunc(GL_ALWAYS, 0, 0xFF);
//        glEnable(GL_DEPTH_TEST);
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteVertexArrays(1, &planeVAO);
//    glDeleteBuffers(1, &cubeVBO);
//    glDeleteBuffers(1, &planeVBO);
//
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}