/**
 * @file Application.cpp
 * @author Rohan Siddhu
 * @brief This file contains the starting point of the program.
 * @version 0.1
 * @date 2022-04-05
 */

#include "Application.hpp"


/* Global Data */
Camera cam(glm::vec3(0.0f, 1.0f, 3.0f));

/* Time */
float deltaTime = 0.0f;
float lastTime = 0.0f;

/* Mouse */
float lastX = WIDTH / 2, lastY = HEIGHT / 2;
bool firstMouse = true;
int cursorMode = GLFW_CURSOR_DISABLED;

/* Light */
glm::vec3 lightPos(1.0f, 1.0f, 2.0f);


/* Main Function */
int main() {
    /* Initialize GLFW */
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    /* set GLFW error callback */
    glfwSetErrorCallback(error_callback);

    /* Set GLFW window options */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create window */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "glLight", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

    /* set callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
    }

    /* OpenGL options */
    glEnable(GL_DEPTH_TEST);

    /* create shader program */
    Shader lightingShader;
    lightingShader.addShader(GL_VERTEX_SHADER, "res/shaders/vertexShader.glsl");
    lightingShader.addShader(GL_FRAGMENT_SHADER, "res/shaders/lighting_fs.glsl");
    lightingShader.createProgram();

    Shader lightObjectShader;
    lightObjectShader.addShader(GL_VERTEX_SHADER, "res/shaders/vertexShader.glsl");
    lightObjectShader.addShader(GL_FRAGMENT_SHADER, "res/shaders/light_object_fs.glsl");
    lightObjectShader.createProgram();

    // Create and Initialize Buffers
	GLuint vaObj, vbObj;
	glGenVertexArrays(1, &vaObj);
	glGenBuffers(1, &vbObj);

	glBindVertexArray(vaObj);
	glBindBuffer(GL_ARRAY_BUFFER, vbObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

    /* Light Vertex Array Object */
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbObj);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);
    glEnableVertexAttribArray(0);

	/* Create Texture */
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int32_t imgWidth, imgHeight, nChannel;
	unsigned char* data = stbi_load("res/textures/box.jpg", &imgWidth, &imgHeight, &nChannel, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load image.\n";
		glDeleteTextures(1, &texture);
	}

	stbi_image_free(data);

    /* Game Loop */
    glfwSwapInterval(1);
    while(!glfwWindowShouldClose(window)) {
        float currentTime = (float) glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents();
        processInput(window);

        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // Modal Matrix
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(30.0f * currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
        // lightShader.setMat4("model", glm::value_ptr(model));

        // // View Matrix
        // glm::mat4 view = cam.getViewMatrix();
        // lightShader.setMat4("view", glm::value_ptr(view));

        // // Projection Matrix
        // glm::mat4 projection = glm::mat4(1.0f);
        // projection = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        // lightShader.setMat4("projection", glm::value_ptr(projection));

        // Draw Object
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", cam.position);

        // Model
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", glm::value_ptr(model));
        // View / Projection
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("view", glm::value_ptr(view));
        lightingShader.setMat4("projection", glm::value_ptr(projection));

        // Render cube
        glBindVertexArray(vaObj);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Draw Light source object
        lightObjectShader.use();
        model = glm::mat4(1.0f);
        float radius = 2.5f;
        float speed = 1.0f * currentTime;
        lightPos.x = radius * glm::cos(speed);
        lightPos.z = radius * glm::sin(speed);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightObjectShader.setMat4("model", glm::value_ptr(model));
        lightObjectShader.setMat4("view", glm::value_ptr(view));
        lightObjectShader.setMat4("projection", glm::value_ptr(projection));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }

    /* clean up */
    lightingShader.clean();
    lightObjectShader.clean();
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &vbObj);
    glDeleteVertexArrays(1, &vaObj);
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
 }


 /* Input Process */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.keyInput(Command::FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.keyInput(Command::BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.keyInput(Command::LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.keyInput(Command::RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cam.keyInput(Command::UP);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam.keyInput(Command::DOWN);
	}
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cam.keyInput(Command::RESET);
    }
}
 

 /* key_callback : Handle keyboard events */
 void key_callback(GLFWwindow* window, int key, int code, int action, int mod)  {
     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
         //glfwSetWindowShouldClose(window, true);
         glfwSetInputMode(window, GLFW_CURSOR,
             (cursorMode == GLFW_CURSOR_DISABLED) ? cursorMode = GLFW_CURSOR_NORMAL : cursorMode = GLFW_CURSOR_DISABLED);
     }
 }

 // GLFW mouse callback
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

// GLFW Scroll callback
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	cam.zoom((float)yOffset);
}

 /* error_callback : Handle GLFW errors */
 void error_callback(int errorCode, const char* description) {
     /* log error */
     std::cerr << "Error " << errorCode << " : " << description << std::endl;
 }