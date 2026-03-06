#include "./Camera.h"
#include "./Shader.h"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#include <filesystem>

std::filesystem::path base = std::filesystem::path(__FILE__).parent_path();

auto vertexPath = (base / "assets" / "shader" / "vertexShader.glsl").string();
auto fragmentPath = (base / "assets" / "shader" / "fragmentShader.glsl").string();
auto img1 = (base / "assets" / "img" / "container2.png").string();
auto img2 = (base / "assets" / "img" / "container2_specular.png").string();

auto _3dmodel = (base / "assets" / "3d" / "backpack" / "backpack.obj").string();
//const char *_3dmodel = "D:/OpenGLCode/OpenGL/opengl/assets/3d/backpack/backpack.obj";


const char *glErrorToString(GLenum err);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path, int index);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define GL_CALL(fun)                     \
    do {                                \
        fun;                            \
        checkError(__FILE__, __LINE__, #fun); \
    } while (0)

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to Create Window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 在 glfwMakeContextCurrent 之后加上这两行
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEPTH_TEST);
	Shader lightingShader(vertexPath.c_str(), fragmentPath.c_str());

	Model ourModel(_3dmodel);

	lightingShader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.setMat4("model", model);

	bool diffuse = false;
	bool specular = false;
	for (int i = 0 ; i < ourModel.meshes.size(); i++)
	{
		for (int j = 0; j < ourModel.meshes[i].textures.size();j++)
		{
			if (diffuse == false && ourModel.meshes[i].textures[j].type == "texture_diffuse")
			{
				lightingShader.setVec3("material_diffuse1.ambient", ourModel.meshes[i].textures[j].ambientColor);
				lightingShader.setVec3("material_diffuse1.diffuse", ourModel.meshes[i].textures[j].diffuseColor);
				lightingShader.setVec3("material_diffuse1.specular", ourModel.meshes[i].textures[j].specularColor);
				lightingShader.setFloat("material_diffuse1.shininess", ourModel.meshes[i].textures[j].shininess);
				diffuse = true;
			}
			else if (specular == false && ourModel.meshes[i].textures[j].type == "texture_specular")
			{
				lightingShader.setVec3("material_specular1.ambient", ourModel.meshes[i].textures[j].ambientColor);
				lightingShader.setVec3("material_specular1.diffuse", ourModel.meshes[i].textures[j].diffuseColor);
				lightingShader.setVec3("material_specular1.specular", ourModel.meshes[i].textures[j].specularColor);
				lightingShader.setFloat("material_specular1.shininess", ourModel.meshes[i].textures[j].shininess);
				specular = true;
			}
		}
		if (specular && diffuse)
			break;
	}

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3("light.color", glm::vec3(1.0f));
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.09f);
		lightingShader.setFloat("light.quadratic", 0.032f);
		lightingShader.setVec3("light.position", camera.Position);
		lightingShader.setVec3("light.direction", camera.Front);
		lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(25.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		ourModel.Draw(lightingShader);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


const char *glErrorToString(GLenum err)
{
	switch (err)
	{
	case GL_INVALID_ENUM:					return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:					return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:				return "GL_INVALID_OPERATION";
	case GL_OUT_OF_MEMORY:					return "GL_OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default: return "UNKNOWN_ERROR";
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCR_WIDTH = (GLuint) width;
	SCR_HEIGHT = (GLuint) height;
}
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
std::string readFile(const std::string &path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "无法打开文件: " << path << std::endl;
		return "";
	}
	std::ostringstream content;
	content << file.rdbuf();
	return content.str();
}
void processInput(GLFWwindow *window)
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
unsigned int loadTexture(const char *path, int index)
{
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannel;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannel, 0);
	if (data == nullptr)
	{
		std::cout << "文件读取失败" << std::endl;
		return -1;
	}
	auto format = (nrChannel == 3) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	stbi_image_free(data);
	return texture;
}

void checkError(const char *file, int line, const char *func)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "[OpenGL Error] "
			<< err
			<< " | file: " << file
			<< " | line: " << line
			<< " | call: " << func
			<< " | err:" << glErrorToString(err)
			<< std::endl;
	}
}
