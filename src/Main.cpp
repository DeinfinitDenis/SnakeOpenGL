#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

float randomFloat(float minValue, float maxValue, int ib)
{
    mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count() + ib);
    uniform_real_distribution<float> dist(minValue, maxValue);
    return dist(gen);
}
static unsigned int CompileShader(unsigned int type, const string& source)
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

class Background
{
private:
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
    float size;

    Background(int size)
    {
        this->size = 16;


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
            " color = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "}\n";

        shader = CreateShader(vertexShader, fragmentShader);

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    }
private:
    void Draw()
    {

        pos[0] = -1;
        pos[1] = -1;
        pos[2] = 1;
        pos[3] = -1;
        pos[4] = 1;
        pos[5] = 1;
        pos[6] = -1;
        pos[7] = 1;

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


        for (int i = 0; i < size; i++)for (int j = 0; j < size; j++)
        {
            float x = ((2 / size) * i) - 1;
            float y = ((2 / size) * j) - 1;
        }


    }
public:


    void Update()
    {
        Draw();

    }
};
class Grid 
{
private:
    unsigned int shaderNull;
    unsigned int shaderSnakeTail;
    unsigned int shaderSnakeHead;
    unsigned int shaderFood;
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
    float size;

    Grid(int size)
    {
        this->size = 16;


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
            " color = vec4(0.0, 0.0, 0.0, 1.0);\n"
            "}\n";

        shaderNull = CreateShader(vertexShader, fragmentShader);

        vertexShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            " gl_Position = position;\n"
            "}\n";
        fragmentShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            " color = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "}\n";

        shaderSnakeTail = CreateShader(vertexShader, fragmentShader);

        vertexShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            " gl_Position = position;\n"
            "}\n";
        fragmentShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            " color = vec4(0.0, 0.0, 0.0, 0.0);\n"
            "}\n";

        shaderSnakeHead = CreateShader(vertexShader, fragmentShader);

        vertexShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            " gl_Position = position;\n"
            "}\n";
        fragmentShader =
            "#version 330 core\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            " color = vec4(1.0, 1.0, 0.0, 1.0);\n"
            "}\n";

        shaderFood = CreateShader(vertexShader, fragmentShader);

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    }
private:
    void Draw(int matx[16][16])
    {
        for (int i = 0; i < size; i++)for (int j = 0; j < size; j++) 
        {
            float x = ((2 / size) * i) - 1;
            float y = ((2 / size) * j) - 1;
            pos[0] = x;
            pos[1] = y;
            pos[2] = x + (2 / size) - 0.01f;
            pos[3] = y;
            pos[4] = x + (2 / size) - 0.01f;
            pos[5] = y + (2 / size) - 0.01f;
            pos[6] = x;
            pos[7] = y + (2 / size) - 0.01f;

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
            glUseProgram(shaderNull);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            

            switch (matx[i][j]) 
            {
            case 1: //Snake tail
                pos[0] = x + 0.015f;
                pos[1] = y + 0.015f;
                pos[2] = x + (2 / size) - 0.025f;
                pos[3] = y + 0.015f;
                pos[4] = x + (2 / size) - 0.025f;
                pos[5] = y + (2 / size) - 0.025f;
                pos[6] = x + 0.015f;
                pos[7] = y + (2 / size) - 0.025f;

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
                glUseProgram(shaderSnakeTail);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                break;
            case 2: //Snake head

                pos[0] = x + 0.015f;
                pos[1] = y + 0.015f;
                pos[2] = x + (2 / size) - 0.025f;
                pos[3] = y + 0.015f;
                pos[4] = x + (2 / size) - 0.025f;
                pos[5] = y + (2 / size) - 0.025f;
                pos[6] = x + 0.015f;
                pos[7] = y + (2 / size) - 0.025f;

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
                glUseProgram(shaderSnakeTail);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                pos[0] = x + 0.025f;
                pos[1] = y + 0.025f;
                pos[2] = x + (2 / size) - 0.035f;
                pos[3] = y + 0.025f;
                pos[4] = x + (2 / size) - 0.035f;
                pos[5] = y + (2 / size) - 0.035f;
                pos[6] = x + 0.025f;
                pos[7] = y + (2 / size) - 0.035f;

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
                glUseProgram(shaderSnakeHead);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                break;

            case 3: //Food

                pos[0] = x + 0.035f;
                pos[1] = y + 0.035f;
                pos[2] = x + (2 / size) - 0.045f;
                pos[3] = y + 0.035f;
                pos[4] = x + (2 / size) - 0.045f;
                pos[5] = y + (2 / size) - 0.045f;
                pos[6] = x + 0.035f;
                pos[7] = y + (2 / size) - 0.045f;

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
                glUseProgram(shaderFood);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                break;
            }
        }
    }
public:
    

    void Update(int matx[16][16])
    {
        Draw(matx);
    
    }
};
class SnakeBody 
{
public:
    int x, y;
    SnakeBody(int x, int y) 
    {
        this->x = x;
        this->y = y;
    }

};
class SnakeHead 
{
public:
    int x, y;
    int score;
    float px, py;
    vector<SnakeBody> body;

public:
    SnakeHead(int x, int y) 
    {
        this->x = x;
        this->y = y;
        score = 0;
    }

    void Update(int lastKey) 
    {
        int i = 0;
        int temp_x, temp_y;
        for (auto& element : body) 
        {
            
            if (i == 0)
            {
                
                element.x = this->x;
                element.y = this->y;
                temp_x = element.x;
                temp_y = element.y;
            }
            else 
            {
                int dx, dy;
                dx = element.x;
                dy = element.y;
                element.x = temp_x;
                element.y = temp_y;
                temp_x = dx;
                temp_y = dy;

                
            }
            i++;
        }
    }
};
class Food 
{
public:
    int x, y;
    Food(int x, int y) 
    {
        this->x = x;
        this->y = y;
    }
};


int main()
{
    
    bool dead = false;
    int lastKey = 0;
    int matx[16][16];
    float speed = 2;
    if (!glfwInit())return -1;
    GLFWwindow* window;
    window = glfwCreateWindow(400, 400, "Window", NULL, NULL);
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
    Grid gr(32);
    Background bg(32);
    SnakeHead sna(6,7);
    Food foof(round(randomFloat(1, 15, 20)), round(randomFloat(1, 15, 19)));
    double deltaTime = 0;
    double timeSinceLastMove = 0;
    while (!glfwWindowShouldClose(window) && !dead)
    {
        
        auto startTime = std::chrono::high_resolution_clock::now();
        timeSinceLastMove += deltaTime;
        glClear(GL_COLOR_BUFFER_BIT);
        for (int i = 0; i < 16; i++)for (int j = 0; j < 16; j++) matx[i][j] = 0;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            lastKey = 1;
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            lastKey = 3;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            lastKey = 4;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            lastKey = 2;
        }
        if (timeSinceLastMove >= 0.3)
        {
            switch (lastKey)
            {
            default:
                break;
            case 1:
                sna.Update(lastKey);
                sna.x -= 1;

                break;
            case 2:
                sna.Update(lastKey);
                sna.y -= 1;
                
                break;
            case 3:
                sna.Update(lastKey);
                sna.x += 1;
                
                break;
            case 4:
                sna.Update(lastKey);
                sna.y += 1;
                
                break;
            }
            timeSinceLastMove = 0;
        }
        
        bg.Update();
        
        for (auto& element : sna.body)
        {
            matx[element.x][element.y] = 1;
        }
        sna.px = sna.x;
        sna.py = sna.y;
        matx[foof.x][foof.y] = 3;
        if (matx[sna.x][sna.y] == 3) 
        {
            do
            {
                foof.x = round(randomFloat(1, 15, 20));
                foof.y = round(randomFloat(1, 15, 21));
            
            } while (matx[foof.x][foof.y] == 1);
            
            sna.body.push_back(SnakeBody(0, 0));
            sna.score++;
            bool ijh = PlaySound(TEXT("1.wav"), NULL, SND_ASYNC);
        }
        else if (matx[sna.x][sna.y] == 1) 
        {
            dead = true;
        
        }
        matx[sna.x][sna.y] = 2;
        gr.Update(matx);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        auto endTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<double>(endTime - startTime).count();
    }
    glfwTerminate();
    return 0;
}