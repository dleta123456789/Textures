//in case the file is already compiled, this header file will not run with ifndef
#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // To get required OpenGl headers
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

class Shader
{
public:
	unsigned int ID;//program id
	//Read and build the shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//Source code for fragment and vertex shader.
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//add abilty to utilize exceptions by ifstream for vertex and fragment
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			//read contents of file into the stream.
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//close  files
			vShaderFile.close();
			fShaderFile.close();

			//convert into string from stream
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		//give out error in case ifstream error
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//create shaders like we did in the code for triangles main.cpp .
		int success;
		char infoLog[512];
		//vertex
		unsigned int vertex;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		};
		//fragment now
		unsigned int fragment;
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		/*
		* Also check for errors here!
		* Same method for checking erros for fragment like with vertexshader.
		*/
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}
		//shader program

		//get id
		ID = glCreateProgram();
		//attach vertex and fragment shader
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		//link program to shaderobject
		glLinkProgram(ID);
		// check if its working
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}
		// delete shaders as they are not needed anymore
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	//Activate shader
	void use()
	{
		glUseProgram(ID);
	}

	//Utility functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}



};
#endif