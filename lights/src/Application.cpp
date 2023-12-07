/**
 * @file Application.cpp
 * @author Rohan Siddhu
 * @brief Program's execution starts from this file.
 * @version 0.1
 * @date 2022-10-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Application.hpp"


// Global Data
//-------------

int g_width = 1080;  /** window width */
int g_height = 640; /** window height */

float g_deltaTime = 0.0f;   /** time between two frames */

Camera cam(glm::vec3(-0.7f, 0.7f, 3.9f), glm::vec3(0.0f, 1.0f, 0.0f), -63.5f, -6.5f);

glm::vec3 lightPos(1.0f, 0.5f, 2.0f);   /** Position of light source. */

// Mouse
float g_lastX = g_width / 2, g_lastY = g_height / 2;
bool firstMouse = true;

// Cube positions
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};


/**
 * @brief GLFW error callback function. This function is called whenever an error occurs in GLFW.
 * Prints GLFW error code with default description associated with it.
 * 
 * @param error Error code.
 * @param message Description of the error.
 * @return void 
 * 
 * @since 2022-10-04
 */
void error_callback(int error, const char* message) {
    std::cerr << ":( Error" << error << ": " << message << std::endl;
}


/**
 * @brief This function is called every frame to process the keyboard inputs.
 * 
 * @param window Pointer to the target window.
 * @return void
 */
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


// Main Function
//---------------
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(g_width, g_height, "Lights", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Set Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffersize_callback);
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
    //--------------------
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


    // Main Loop
    //-----------
    while (!glfwWindowShouldClose(window)) {
        // Delta Time
        //------------
        static float lastTime;  /** Time of last frame. */
        float currentTime = (float)glfwGetTime();
        g_deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents();
        process_input(window);

        static ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        static glm::vec3 lightColor(1.0f);     /** Light Color */


        // Dear ImGui
        //------------

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Creating a Test window with Dear ImGui
        {
            static bool initFlag = true;
            static float f = 0.0f;
            static int counter = 0;
            static ImVec4 light {lightColor.r, lightColor.g, lightColor.b, 1.0f};

            ImGui::Begin("Test");

            ImGui::Text("Camera");
            ImGui::Text("Position: x = %.2f, y = %.2f, z = %.2f", cam.position.x, cam.position.y, cam.position.z);
            ImGui::Text("Rotation: pitch = %.2f, yaw = %.2f", cam.pitch, cam.yaw);

            if (initFlag) {
                ImGui::SetWindowPos(ImVec2{ 5, 5 });
                ImGui::SetWindowSize(ImVec2{ 350, 180 });
                initFlag = false;
            }

            ImGui::Text("Background");
            ImGui::ColorEdit3("Clear Color", (float*)&clearColor);

            ImGui::Text("Light");
            ImGui::ColorEdit3("Light Color", (float*)&light);
            lightColor = {light.x, light.y, light.z};
            
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Cubes
        shader.use();
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("light.position", lightPos);
        // shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        shader.setVec3("light.ambient", lightColor * 0.2f);
        shader.setVec3("light.diffuse", lightColor * 0.5f);
        shader.setVec3("light.specular", lightColor);
        shader.setFloat("light.constant", 1.0f);
        shader.setFloat("light.linear", 0.09f);
        shader.setFloat("light.quadratic", 0.032f);

        // Material
        shader.setFloat("material.shininess", 32.0f);

        // Transformation
        glm::mat4 view = cam.getViewMatrix();
        shader.setMat4("view", glm::value_ptr(view));
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), (float)g_width / g_height, 0.1f, 1000.0f);
        shader.setMat4("projection", glm::value_ptr(projection));

        // Light source object
        lightShader.use();
        lightShader.setVec3("color", lightColor);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("model", glm::value_ptr(model));
        lightShader.setMat4("view", glm::value_ptr(view));
        lightShader.setMat4("projection", glm::value_ptr(projection));


        // Render
        //---------
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render Cubes
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(vaoCube);
        for (int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3, 0.5));
            shader.setMat4("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render light source object
        lightShader.use();
        glBindVertexArray(vaoLight);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    //---------
    shader.clean();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vaoLight);
    glDeleteVertexArrays(1, &vaoCube);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
//======================
// End of Main Function
//----------------------


/**
 * @brief Set the OpenGL Viewport. This function is called whenever window is resized.
 * 
 * @param window Pointer to the window.
 * @param width Frame width.
 * @param height Frame height.
 * @return void
 */
void framebuffersize_callback(GLFWwindow* window, int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
}


/**
 * @brief Keyboard key callback function. Captures keyboard event.
 * 
 * @param window The window that received the event.
 * @param key The keyboard key that was pressed or released.
 * @param scanCode The system-specific scancode of the key.
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
 * @param mod Bit field describing which modifier keys were held down.
 * @return void
 */
void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mod) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
            glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetScrollCallback(window, scroll_callback);
        }
    }
}


/**
 * @brief Cursor position callback function.
 * 
 * @param window The window that received the event.
 * @param xPos The new cursor x-coordinate, relative to the left edge of the content area.
 * @param yPos The new cursor y-coordinate, relative to the top edge of the content area.
 * @return void
 */
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        g_lastX = xPos;
        g_lastY = yPos;
        firstMouse = false;
    }

	float xOffset = xPos - g_lastX;
	float yOffset = g_lastY - yPos;
	g_lastX = xPos;
	g_lastY = yPos;

	cam.mouseInput(xOffset, yOffset);
}


/**
 * @brief Scroll event callback function. Zoom in/out.
 * 
 * @param window The window that received the event.
 * @param xOffset The scroll offset along the x-axis.
 * @param yOffset The scroll offset along the y-axis.
 * @return void
 */
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	cam.zoom((float)yOffset);
}


/**
 * @brief Load the texture image located at 'path'.
 * 
 * @param path Path to the texture.
 * @return GLuint [typedef unsigned int GLuint] - Texture ID.
 */
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