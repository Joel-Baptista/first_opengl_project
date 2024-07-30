#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
// This macro gives a useful error message when an OpenGL function fails
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError(){
    while (glGetError() != GL_NO_ERROR); // Clear all the errors
}

static bool GLLogCall(const char* function, const char* file, int line){
    while (GLenum error = glGetError()){
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    };
    return true;
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource parse_shader(const std::string& filepath){
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)){
        if (line.find("#shader") != std::string::npos){
            if (line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT; 
            }
        }else{
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type); // Create a shader, that returns its ID
    const char* src = source.c_str(); // Get the pointer of the first character of the code
    glShaderSource(id, 1, &src, nullptr); // Set the source code of the shader
    glCompileShader(id); // Compile the shader

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // Get the result of the compilation
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Get the length of the error message
        char message[length]; // Create a char array with the length of the error message
        glGetShaderInfoLog(id, length, &length, message); // Get the error message
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id); // Delete the shader
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    /* This string are literally the source code for the shaders. We can have this strings hardcoded, or read them from files*/
    unsigned int program = glCreateProgram(); // Create a program, that returns its ID
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); // Create a vertex shader, that returns its ID
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // Create a vertex shader, that returns its ID

    glAttachShader(program, vs); // Attach the vertex shader to the program
    glAttachShader(program, fs); // Attach the fragment shader to the program
    glLinkProgram(program); // Link the program
    glValidateProgram(program); // Validate the program

    // Shaders are no longer needed after linking
    glDeleteShader(vs); // Delete the vertex shader
    glDeleteShader(fs); // Delete the fragment shader

    return program;
}

int main(void)
{   
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Enable VSync

    // Initialize GLEW only after creating the window
    if (glewInit() != GLEW_OK) 
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create a buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer); // Instantiate a buffer and store its ID  
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select the buffer
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW); // Fill the buffer with data

    // We only have one attribute (position), so we just need to do this once
    glEnableVertexAttribArray(0); // Enable the attribute (position)
    glVertexAttribPointer(
        0, // Index of the attribute (position) 
        2, // Number of components per attribute (x, y)
        GL_FLOAT, // Type of the components
        GL_FALSE, // Normalize the components (Floats are already normalized, but maybe we want to normalize colors)
        sizeof(float) * 2, // Stride (The ammount we need to go to get to the next vertex)
        (const void*)0 // Pointer (Location of the attribute inside the vertex)
    );

    unsigned int ibo; // Index buffer object
    glGenBuffers(1, &ibo); // Instantiate a buffer and store its ID  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Select the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // Fill the buffer with data

    ShaderProgramSource source = parse_shader("res/shaders/basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader)); // Use the program

    int location = glGetUniformLocation(shader, "u_Color"); // Get the location of the uniform
    ASSERT(location != -1); // Check if the uniform exists
    glUniform4f(location, 0.2f, 0.3, 0.8f, 1.0f); // Set the uniform 4f (four floats)

    float r = 0.0f;
    float increment = 0.05f;
    // glBindBuffer(GL_ARRAY_BUFFER, 0); // Select no buffer
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUniform4f(location, r, 0.3, 0.8f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Draw the elements

        if (r > 1.0f){
            r = 1.0f;
            increment = -0.05f;
        }else if (r < 0.0f){
            r = 0.0f;
            increment = 0.05f;
        }
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); // Delete the program

    glfwTerminate();
    return 0;
}
