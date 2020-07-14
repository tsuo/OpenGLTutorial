/*
Finish setting up Polygon constructors
Added simple vertices and indices system

TODO:
- Work on Graphics drawing Polygons
- Should work for all Polygons
*/


#include <iostream>

#include <glad.h>
#include <glfw3.h>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = 
				"#version 330 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aColor;\n"
				"out vec3 ourColor;\n"
				"void main()\n"
				"{\n"
				//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
				"	gl_Position = vec4(aPos, 1.0);\n"
				"	ourColor = aColor;\n"
				"}\0";

const char* fragmentShaderSource =
				"#version 330 core\n"
				"out vec4 FragColor;\n"
				// "uniform vec4 ourColor;\n" //using uniform
				"in vec3 ourColor;\n"
				"void main()\n"
				"{\n"
				//"	FragColor = ourColor;\n"  //using uniform
				"	FragColor = vec4(ourColor, 1.0);\n"
				"}\n";


int tutorial() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window << std::endl";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	int  success;
	char infoLog[512];
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	float vertices1[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		-0.9f, -0.9f, 0.0f,
		-0.7f, -0.7f, 0.0f,
		-0.8f, -0.5f, 0.0f
	};

	float vertices2[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	float vertices3[] = {
		-0.2f, -0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, -0.4f, 0.0f,	0.0f, 1.0f, 0.0f,
		-0.1f, 0.8f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// use vertices1 for regular triangles drawing without indices
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	/*
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(shaderProgram);

	// Testing Shaders functions
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		glBindVertexArray(VAO);


		/*
		// Get uniform "ourColor"
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		*/

		glDrawArrays(GL_TRIANGLES, 0, 3);  // drawing triangles given verteces
		// drawing squares efficiently using indices
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

struct VertexInfo {
	float X;
	float Y;
	float Z;
	float R;
	float G;
	float B;
	float A;
	float TextLocX;
	float TextLocY;
};

class Polygon
{
private:
	int numVertices;
	VertexInfo* vertices;
	int numIndices;
	int* indices;    // for EBO draw

	void loadVertexData(VertexInfo* vertex, float x, float y, float z, float r, float g, float b, float a, float textx, float texty)
	{
		vertex->X = x;
		vertex->Y = y;
		vertex->Z = z;
		vertex->R = r;
		vertex->G = g;
		vertex->B = b;
		vertex->A = a;
		vertex->TextLocX = textx;
		vertex->TextLocY = texty;
	}

	void loadIndicesData(int index[], int indlen)
	{
		if (indlen > 0)
		{
			memcpy(indices, index, sizeof(int) * indlen);
		}
	}

public:
	Polygon(int ln, int ii, int inds[], float locations[]) {
		int locindex = 0;
		numIndices = ii;
		numVertices = ln;
		vertices = new VertexInfo[ln];
		indices = new int[ii];

		// load vertices array
		for (int i = 0; i < ln; i++)
		{
			this->loadVertexData(&vertices[i], locations[locindex], locations[locindex+1], locations[locindex+2],
								1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
			locindex += 3;
		}
		
		// load indices array
		this->loadIndicesData(inds, ii);
		
	}
	
	Polygon(int ln, int ii, int inds[], float locations[], float colors[])
	{
		int locindex = 0;
		int colindex = 0;
		numIndices = ii;
		numVertices = ln;
		vertices = new VertexInfo[ln];
		indices = new int[ii];

		// load vertices array
		for (int i = 0; i < ln; i++)
		{
			this->loadVertexData(&vertices[i], locations[locindex], locations[locindex + 1], locations[locindex + 2],
				colors[colindex], colors[colindex+1], colors[colindex+2], colors[colindex+3], 0.0f, 0.0f);
			locindex += 3;
			colindex += 4;
		}

		// load indices array
		this->loadIndicesData(inds, ii);
		
	}

	Polygon(int ln, int ii, int inds[], float locations[], float colors[], float textLocs[])
	{
		int locindex = 0;
		int colindex = 0;
		int texindex = 0;
		numIndices = ii;
		numVertices = ln;
		vertices = new VertexInfo[ln];
		indices = new int[ii];

		// load vertices array
		for (int i = 0; i < ln; i++)
		{
			this->loadVertexData(&vertices[i], locations[locindex], locations[locindex + 1], locations[locindex + 2],
				colors[colindex], colors[colindex + 1], colors[colindex + 2], colors[colindex + 3], 
				textLocs[texindex], textLocs[texindex+1]);
			locindex += 3;
			colindex += 4;
			texindex += 2;
		}

		// load indices array
		this->loadIndicesData(inds, ii);
	}
	
	Polygon(int ln, int ii, int inds[], VertexInfo verts[])
	{
		numIndices = ii;
		numVertices = ln;
		vertices = new VertexInfo[ln];
		indices = new int[ii];

		// load vertices array
		memcpy(vertices, verts, ln * sizeof(VertexInfo));
		
		// load indices array
		this->loadIndicesData(inds, ii);
	}

	void printVerticesInfo()
	{
		for (int i = 0; i < numVertices; i++)
		{
			std::cout << i << "|Location: [" << vertices[i].X << "," << vertices[i].Y << "," << vertices[i].Z << "]" << std::endl;
			std::cout << i << "|Color: [" << vertices[i].R << "," << vertices[i].G << "," << vertices[i].B << "," << vertices[i].A << "]" << std::endl;
			std::cout << i << "|Texture Index: [" << vertices[i].TextLocX << "," << vertices[i].TextLocY << "]" << std::endl;
		}
	}

	void printIndices()
	{
		for (int i = 0; i < numIndices; i++)
		{
			std::cout << i << "|" << indices[i] << std::endl;
		}
	}
};

// class in charge of drawing onto window
class Graphics
{
private:

public:
	void drawTriangle(Polygon p, float x, float y, float z)
	{

	}

	void drawPolygon(Polygon p, float x, float y, float z)
	{

	}

	void drawPolygonGroup(Polygon* ps, float x, float y, float z)
	{

	}
};

class Game
{
private:
	int width, height, framebufWidth, framebufHeight;
	char* name;
	GLFWwindow* window;

public:
	Game(int w, int h, char *n) {
		width = w;
		height = h;
		framebufWidth = 0;
		framebufHeight = 0;
		name = n;

		// init code and return window object
		// requires private variables width, height, and name to be predefined
		if ((window = initialize()) != NULL)
		{
			// game loop
			while (!glfwWindowShouldClose(window))
			{
				update();
				draw();
				glfwPollEvents();
			}
		}
		destroy();
	}

	// init and return a window object
	GLFWwindow* initialize() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window << std::endl";
			glfwTerminate();
			return NULL;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return NULL;
		}

		glfwGetFramebufferSize(window, &framebufWidth, &framebufHeight);
		glViewport(0, 0, framebufWidth, framebufHeight);
		std::cout << framebufWidth << std::endl << framebufHeight << std::endl;

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		return window;
	}

	void update() {
		glfwGetFramebufferSize(window, &framebufWidth, &framebufHeight);
		processInput(window);
	}

	void draw() {
		glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		// g.drawTriangle(triangle, 0, 0);

		glfwSwapBuffers(window);
	}

	void destroy() {
		glfwTerminate();
	}

	
};

int main()
{

	std::cout << sizeof(VertexInfo) << std::endl;
	float f[] = {
		0.2f, 0.4f, 0.0f,
		0.5f, 0.7f, 0.0f,
		0.7f, 0.7f, 0.0f
	};
	float c[] = { 0.1f, 0.4f, 0.7f, 1.0f,
				  0.2f, 0.3f, 0.4f, 0.5f,
				  0.4f, 0.5f, 0.6f, 0.7f};
	float t[] = { 0.0f, 0.5f, 
				  0.0f, 0.5f,
				  0.0f, 0.5f};

	VertexInfo verts[] = {	{0.2f, 0.4f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f, 0.0f, 0.5f},
							{0.5f, 0.7f, 0.0f, 0.2f, 0.3f, 0.4f, 0.5f, 0.0f, 0.5f},
							{0.7f, 0.7f, 0.0f, 0.4f, 0.5f, 0.6f, 0.7f, 0.0f, 0.5f}
						};
	int inds[] = { 1, 2, 3, 4, 5, 6 };

	Polygon triangle(3, 6, inds, f, c, t);
	//triangle.printVerticesInfo();

	Polygon triangle2(3, 6, inds, verts);
	triangle2.printVerticesInfo();
	std::cout << std::endl;
	triangle2.printIndices();
	std::cout << std::endl;

	//verts[2].TextLocY = 20.f;
	//std::cout << verts[2].TextLocY << std::endl;
	//triangle2.printVerticesInfo();

	//return tutorial();
	char name[] = "Game";
	Game game(1200, 800, name);

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}