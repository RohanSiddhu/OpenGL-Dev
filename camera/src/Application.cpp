/**
 * @file Application.cpp
 * @author Rohan Siddhu
 * @brief Program's execution starts from this file.
 * @version 0.1
 * @date 2022-06-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Application.hpp"


/* Camera */
Camera cam(glm::vec3(0.0f, 1.0f, 3.0f));

/* Light */
glm::vec3 lightPos(1.0f, 0.5f, 2.0f);

/* Time */
float g_deltaTime = 0.0f;
float g_lastTime = 0.0f;

/* Mouse */
float lastX = DEFAULT_WINDOW_WIDTH / 2, lastY = DEFAULT_WINDOW_HEIGHT / 2;
bool firstMouse = true;
int cursorMode = GLFW_CURSOR_DISABLED;


/* prototype declaration */
void process_input(GLFWwindow* window);


/* main function starts */
int main(int argc, char* argv[]) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";

        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(error_callback);

    // Set window parameters
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creating window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Camera", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);

    // Initialize Buffers
    GLuint vaoCube, vaoLight, vbo;

    glGenVertexArrays(1, &vaoCube);
    glGenVertexArrays(1, &vaoLight);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindVertexArray(vaoLight);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);
    glEnableVertexAttribArray(0);

    // Load Textures
    GLuint diffuseMap = load_texture("res/textures/container1.png");
    GLuint specularMap = load_texture("res/textures/container1_specular.png");

    // Create Shader
    Shader shader;
    shader.addShader(GL_VERTEX_SHADER, "res/shaders/vertexShader.glsl");
    shader.addShader(GL_FRAGMENT_SHADER, "res/shaders/fragmentShader.glsl");
    shader.createProgram();
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    Shader lightShader;
    lightShader.addShader(GL_VERTEX_SHADER, "res/shaders/vertexShader.glsl");
    lightShader.addShader(GL_FRAGMENT_SHADER, "res/shaders/fsLight.glsl");
    lightShader.createProgram();

    //===========
    // Main Loop
    //===========
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time, i.e., time diffrence b/w two consecutive frame renders.
        float currentTime = (float)glfwGetTime();
        g_deltaTime = currentTime - g_lastTime;
        g_lastTime = currentTime;

        glfwPollEvents();
        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw Cube object
        shader.use();
        shader.setVec3("lightPos", lightPos);

        // light properties
        glm::vec3 lightColor(1.0f);

        shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        shader.setFloat("material.shininess", 64.0f);

        // Model
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", glm::value_ptr(model));
        // View
        glm::mat4 view = cam.getViewMatrix();
        shader.setMat4("view", glm::value_ptr(view));
        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), 800.0f / 600.0f, 0.1f, 1000.0f);
        shader.setMat4("projection", glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        // Render Cube
        glBindVertexArray(vaoCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw light source
        lightShader.use();
        lightShader.setVec3("color", lightColor);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("model", glm::value_ptr(model));
        lightShader.setMat4("view", glm::value_ptr(view));
        lightShader.setMat4("projection", glm::value_ptr(projection));
        // Render light source
        glBindVertexArray(vaoLight);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }

    shader.clean();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vaoLight);
    glDeleteVertexArrays(1, &vaoCube);
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
/* main function ends */


/* process_input */
void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.keyInput(Command::FORWARD, g_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.keyInput(Command::BACKWARD, g_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.keyInput(Command::LEFT, g_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.keyInput(Command::RIGHT, g_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cam.keyInput(Command::UP, g_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam.keyInput(Command::DOWN, g_deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cam.keyInput(Command::RESET, g_deltaTime);
    }
}


/* GLFW callback definition */
void error_callback(int error, const char* message) {
    std::cerr << "Error(" << error << "): " << message << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mod) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	cam.mouseInput(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	cam.zoom((float)yOffset);
}

/* loads 2D texture from file */
GLuint load_texture(const char* path) {
    GLuint id;
    glGenTextures(1, &id);

    int width, height, nrComponents;
    unsigned char* img = stbi_load(path, &width, &height, &nrComponents, 0);
    if (img) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(img);
    }
    else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(img);
    }

    return id;
}