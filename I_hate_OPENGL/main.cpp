#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include "VBO.h"
#include "IBO.h"





struct shaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};


static shaderSource parseShader(const std::string& filepath) {

    enum ShaderType {

        NONE = -1, VERTEX = 0, FRAGMENT = 1
	};


    std::fstream stream(filepath);
    std::string line;
    
    std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

    while(std::getline(stream, line)) {
        
        if (line.find("#shader") != std::string::npos) {


            if (line.find("vertex") != std::string::npos) {

				type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {

				type = ShaderType::FRAGMENT;

            }
            
        }
        else {
            if (type != NONE) {
                ss[(int)type] << line << '\n';
            }
        }
    }

	return { ss[0].str(), ss[1].str() };    
}

unsigned int compileShader( unsigned int type, const std::string& source) {

    unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "Failed to compile shader!"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
			std::cout << message << std::endl;

			glDeleteShader(id);

            return 0;

            
        
        }

    return id;
}

static  unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);


	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}


int main(void)
{

    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

	 

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f, 
         0.5f, -0.5f,
        -0.5f,  0.5f,
		 0.5f,  0.5f,
	};

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2
	};
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	VertexBuffer vb(positions, 4 * 2 * sizeof(float));
	

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

    IndexBuffer ib(indices, 6);
    
	
    
    
    shaderSource source = parseShader("rec/Shaders/Basic.shader");
    
	
    unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	_ASSERT(location != -1);
	glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);

	glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float r = 0.0f;
    float i = 0.5f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        glUseProgram(shader);
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);


        
        
		glBindVertexArray(VAO);
        ib.Bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        

		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, nullptr);
        
        if (r > 1.0f) {
            i = -0.05f;
        }
        else if (r < 0.0f) {
			i = 0.05f;
        }

		r += i;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
	glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
