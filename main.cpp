#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>

const char* vertSrc = "#version 330 core\nlayout(location=0)in vec3 p;void main(){gl_Position=vec4(p,1);}";
const char* fragSrc = "#version 330 core\nout vec4 c;uniform vec3 color;void main(){c=vec4(color,1);}";

struct Triangle {
    glm::vec2 verts[3];
    glm::vec3 color;
};

std::vector<Triangle>  triangles;
std::vector<glm::vec2> pending;   // vértices ainda não formaram triângulo

int winW = 800, winH = 600;

void mouseCallback(GLFWwindow* win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mx, my;
        glfwGetCursorPos(win, &mx, &my);

        float nx =  (float)(mx / winW) * 2.0f - 1.0f;
        float ny = 1.0f - (float)(my / winH) * 2.0f;

        pending.push_back({ nx, ny });

        if (pending.size() == 3) {
            triangles.push_back({
                { pending[0], pending[1], pending[2] },
                { (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX }
            });
            pending.clear();
        }
    }
}

GLuint createVAO(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    float verts[] = {
        a.x, a.y, 0,
        b.x, b.y, 0,
        c.x, c.y, 0
    };
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    return VAO;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(winW, winH, "Triângulos por Clique", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetMouseButtonCallback(window, mouseCallback);

    // Shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSrc, NULL); glCompileShader(vert);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc, NULL); glCompileShader(frag);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert); glAttachShader(prog, frag); glLinkProgram(prog);

    GLint colorLoc = glGetUniformLocation(prog, "color");

    std::vector<GLuint> triVAOs;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        while (triVAOs.size() < triangles.size()) {
            auto& t = triangles[triVAOs.size()];
            triVAOs.push_back(createVAO(t.verts[0], t.verts[1], t.verts[2]));
        }

        glClearColor(0.1f, 0.f, 0.3f, 1); glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);

        for (size_t i = 0; i < triVAOs.size(); i++) {
            glUniform3f(colorLoc,
                triangles[i].color.r,
                triangles[i].color.g,
                triangles[i].color.b);
            glBindVertexArray(triVAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window); glfwPollEvents();
    }

    glfwTerminate();
}