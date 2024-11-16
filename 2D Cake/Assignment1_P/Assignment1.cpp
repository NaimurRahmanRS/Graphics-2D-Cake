#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;

// Variables for object 1 (plate)
float rotateAngle1 = 0.0f;
float translate_X1 = 0.0f;
float translate_Y1 = 0.0f;
float scale_X1 = 1.0f;
float scale_Y1 = 1.0f;

// Variables for object 2 (cake)
float rotateAngle2 = 0.0f;
float translate_X2 = 0.0f;
float translate_Y2 = 0.0f;
float scale_X2 = 1.0f;
float scale_Y2 = 1.0f;

// Variables for both objects
float rotateAngleBoth = 0.0f;
float translate_XBoth = 0.0f;
float translate_YBoth = 0.0f;
float scale_XBoth = 1.0f;
float scale_YBoth = 1.0f;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Assignment 1 by 1907025", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //House Points
    float vertices_plate[] = {

        //1 plate
        -0.75f, 0.125f, 0.0f,
         0.75f, 0.125f, 0.0f,
         0.75f, -0.7f, 0.0f,
        -0.75f, -0.7f, 0.0f,

         //2 plate extension
        -0.75f, -0.7f, 0.0f,
         0.75f, -0.7f, 0.0f,
         0.75f, -0.8f, 0.0f,
        -0.75f, -0.8f, 0.0f,
    };

    float vertices_cake[] = {
        //3 cake top
         0.0f, 0.5f, 0.0f,
         0.667f, 0.0f, 0.0f,
        -0.667f, 0.0f, 0.0f,

        //4 layer 1
        -0.667f, 0.0f, 0.0f,
         0.667f, 0.0f, 0.0f,
         0.667f, -0.125f, 0.0f,
        -0.667f, -0.125f, 0.0f,

        //5 layer 2
        -0.667f, -0.125f, 0.0f,
         0.667f, -0.125f, 0.0f,
         0.667f, -0.25f, 0.0f,
        -0.667f, -0.25f, 0.0f,

        //6 layer 3
        -0.667f, -0.25f, 0.0f,
         0.667f, -0.25f, 0.0f,
         0.667f, -0.375f, 0.0f,
        -0.667f, -0.375f, 0.0f,

        //7 layer 4
        -0.667f, -0.375f, 0.0f,
         0.667f, -0.375f, 0.0f,
         0.667f, -0.5f, 0.0f,
        -0.667f, -0.5f, 0.0f,

        //8 layer 5
        -0.667f, -0.5f, 0.0f,
         0.667f, -0.5f, 0.0f,
         0.667f, -0.625f, 0.0f,
        -0.667f, -0.625f, 0.0f,
    };

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_plate), vertices_plate, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cake), vertices_cake, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 identityMatrix = glm::mat4(1.0f);

        glm::mat4 modelMatrix1 = glm::translate(identityMatrix, glm::vec3(translate_X1 + translate_XBoth, translate_Y1 + translate_YBoth, 0.0f));
        modelMatrix1 = glm::rotate(modelMatrix1, glm::radians(rotateAngle1 + rotateAngleBoth), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(scale_X1 * scale_XBoth, scale_Y1 * scale_YBoth, 1.0f));

        glm::mat4 modelMatrix2 = glm::translate(identityMatrix, glm::vec3(translate_X2 + translate_XBoth, translate_Y2 + translate_YBoth, 0.0f));
        modelMatrix2 = glm::rotate(modelMatrix2, glm::radians(rotateAngle2 + rotateAngleBoth), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(scale_X2 * scale_XBoth, scale_Y2 * scale_YBoth, 1.0f));

        // get matrix's uniform location and set matrix
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix1));
        glBindVertexArray(VAO[0]);
        glUniform4f(colorLoc, 0.8588f, 0.8941f, 0.9294f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glUniform4f(colorLoc, 0.7569f, 0.8039f, 0.8510f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
        

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix2));
        glBindVertexArray(VAO[1]);

        glUniform4f(colorLoc, 0.4196f, 0.2471f, 0.1333f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
        glUniform4f(colorLoc, 0.7569f, 0.4745f, 0.2471f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 3, 4);
        glUniform4f(colorLoc, 0.9020f, 0.6392f, 0.4314f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 7, 4);
        glUniform4f(colorLoc, 0.7569f, 0.4745f, 0.2471f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 11, 4);
        glUniform4f(colorLoc, 0.9020f, 0.6392f, 0.4314f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 15, 4);
        glUniform4f(colorLoc, 0.7569f, 0.4745f, 0.2471f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);

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

    // Object 1 Controls
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        translate_X1 += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        translate_X1 -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        translate_Y1 += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        translate_Y1 -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        scale_X1 += 0.001f, scale_Y1 += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        scale_X1 -= 0.001f, scale_Y1 -= 0.001f;
        if (scale_X1 < 0.1f) scale_X1 = 0.1f;
        if (scale_Y1 < 0.1f) scale_Y1 = 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        rotateAngle1 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        rotateAngle1 -= 0.5f;

    // Object 2 Controls
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        translate_X2 += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        translate_X2 -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        translate_Y2 -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        translate_Y2 += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        scale_X2 += 0.001f, scale_Y2 += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        scale_X2 -= 0.001f, scale_Y2 -= 0.001f;
        if (scale_X2 < 0.1f) scale_X2 = 0.1f;
        if (scale_Y2 < 0.1f) scale_Y2 = 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        rotateAngle2 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        rotateAngle2 -= 0.5f;

    // Both Objects Controls
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translate_XBoth += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translate_XBoth -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translate_YBoth += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translate_YBoth -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        scale_XBoth += 0.001f, scale_YBoth += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        scale_XBoth -= 0.001f, scale_YBoth -= 0.001f;
        if (scale_XBoth < 0.1f) scale_XBoth = 0.1f;
        if (scale_YBoth < 0.1f) scale_YBoth = 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        rotateAngleBoth += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        rotateAngleBoth -= 0.5f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
