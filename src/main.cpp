#include "resource_manager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <map>
#include <filesystem>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

float currentFrame = glfwGetTime();
float prevFrame = 0;
float deltaTime;

ResourceManager* rManager = new ResourceManager();

void processInput(GLFWwindow* window);


struct Square
{
	glm::vec2 position;
	glm::vec2 scale;
	int state;
	bool stuck = false;
};

class Grid
{
public:
	int rows, columns, screenWidth, screenHeight;
	float cellWidth, cellHeight;
	std::vector<Square*> squares;
	std::vector<glm::mat4> modelMatrices;
	float updateAccum = 0;

	bool start = false;
	// declaring mouse buttons
	bool mb1 = false; bool mb2 = false;

	Grid(GLFWwindow* window, int rows, int columns, int screenWidth, int screenHeight, Shader shader)
		: window(window), rows(rows), columns(columns), screenWidth(screenWidth), screenHeight(screenHeight), shader(shader)
	{
		createGrid();
		genBuffers();

		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
	}

	~Grid()
	{
		for (auto s : squares)
		{
			delete s;
		}
	}

	void DrawGrid()
	{
		shader.Use();
		glBindVertexArray(vao);
		int idx = 0;
		for (auto s : squares)
		{
			if (s->state > 0)
			{
				shader.SetMatrix4("model", modelMatrices[idx], GL_FALSE, false);
				shader.SetVector3f("color", glm::vec3(0.f), true);				
				glDrawArrays(GL_TRIANGLES, 0, 6);
				idx++;
			}
			else {
				idx++;
			}
		}
		glBindVertexArray(0);
	}

	void RandomizeGrid()
	{
		srand(static_cast<unsigned>(time(0)));
		for (auto s : squares)
		{
			s->state = rand() % 2;
		}
	}

	void Update(float dt)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) start = true;
		if (start && !mb2)
		{
			updateAccum += dt;
			if (updateAccum >= 0.0f)
			{
				moveSand();
				if (updateAccum >= 0.5f)
				{
					resetStuck();
					updateAccum = 0.f;
				}
			}
		}
	}

private:
	float lastIndex;
	GLuint vao, vbo;
	Shader shader;
	GLFWwindow* window;

	void createGrid()
	{
		cellHeight = screenHeight / static_cast<float>(rows);
		cellWidth = screenWidth / static_cast<float>(columns);
		

		std::cout << "Grid: " << rows << "x" << columns << '\n';
		std::cout << "RowHeight = " << cellHeight << ", ColWidth = " << cellWidth << '\n';

		int vertexCounter = 1;

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				// generating positions from column width and row height
				float x = j * cellWidth;
				float y = i * cellHeight;

				// creating box that fits into the grid location
				Square* s = new Square();
				s->position = glm::vec2(x, y);
				s->scale = glm::vec2(cellWidth, cellHeight);
				s->state = 0; // do not draw by default
				squares.push_back(s);

				// generating model matrix for the box to draw later
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(s->position, 0.f));
				model = glm::scale(model, glm::vec3(s->scale, 0.f));
				modelMatrices.push_back(model);

				// std::cout << "Vertex " << vertexCounter << ": (" << x << ", " << y << ")\n";
				vertexCounter++;
			}
		}

	}

	void genBuffers()
	{
		float geometry[] =
		{
			 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,

			 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  0.0f, 0.0f, 0.0f, 0.0f
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}


	int locateGridIndex(Square* s)
	{
		int x = static_cast<int>(floor(s->position.x / cellWidth));
		int y = static_cast<int>(float(s->position.y / cellHeight));
		return y * columns + x;
	}

	int locateGridIndex(double x, double y)
	{
		int indexX = static_cast<int>(floor(x / cellWidth));
		int indexY = static_cast<int>(float(y / cellHeight));
		return indexY * columns + indexX;
	}

	void moveSand()
	{
		std::map<int, int> changes; // list of indices and states
		// find the necessary changes and store them 
		for (auto s : squares)
		{
			if (s->state > 0)
			{
				int idx = locateGridIndex(s);
				if (idx >= columns * rows - columns)
				{
					continue;
				}
				else
				{
					int directlyUnder = idx + columns;
					int downLeft = idx + columns - 1;
					int downRight = idx + columns + 1;
					if (downRight > rows * columns - 1) continue;
					if (squares[directlyUnder]->state == 0)
					{
						changes[idx] = 0;
						changes[directlyUnder] = 1;
					}
					else if (squares[downLeft]->state == 0 && squares[downRight]->state == 0)
					{
						int possible[2] = { downLeft, downRight };
						int choice = rand() % 2;
						changes[idx] = 0;
						changes[possible[choice]] = 1;
					}
					else if (downLeft < rows * columns || downRight < rows * columns)
					{
						if (squares[downLeft]->state == 0)
						{
							changes[idx] = 0;
							changes[downLeft] = 1;
						}
						else if (squares[downRight]->state == 0)
						{
							changes[idx] = 0;
							changes[downRight] = 1;
						}
					}
					else s->stuck = true;
				}
			}
		}

		// apply the changes
		for (auto& kv : changes)
		{
			squares[kv.first]->state = kv.second;
		}
	}

	void resetStuck()
	{
		for (auto& s : squares)
		{
			s->stuck = false;
		}
	}

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		// need to get this since static function can't access the this pointer
		Grid* grid = static_cast<Grid*>(glfwGetWindowUserPointer(window));
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			if (action == GLFW_PRESS)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
				grid->mb1 = true;
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				int idx = grid->locateGridIndex(x, y);
				grid->squares[idx]->state = 1;
			}
			if (action == GLFW_RELEASE)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				grid->mb1 = false;
				grid->lastIndex = -1;
			}
		}

		if (button == GLFW_MOUSE_BUTTON_2)
		{
			if (action == GLFW_PRESS)
			{
				grid->mb2 = true;
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				int idx = grid->locateGridIndex(x, y);
				grid->squares[idx]->state = 0;
			}
			if (action == GLFW_RELEASE)
			{
				grid->mb2 = false;
				grid->lastIndex = -1;
			}
		}
	}


	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		// acquiring grid pointer hopefully i put it there lol
		Grid* grid = static_cast<Grid*>(glfwGetWindowUserPointer(window));
		double x = xpos;
		double y = ypos;
		
		// reset cursor position if it goes off screen on the y axis
		if (ypos > SCREEN_HEIGHT || ypos < 0)
		{
			glfwSetCursorPos(window, x, SCREEN_HEIGHT / 2.f);
			y = SCREEN_HEIGHT / 2.f;
		}

		// if mb pressed, find idx of sand and check it's within grid
		// handles mouse drag
		if (grid->mb1 || grid->mb2)
		{
			int idx = grid->locateGridIndex(x, y);
			if (idx < grid->columns * grid->rows && idx > -1)
			{
				if (grid->mb1)
				{
					if (idx != grid->lastIndex);
					{
						grid->squares[idx]->state = 1;
						grid->lastIndex = idx;
					}
				}
				if (grid->mb2)
				{
					if (idx != grid->lastIndex)
					{
						grid->squares[idx]->state = 0;
						grid->lastIndex = idx;
					}
				}

			}
		}
	}
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling Sand Simulator", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create OpenGL window.\n";
		glfwTerminate();
		return 1;
	}

	
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad.\n";
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	std::cout << "CWD: " << std::filesystem::current_path() << '\n';
	rManager->LoadShader("../shaders/shader.vert", "../shaders/shader.frag", nullptr, "basic");
	Shader shader = rManager->GetShader("basic");

	glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f);

	shader.Use();
	shader.SetMatrix4("projection", projection, GL_FALSE, false);

	int ratioMultiplier = 32; 

	Grid grid = Grid(window, 9 * ratioMultiplier, 16 * ratioMultiplier, SCREEN_WIDTH, SCREEN_HEIGHT, shader);
	
	glfwSetWindowUserPointer(window, &grid);

	float accumulator = 0;

	int fpsAccum = 0;
	int frameCounter = 0;

	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - prevFrame;
		prevFrame = currentFrame;
		accumulator += deltaTime;
		
		fpsAccum += deltaTime;
		frameCounter++;
		if (fpsAccum >= 5.0f)
		{
			std::cout << "Average FPS: " << frameCounter / fpsAccum << '\n';
			frameCounter = 0;
			fpsAccum = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

		processInput(window);

		grid.Update(deltaTime);
		grid.DrawGrid();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

