#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "shaders.h"
#include <iostream>
#include "stb_image.h"
using namespace std;

/*
*	Callback function to resize the GLFW window.
*	Prototype for the function is here:
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//add body  for the function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
/*
* Function for Input Control
* Getkey will take window along witha key as input.
* Function will be named processInput to handle all such code and keep code organzied.
* This function should be near the top of the loop as it should be the first task carried out by the program.
* If we it is below in the order then, the function will run after carrying out all other tasks above it.
*/
void processInput(GLFWwindow* window)
{
	/*
	* we are checking if user has used escape key.
	* if pressed we will close the window.
	*/

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

//main code file
int main()
{
	//Initaite GLFW and configure it
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Window Object and check if its working.
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//
	//Initialize GLAD and check if its working. 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	////////////*******END OF SAFTEY CHECKS****************///////////////

	/*
	Rendering Window size
	X0,Y0,X1,Y1
	First two are the parameters of lower left corner
	and the last two are the width and height of window in pixels
	*/
	glViewport(0, 0, 800, 600);
	//calling the resize function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//build and compile shader
	Shader ourShader("vertex.s", "fragment.s");
	/*
	*If we want to see the verticesw only then we can use wirefram to see how the objects are connected
	* We can use polygon mode to see that
	* Parameters:
	* if we want to apply it to front and back
	* if we want to fill or draw lines
	* Comment it out to go back to normal mode
	*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//----------------------------------- Vertex and config data here 

	//Comments removed for ease of use in excercise

	float vertices[] = {
		// positions // colors // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.55f, 0.55f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.55f, 0.45f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.45f, 0.45f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.45f, 0.55f // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//---Config for First triangle
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Textures
	/*
	*Generate Texture ID like we have done prevously for VBO,VAO etc
	* glGenTextures takes the number of textures we are going to have
	* then we bind it
	*
	* Then we generate the texture using glTexImage2D
	* Parameters:
	* Target
	* Mipmap level,base level is 0. This is manual. We can use glGenerateMipmap to do this auto for us
	* Format of texure to be stored in. We using RGB so it makes sense to use RGB for textures
	* Width
	* Height
	* No idea given but it is said to be legacy stuff. Never be anything else then 0.
	* Format of source file
	* Datatype of source file
	* Image data
	* glGenerateMipmap is used to create smaller textures so that we dont waste memory
	as the objects which are far away dont need objects to be given high quality textures as well.
	* remove the image data as its good practice to free up memory for performance.
	*/
	//Texture 1
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

//flip images
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// change GL REPEAT to the type we want.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("D:/PRIVATE PROJECT/Learning OpenGL/Files/Textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//Texture 2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// change GL REPEAT to the type we want.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	data = stbi_load("D:/PRIVATE PROJECT/Learning OpenGL/Files/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);

	//ourShader.setInt("texture2", 1);

	//ourShader.setInt("texture2", 1);
	/*
	* Code to only allow explict closing.
	* It is also the render loop
	*/
	while (!glfwWindowShouldClose(window))
	{
		/*
		*input
		*/
		processInput(window);

		/*
		* render
		* MAke sure that clearcolor is before anything else as it will be creating the background
		*/

		//glclear will clear the buffer with the collor given in glclearColor. It is also coloring the background piels
		//The parameters are RGBA meaning Red,Green,Blue and Alpha
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Bind  Vertix/Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		//Update Shader input

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		/*
		* GL Draw Array has been commented out as we are going to use EBO which will
		remove the overhead of having same vertices for many triangles.
		*/

		/*
		*check call events
		*/
		//swapbuffers will swap the color buffer
		glfwSwapBuffers(window);
		//Pollevents check if window has been closed or not.
		glfwPollEvents();
	}
	//terminate GLFW at end of program
	glfwTerminate();
	return 0;
}