#include<filesystem>
namespace fs = std::filesystem;

#include"Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Number of samples per pixel for MSAA
unsigned int samples = 8;

// Controls the gamma function
float gamma = 2.2f;

float rectangleVertices[] =
{
	//  Coords   // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

// Vertices for plane with texture
std::vector<Vertex> vertices =
{
	Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for plane with texture
std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_SAMPLES, samples);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl Normal Map", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, width, height);

	// Generates shaders
	Shader shaderProgram("default.vert", "default.frag", "default.geom");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 2.0f));

	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;

	// Paths to textures
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string diffusePath = "/Resources/diffuse.png";
	std::string normalPath = "/Resources/normal.png";

	std::string fullPath = parentDir + diffusePath;
	std::string fullPath1 = parentDir + normalPath;

	// Check if the texture file exists
	if (!fs::exists(fullPath)) {
		std::cerr << "Error: Texture file does not exist at path: " << fullPath << std::endl;
		return -1;
	}
	else if (!fs::exists(fullPath1)) {
		std::cerr << "Error: Texture file does not exist at path: " << fullPath1 << std::endl;
		return -1;
	}
	else {
		std::cout << "Success: Found texture file at path: " << fullPath << std::endl;
		std::cout << "Success: Found texture file at path: " << fullPath1 << std::endl;
	}

	std::vector<Texture> textures =
	{
		Texture((parentDir + diffusePath).c_str(), "diffuse", 0)
	};

	// Plane with the texture
	Mesh plane(vertices, indices, textures);
	// Normal map for the plane
	Texture normalMap((parentDir + normalPath).c_str(), "normal", 1);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Normal Map" + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;
		}

		// Specify the color of the background
		glClearColor(pow(0.07f, gamma), pow(0.05f, gamma), pow(0.4f, gamma), 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shaderProgram.Activate();
		normalMap.Bind();
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "normal0"), 1);

		// Draw the normal model
		plane.Draw(shaderProgram, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects
	shaderProgram.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}