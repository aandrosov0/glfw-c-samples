#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char *TITLE = "Simple triangle Program";
const int WIDTH = 640, HEIGHT = 480;

float points[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

void check(int result, char *error) {
  if(result) return;
  printf("Can`t be initialized - %s\n", error);
  exit(1);
}

GLuint getShaderProgram() {
  const char *vertexShaderSource =
  "#version 330\n"
  "in vec3 vp;"
  "void main() {"
  " gl_Position = vec4(vp, 1.0);"
  "}";

  const char *fragmentShaderSource =
  "#version 330\n"
  "out vec4 color;"
  "void main() {"
  " color = vec4(0.5, 0.0, 0.5, 1.0);"
  "}";

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, fragmentShader);
  glAttachShader(shaderProgram, vertexShader);
  glLinkProgram(shaderProgram);

  return shaderProgram;
}

GLuint getTriangleVAO() {
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  return vao;
}

int main() {
  check(glfwInit(), "GLFW3");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  check(window != NULL, "main Window");

  glfwMakeContextCurrent(window);
  glewInit();

  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  printf("Renderer: %s\nOpenGL version supported %s\n", renderer, version);

  GLuint shaderProgram = getShaderProgram();
  GLuint vao = getTriangleVAO();

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
