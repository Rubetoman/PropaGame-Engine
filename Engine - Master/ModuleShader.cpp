#include "ModuleShader.h"
#include "Application.h"

ModuleShader::ModuleShader()
{
	for (unsigned i = 0; i < PROGRAM_COUNT; ++i)
	{
		programs[i] = 0;
	}
}

// Destructor
ModuleShader::~ModuleShader()
{
}

bool ModuleShader::Init()
{
	programs[DEFAULT] = LoadShaders("Assets/Shaders/default.vs", "Assets/Shaders/default.fs");
	//programs[FLAT] = LoadShaders("Assets/Shaders/flat.vs", "Assets/Shaders/flat.fs");
	//programs[GOURAUD] = LoadShaders("Assets/Shaders/gouraud.vs", "Assets/Shaders/gouraud.fs");
	//programs[PHONG] = LoadShaders("Assets/Shaders/phong.vs", "Assets/Shaders/phong.fs");
	//programs[BLINN] = LoadShaders("Assets/Shaders/blinn.vs", "Assets/Shaders/blinn.fs");

	bool ok = true;

	/*for (unsigned i = 0; ok && i < PROGRAM_COUNT; ++i)
	{
		ok = programs[i] != 0;
	}*/

	return ok;
}

GLuint ModuleShader::LoadShaders(const char* vertShaderPath, const char* fragShaderPath) {

	// How to: https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint compileStatus = GL_FALSE;
	int logLength = 0;

	char* vertShaderStr = ReadShaderFile(vertShaderPath);
	char* fragShaderStr = ReadShaderFile(fragShaderPath);

	if (vertShaderStr == nullptr || fragShaderStr == nullptr) {
		LOG("Error: Reading shaders failed");
		return GL_FALSE;
	}

	// Compile shaders
	glShaderSource(vertShader, 1, &vertShaderStr, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		LOG("Error: Failed compiling vertex shader");
		CheckCompilationErrors(vertShader);
		return GL_FALSE;
	}

	glShaderSource(fragShader, 1, &fragShaderStr, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		LOG("Error: Failed compiling fragment shader");
		CheckCompilationErrors(fragShader);
		return GL_FALSE;
	}

	// Compile Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compileStatus);

	// Remove shaders, we wont need them anymore if they are loaded correctly into Program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

char* ModuleShader::ReadShaderFile(const char* shaderPath) 
{
	FILE* file;
	errno_t err = fopen_s(&file, shaderPath, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		char* shaderData = (char*)malloc(size + 1);
		fread(shaderData, 1, size, file);
		shaderData[size] = 0;
		fclose(file);
		return shaderData;
	}

	return nullptr;
}

void ModuleShader::CheckCompilationErrors(GLuint shader) 
{
	GLint infoLogLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	GLchar* strInfoLog = new GLchar[infoLogLength + 1];
	glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

	LOG(strInfoLog);

	delete[] strInfoLog;
	infoLogLength = NULL;
	glDeleteShader(shader); // Don't leak the shader.
}

