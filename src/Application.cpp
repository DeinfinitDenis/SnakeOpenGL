/*


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

float randomFloat(float minValue, float maxValue, int ib) 
{
    mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count() + ib);
    uniform_real_distribution<float> dist(minValue, maxValue);
    return dist(gen);
}
static unsigned int CompileShader(unsigned int type, const string& source )
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!";
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



class Player
{
public:
    float xDiff = 0;
    float yDiff = 0;
    double xMouseDiff = 0;
    double yMouseDiff = 0;
    float x, y;
private:
    float speed;
    float health;
    unsigned int shader;
    unsigned int buffer;
    unsigned int ibo;
    float pos[8] =
    {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int ind[6] =
    {
        0,1,2,
        2,3,0
    };
public:
    Player(float x, float y, float speed)
    {
        this->x = x;
        this->y = y;
        this->speed = speed;
        health = 100;

        string vertexShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            " gl_Position = position;\n"
            "}\n";
        string fragmentShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            " color = vec4(0.0, 0.0, 1.0, 1.0);\n"
            "}\n";
        shader = CreateShader(vertexShader, fragmentShader);

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    }

    

    void Update() 
    {
        x += xDiff;
        y += yDiff;
        xDiff = 0;
        yDiff = 0;
        Draw();
    }

private:
    void Draw() 
    {
        pos[0] = x;
        pos[1] = y;
        pos[2] = x + 0.05f;
        pos[3] = y;
        pos[4] = x + 0.05f;
        pos[5] = y + 0.05f;
        pos[6] = x;
        pos[7] = y + 0.05f;

        ind[0] = 0;
        ind[1] = 1;
        ind[2] = 2;
        ind[3] = 2;
        ind[4] = 3;
        ind[5] = 0;

        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), pos, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), ind, GL_STATIC_DRAW);
        glUseProgram(shader);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    }
};



int main() 
{

    if (!glfwInit())return -1;

    GLFWwindow* window;

    window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        return -1;
    
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        cout << "Glew not working" << endl;
        return -1;
    }

    //glfwSetKeyCallback(window, key_callback);

    

    while (!glfwWindowShouldClose(window))
    {
        

        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            

        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        {
            
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            
        }

        

        //glfwGetCursorPos(window, &player.xMouseDiff, &player.yMouseDiff);
        

        

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            
            
            //proj.dirX = player.xMouseDiff;
            //proj.dirY = player.yMouseDiff;

        }

        

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}

*/