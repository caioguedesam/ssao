#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL.h>
#include <vector>
#include <iostream>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <chrono>

#if _DEBUG
#include <cassert>
#define ASSERT(EXPR, MSG) do {assert(EXPR && MSG);} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {assert(!EXPR && MSG);} while(false)
#else
#define ASSERT(EXPR, MSG) do {} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {} while(false)
#endif

#define APP_DEFAULT_WIDTH 640
#define APP_DEFAULT_HEIGHT 480

#define RESOURCES_PATH "./resources/"

void GetDisplayDimensions(uint32_t &w, uint32_t &h)
{
	ASSERT(SDL_WasInit(SDL_INIT_VIDEO), "Trying to get display dimensions without initializing SDL.");
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);
	w = DM.w;
	h = DM.h;
}

// Default resources
// Vertices for 1x1 quad centered on origin
float g_quadVertices[] =
{
		0.5f,  0.5f, 0.f,  // top right
		 0.5f, -0.5f, 0.f,  // bottom right
		-0.5f, -0.5f, 0.f,  // bottom left
		-0.5f,  0.5f, 0.f   // top left
};
// Indices for 1x1 quad
uint32_t g_quadIndices[] =
{
	0, 2, 1,
	0, 3, 2
};

class FileReader
{
public:
	enum class Result
	{
		OK = 0,
		FAIL_OPEN,
		FAIL_READ
	};

	static Result ReadFile(const char* path, char* buffer)
	{
		FILE* pFile;
		long fSize = 0;
		// Open file
		pFile = fopen(path, "r");
		if (!pFile)
		{
			return Result::FAIL_OPEN;
		}

		// Query file size
		fseek(pFile, 0, SEEK_END);
		fSize = ftell(pFile);
		rewind(pFile);

		// Read file to buffer
		auto result = fread(buffer, 1, fSize, pFile);
		if (result != fSize && !feof(pFile))
		{
			return Result::FAIL_READ;
		}

		// Close file
		fclose(pFile);
		buffer[result] = '\0';
		return Result::OK;
	}
};

class Time
{
public:
	static std::chrono::high_resolution_clock::time_point lastTimePoint;

	// Times in seconds
	static double time;
	static double deltaTime;

	static void Init()
	{
		lastTimePoint = std::chrono::high_resolution_clock::now();
		time = 0.;
		deltaTime = 0.;
	}

	static void UpdateTime()
	{
		auto currentTimePoint = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> timePointDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTimePoint - lastTimePoint);

		deltaTime = timePointDelta.count();
		time += deltaTime;
		lastTimePoint = currentTimePoint;
	}
};
std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastTimePoint;
double Time::time;
double Time::deltaTime;

class Buffer
{
public:
	uint32_t handle;
	uint32_t sizeInBytes;
	uint32_t count;
	void* pData;

	Buffer()
	{
		glGenBuffers(1, &handle);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}

	void Bind(GLenum bindTarget)
	{
		glBindBuffer(bindTarget, handle);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}

	void Init(GLenum bindTarget, uint32_t bufferSize, uint32_t bufferCount, void* bufferData)
	{
		sizeInBytes = bufferSize;
		count = bufferCount;
		pData = bufferData;
		Bind(bindTarget);
		glBufferData(bindTarget, sizeInBytes, pData, GL_STATIC_DRAW);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}
};

class Shader
{
public:
	uint32_t programHandle = UINT32_MAX;

	uint32_t vertexHandle = UINT32_MAX;
	uint32_t fragmentHandle = UINT32_MAX;

	const char* vertexSrc = nullptr;
	const char* fragmentSrc = nullptr;

	void InitAndCompile(const char* vSrc, const char* fSrc)
	{
		if (programHandle != UINT32_MAX)
		{
			glDeleteProgram(programHandle);
		}
		programHandle = glCreateProgram();
		if (!vertexSrc || strcmp(vertexSrc, vSrc) != 0)
		{
			vertexSrc = vSrc;
			if (vertexHandle == UINT32_MAX)
			{
				vertexHandle = glCreateShader(GL_VERTEX_SHADER);
			}
			glShaderSource(vertexHandle, 1, &vertexSrc, NULL);
			glCompileShader(vertexHandle);
			int  success;
			char infoLog[512];
			glGetShaderInfoLog(vertexHandle, 512, NULL, infoLog);
			glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &success);
			if (!success) {
				std::cout << "ERROR::SHADER::COMPILE_FAILED\n" << infoLog << std::endl;
			}
		}
		if (!fragmentSrc || strcmp(fragmentSrc, fSrc) != 0)
		{
			fragmentSrc = fSrc;
			if (fragmentHandle == UINT32_MAX)
			{
				fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
			}
			glShaderSource(fragmentHandle, 1, &fragmentSrc, NULL);
			glCompileShader(fragmentHandle);
			int  success;
			char infoLog[512];
			glGetShaderInfoLog(vertexHandle, 512, NULL, infoLog);
			ASSERT(glGetError() == GL_NO_ERROR, "");
			glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &success);
			if (!success) {
				std::cout << "ERROR::SHADER::COMPILE_FAILED\n" << infoLog << std::endl;
			}
		}
		glAttachShader(programHandle, vertexHandle);
		glAttachShader(programHandle, fragmentHandle);
		glLinkProgram(programHandle);
		int  success;
		char infoLog[512];
		glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}
	}

	void Bind()
	{
		glUseProgram(programHandle);
		ASSERT(glGetError() == GL_NO_ERROR, "Couldn't bind shader program.");
	}

	void SetUniform(const char* uName, const glm::mat4& uValue)
	{
		GLint location = glGetUniformLocation(programHandle, uName);	// TODO: Cache this
		ASSERT(glGetError() == GL_NO_ERROR, "Couldn't get uniform location.");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uValue));
	}
};

void InitSDL()
{
	uint32_t initFlags =
		SDL_INIT_VIDEO
		| SDL_INIT_TIMER;

	int result = SDL_Init(initFlags);
	ASSERT_ZERO(result, "Failed to initialize SDL2.");
	result = SDL_GL_LoadLibrary(NULL);
	ASSERT_ZERO(result, "Failed to load SDL2 OpenGL library.");
}

void DestroySDL()
{
	SDL_Quit();
}

class Window
{
public:
	uint32_t width;
	uint32_t height;
	glm::uvec2 position;

	SDL_Window* handle;

	~Window()
	{
		if (handle)
		{
			SDL_DestroyWindow(handle);
		}
	}

	void Init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		handle = SDL_CreateWindow(
			title,
			x, y,
			width, height, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		width = width;
		height = height;
		position = glm::uvec2(x, y);
	}

	void OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		width = newWidth;
		height = newHeight;
	}
};

class Camera
{
public:
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 front = glm::vec3(0.f);
	glm::vec3 right = glm::vec3(0.f);

	float speed = 10.f;
	float anglePitch = 0.f;
	float angleYaw = glm::radians(-90.f);
	float rotationSpeed = glm::radians(40.f);
	float fov = 0.f;
	float aspectRatio = 0.f;

	glm::ivec2 moveAmounts = glm::vec2(0);
	glm::ivec2 rotateAmounts = glm::vec2(0);

	void SetPosition(float x, float y, float z)
	{
		position = glm::vec3(x, y, z);
	}

	void SetFront(glm::vec3 f)
	{
		glm::vec3 result = f;
		front = glm::normalize(result);
		right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
	}

	void Move(glm::vec3& dir, float dt)
	{
		position += (speed * dt) * dir;
	}

	void Rotate(float anglesPitch, float anglesYaw, float dt)
	{
		anglePitch += anglesPitch * dt;
		angleYaw += anglesYaw * dt;

		glm::vec3 rotationVector;
		rotationVector.x = cos(angleYaw) * cos(anglePitch);
		rotationVector.y = sin(anglePitch);
		rotationVector.z = sin(angleYaw) * cos(anglePitch);
		rotationVector = glm::normalize(rotationVector);

		SetFront(rotationVector);
	}

	void SetPerspective(float newFov, float newAspect)
	{
		fov = newFov;
		aspectRatio = newAspect;
	}

	void Init(float startX, float startY, float startZ, float startFov, float startAspect)
	{
		SetPosition(startX, startY, startZ);
		SetFront(glm::vec3(0, 0, -1));
		SetPerspective(startFov, startAspect);
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, position + front, glm::vec3(0.f, 1.f, 0.f));
	}

	glm::mat4 GetProjectionMatrix()
	{
		return glm::perspective(fov, aspectRatio, 0.1f, 1000.f);
	}

	void Update(float dt)
	{
		if (moveAmounts.x == 0 && moveAmounts.y == 0 
			&& rotateAmounts.x == 0 && rotateAmounts.y == 0)
		{
			return;
		}

		auto moveFront = (float)moveAmounts.y * front;
		auto moveSide = (float)moveAmounts.x * right;
		auto moveDir = glm::normalize(moveFront + moveSide);
		if (!glm::any(glm::isnan(moveDir)))
		{
			Move(moveDir, dt);
		}
		Rotate(rotateAmounts.x * rotationSpeed, rotateAmounts.y * rotationSpeed, dt);
	}
};

struct RenderParams
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Renderable
{
public:
	uint32_t vaoHandle = UINT32_MAX;
	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	Shader* shader;
	glm::mat4 uModel;

	void SetVertexData(Buffer* vb, Buffer* ib)
	{
		if (vaoHandle == UINT32_MAX)
		{
			glGenVertexArrays(1, &vaoHandle);
			ASSERT(glGetError() == GL_NO_ERROR, "");
		}

		vertexBuffer = vb;
		indexBuffer = ib;

		glBindVertexArray(vaoHandle);
		ASSERT(glGetError() == GL_NO_ERROR, "");
		vertexBuffer->Bind(GL_ARRAY_BUFFER);
		indexBuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)0);
		ASSERT(glGetError() == GL_NO_ERROR, "");
		glEnableVertexAttribArray(0);
		ASSERT(glGetError() == GL_NO_ERROR, "");

		glBindVertexArray(0);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}

	void SetShader(Shader* sh)
	{
		shader = sh;
	}

	void Draw(const RenderParams& params)
	{
		shader->Bind();
		shader->SetUniform("uModel", params.model);
		shader->SetUniform("uVP", params.proj * params.view);
		shader->SetUniform("uMVP", params.proj * params.view * params.model);

		glBindVertexArray(vaoHandle);
		ASSERT(glGetError() == GL_NO_ERROR, "Couldn't bind vertex array.");

		glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0);
		ASSERT(glGetError() == GL_NO_ERROR, "Failed drawing renderable.");
	}
};

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;
	Camera camera;

	std::vector<Renderable*> renderables;

	~Renderer()
	{
		if (pGlContextHandle)
		{
			SDL_GL_DeleteContext(pGlContextHandle);
		}
		if (pWindow) delete pWindow;
	}

	void CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		pWindow = new Window();
		pWindow->Init(width, height, x, y, title);
	}

	void CreateNewRenderContext()
	{
		ASSERT(!pGlContextHandle, "Trying to create new OpenGL context when renderer already has one.");
		ASSERT(pWindow, "Trying to create new OpenGL context without window associated to renderer.");
		pGlContextHandle = SDL_GL_CreateContext(pWindow->handle);
		ASSERT(pGlContextHandle, "Failed to create new OpenGL context.");
		SDL_GL_MakeCurrent(pWindow->handle, pGlContextHandle);
	}

	void RetrieveAPIFunctionLocations()
	{
		bool result = gladLoadGLLoader(SDL_GL_GetProcAddress);
		ASSERT(result, "Failed to retrieve OpenGL API function locations using GLAD.");
	}

	void SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
	{
		glViewport(x, y, width, height);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}

	void SetCamera(float x, float y, float z, float fov, float aspect)
	{
		camera.Init(x, y, z, fov, aspect);
	}

	void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
		float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		CreateNewWindow(windowWidth, windowHeight, windowX, windowY, windowTitle);
		CreateNewRenderContext();
		RetrieveAPIFunctionLocations();

		glEnable(GL_DEPTH_TEST);
		ASSERT(glGetError() == GL_NO_ERROR, "");
		glEnable(GL_CULL_FACE);
		ASSERT(glGetError() == GL_NO_ERROR, "");
		glFrontFace(GL_CCW);
		ASSERT(glGetError() == GL_NO_ERROR, "");

		SetViewport(windowWidth, windowHeight, 0, 0);
		SetCamera(cameraX, cameraY, cameraZ, cameraFOV, cameraAspect);
	}

	void OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		pWindow->OnResize(newWidth, newHeight);
		SetViewport(newWidth, newHeight, 0, 0);
	}

	void SetVertexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset)
	{
		buffer->Bind(GL_ARRAY_BUFFER);
		// TODO: Currently vertex attributes are hard set.
		uint32_t offset = firstElementOffset * elementSize;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * elementSize, (void*)offset);
		glEnableVertexAttribArray(0);
	}

	void SetIndexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset)
	{
		buffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
	}

	void AddRenderable(Renderable* renderable)
	{
		renderables.push_back(renderable);
	}

	void Render()
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		ASSERT(glGetError() == GL_NO_ERROR, "");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ASSERT(glGetError() == GL_NO_ERROR, "");

		for (int i = 0; i < renderables.size(); i++)
		{
			Renderable* renderable = renderables[i];
			RenderParams params;
			params.model = renderable->uModel;
			params.view = camera.GetViewMatrix();
			params.proj = camera.GetProjectionMatrix();

			renderable->Draw(params);
		}

		SDL_GL_SwapWindow(pWindow->handle);
	}
};

class App
{
public:
	bool isRunning;

	Renderer renderer;

	void Init()
	{
		uint32_t appWidth = APP_DEFAULT_WIDTH;
		uint32_t appHeight = APP_DEFAULT_HEIGHT;
		uint32_t screenWidth, screenHeight;
		GetDisplayDimensions(screenWidth, screenHeight);

		renderer.Init(appWidth, appHeight, (screenWidth - appWidth) / 2, (screenHeight - appHeight) / 2, "SSAO",
			0, 0, 3.f, 45.f, static_cast<float>(appWidth) / static_cast<float>(appHeight));
		Time::Init();

		isRunning = true;
	}

	void PollEvents(double dt)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				SDL_Keysym key = event.key.keysym;
				switch (key.sym)
				{
				case SDLK_ESCAPE:
				{
					isRunning = false;
				} break;
				case SDLK_w:
				{
					renderer.camera.moveAmounts.y = 1;
				} break;
				case SDLK_s:
				{
					renderer.camera.moveAmounts.y = -1;
				} break;
				case SDLK_a:
				{
					renderer.camera.moveAmounts.x = -1;
				} break;
				case SDLK_d:
				{
					renderer.camera.moveAmounts.x = 1;
				} break;
				case SDLK_UP:
				{
					renderer.camera.rotateAmounts.x = 1;
				} break;
				case SDLK_DOWN:
				{
					renderer.camera.rotateAmounts.x = -1;
				} break;
				case SDLK_LEFT:
				{
					renderer.camera.rotateAmounts.y = -1;
				} break;
				case SDLK_RIGHT:
				{
					renderer.camera.rotateAmounts.y = 1;
				} break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				SDL_Keysym key = event.key.keysym;
				switch (key.sym)
				{
				case SDLK_w:
				case SDLK_s:
				{
					renderer.camera.moveAmounts.y = 0;
				} break;
				case SDLK_a:
				case SDLK_d:
				{
					renderer.camera.moveAmounts.x = 0;
				} break;
				case SDLK_UP:
				case SDLK_DOWN:
				{
					renderer.camera.rotateAmounts.x = 0;
				} break;
				case SDLK_LEFT:
				case SDLK_RIGHT:
				{
					renderer.camera.rotateAmounts.y = 0;
				} break;
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					uint32_t newWidth = event.window.data1;
					uint32_t newHeight = event.window.data2;
					renderer.OnResize(newWidth, newHeight);
				}
			}
			else if (event.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}
	}

	void Run()
	{
		Buffer vb;
		vb.Init(GL_ARRAY_BUFFER, sizeof(g_quadVertices), sizeof(g_quadVertices) / sizeof(float), g_quadVertices);
		Buffer ib;
		ib.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_quadIndices), sizeof(g_quadIndices) / sizeof(uint32_t), g_quadIndices);

		Renderable obj;
		obj.uModel = glm::mat4(1.f);
		obj.SetVertexData(&vb, &ib);
		Shader objShader;
		
		char vertSrc[256];
		char fragSrc[256];
		FileReader::ReadFile(RESOURCES_PATH"shaders/default_vert.glsl", vertSrc);
		FileReader::ReadFile(RESOURCES_PATH"shaders/default_frag.glsl", fragSrc);

		objShader.InitAndCompile(vertSrc, fragSrc);
		obj.SetShader(&objShader);

		renderer.AddRenderable(&obj);

		while (isRunning)
		{
			Time::UpdateTime();
			PollEvents(Time::deltaTime);
			renderer.camera.Update(Time::deltaTime);
			// TODO: update logic here
			renderer.Render();
		}
	}
};

int main(int argc, char* argv[])
{
	InitSDL();
	App app;
	app.Init();

	app.Run();

	DestroySDL();
	return 0;
}