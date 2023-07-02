#include "ShaderLoader.h"
#include "Engine.h"
#ifdef _WIN32
#include "glex_impl.h"
#else
#include<OpenGL/glext.h>
#endif







ShaderLoader::ShaderLoader(void)
{
}

ShaderLoader::~ShaderLoader(void)
{
}

GLhandleARB ShaderLoader::loadShader(char* filename, unsigned int type)
{
	string completeFilePath = Engine::engine->shaderFolder +  string(filename);
	
	//printf("Loading Shader file:'%s'.\n",completeFilePath.c_str());

   ifstream file;
    string line;
	string fileString;
   file.open(completeFilePath.c_str());

	 if (!file)
	{
		printf("Could not load Shader file:'%s'\n",completeFilePath.c_str());
		return 0;
	}

   while (getline(file, line) )
		fileString += line + "\n";
   
   //Create a handle.
	GLhandleARB vertexHandle;
 
	//Obtain a valid handle to a vertex shader object.
	vertexHandle = glCreateShaderObjectARB(type);
	if (!vertexHandle)
	{
		//We have failed creating the vertex shader object.
		printf("Failed creating vertex shader object from file: %s.",filename);
	}

	const GLcharARB* files[1] = {(const GLcharARB*)(fileString.c_str())};
	//Now, compile the shader source. 
	//Note that glShaderSource takes an array of chars. This is so that one can load multiple vertex shader files at once.
	//This is similar in function to linking multiple C++ files together. Note also that there can only be one "void main" definition
	//In all of the linked source files that are compiling with this funciton.
	glShaderSourceARB(
		vertexHandle, //The handle to our shader
		1, //The number of files.
		files, //An array of const char * data, which represents the source code of theshaders
		NULL); //An array of string lengths. For have null terminated strings, pass NULL.
	 
	//Attempt to compile the shader.
	glCompileShaderARB(vertexHandle);
	 
	//Error checking.
	GLint result;
	glGetObjectParameterivARB(vertexHandle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	if (!result)
	{
		//We failed to compile.
		printf("Vertex shader '%s' failed compilation.\n",filename);
		//Attempt to get the length of our error log.
		GLint length;
		glGetObjectParameterivARB(vertexHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
		//Create a buffer.
		vector<char> buffer;
		buffer.resize(length);
	 
		//Used to get the final length of the log.
		GLsizei final;
		glGetInfoLogARB(vertexHandle, length, &final, &buffer[0]);
		//Convert our buffer into a string.
		string message(&buffer[0], length);
		printf("%s\n",message.c_str());
		if (final > length)
		{
			//The buffer does not contain all the shader log information.
			printf("Shader Log contained more information!\n");
		}
		
	}

	return vertexHandle;
}