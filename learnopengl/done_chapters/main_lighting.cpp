#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "stb_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
float lastX = SCR_WIDTH / 2.f;
float lastY = SCR_HEIGHT / 2.f;
bool firstMouse = true;

// timing
float deltaTime = 0.f;
float lastFrame = 0.f;

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
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("shaders/lighting.vs", "shaders/lighting.fs");
    Shader lightCubeShader("shaders/light_cube.vs", "shaders/light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  0.f,
         0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  1.f,  0.f,
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  1.f,  1.f,
         0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  1.f,  1.f,
        -0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  1.f,
        -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  0.f,

        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  0.f,  0.f,
         0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  1.f,  0.f,
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  1.f,  1.f,
         0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  1.f,  1.f,
        -0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f,  0.f,  1.f,
        -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f,  0.f,  0.f,

        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,  1.f,  0.f,
        -0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  1.f,  1.f,
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.f,  1.f,
        -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.f,  1.f,
        -0.5f, -0.5f,  0.5f, -1.f,  0.f,  0.f,  0.f,  0.f,
        -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f,  1.f,  0.f,

         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,  1.f,  0.f,
         0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.f,  1.f,
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  0.f,  1.f,
         0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  0.f,  1.f,
         0.5f, -0.5f,  0.5f,  1.f,  0.f,  0.f,  0.f,  0.f,
         0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f,  1.f,  0.f,

        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.f,  1.f,
         0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.f,  1.f,
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  1.f,  0.f,
         0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  1.f,  0.f,
        -0.5f, -0.5f,  0.5f,  0.f, -1.f,  0.f,  0.f,  0.f,
        -0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.f,  1.f,

        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,  1.f,
         0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  1.f,  1.f,
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  1.f,  0.f,
         0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  1.f,  0.f,
        -0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f,  0.f,  0.f,
        -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,  1.f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.f,  0.f,  0.f),
        glm::vec3( 2.f,  5.f, -15.f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.f, -7.5f),
        glm::vec3( 1.3f, -2.f, -2.5f),
        glm::vec3( 1.5f,  2.f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };  
    
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture("textures/wood_container.png");
    unsigned int specularMap = loadTexture("textures/wood_container_specular.png");

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

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
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightColor;
        lightColor.x = 0.4f;
        lightColor.y = 0.7;
        lightColor.z = 0.1f;
        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("dirLight.diffuse", 1.f, 1.f, 1.f);
        lightingShader.setVec3("dirLight.specular", 1.f, 1.f, 1.f);

        for (int i = 0; i < 4; ++i) {
            lightingShader.setVec3("uPointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            lightingShader.setVec3("uPointLights[" + std::to_string(i) + "].ambient", 0.04f, 0.07f, 0.01f);
            lightingShader.setVec3("uPointLights[" + std::to_string(i) + "].diffuse", 0.4f, 0.7f, 0.1f);
            lightingShader.setVec3("uPointLights[" + std::to_string(i) + "].specular", 0.4f, 0.7f, 0.1f);
            lightingShader.setFloat("uPointLights[" + std::to_string(i) + "].constant", 1.f);
            lightingShader.setFloat("uPointLights[" + std::to_string(i) + "].linear", 0.07f);
            lightingShader.setFloat("uPointLights[" + std::to_string(i) + "].quadratic", 0.017f);
        }

        lightingShader.setVec3("uSpotLight.position", camera.Position);
        lightingShader.setVec3("uSpotLight.direction", camera.Front);
        lightingShader.setVec3("uSpotLight.ambient", 0.f, 0.f, 0.f);
        lightingShader.setVec3("uSpotLight.diffuse", 0.f, 1.f, 0.f);
        lightingShader.setVec3("uSpotLight.specular", 0.f, 1.f, 0.f);
        lightingShader.setFloat("uSpotLight.innerCutOff", glm::radians(7.f));
        lightingShader.setFloat("uSpotLight.outerCutOff", glm::radians(10.f));

        lightingShader.setFloat("material.shininess", 64.f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // render the cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);
        
        glm::mat4 model = glm::mat4(1.f);

        for (int i = 0; i < 10; ++i) {
            model = glm::mat4(1.f);
            // world transformation
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);
            
            glm::mat4 normalTransform = glm::transpose(glm::inverse(view * model));
            lightingShader.setMat4("normalTransform", normalTransform);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setVec3("color", lightColor);

        glBindVertexArray(lightCubeVAO);
        
        for (int i = 0; i < 4; ++i) {
            model = glm::mat4(1.f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lightCubeShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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

unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (data) {
        GLenum format;
        switch (channels) {
            case 1:
                format = GL_RED;
                break;
            case 2:
                format = GL_RG;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "ERROR: Failed to load texture: " << path << "\n";

        stbi_image_free(data);
    }

    return textureID;
}