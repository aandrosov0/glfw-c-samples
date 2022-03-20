#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int WIDTH = 640, HEIGHT = 480;
const char *TITLE = "Hot reloading shaders";

void emitError(char *error) {
  printf("%s\n", error);
  exit(1);
}

void check(int result, char *error) {
  if(result) return;
  emitError(strcat("Can`t be initializated - ", error));
}

const int getFileSize(char *fileName) {
  FILE *file = fopen(fileName, "r");
  fseek(file, 0L, SEEK_END);

  const int SIZE = ftell(file);

  fclose(file);
  return SIZE;
}

void readFile(char *fileName, char **buffer) {
  FILE *file = fopen(fileName, "r");
  const int SIZE = getFileSize(fileName);

  *buffer = (char*) malloc(sizeof(char) * SIZE);
  fread(*buffer, sizeof(char), SIZE, file);
  (*buffer)[SIZE] = '\0';

  fclose(file);
}

void checkGL(GLuint object, void (*getIv) (GLuint, GLenum, GLint*), void (*getInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*), GLenum status) {
  int success;
  char infoLog[512];
  getIv(object, status, &success);

  if(!success) {
    getInfoLog(object, 512, NULL, infoLog);
    printf("%s\n", infoLog);
  }
}

void checkShaderCompile(GLuint shader) {
  checkGL(shader, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

void checkProgramLinking(GLuint program) {
  checkGL(program, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

GLuint createShader(GLenum shaderType, const char* shaderSource) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  checkShaderCompile(shader);
  return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  checkProgramLinking(program);
  return program;
}

void reloadProgram(GLuint *shaderProgram, char *vertexShaderSource, char *fragmentShaderSource) {
  GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  glDeleteProgram(*shaderProgram);
  *shaderProgram = createProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

int main() {
  check(glfwInit(), "GLFW3");

  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  check(window != NULL, "Window");

  glfwMakeContextCurrent(window);
  glewInit();

  float points[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
  };

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint shaderProgram;
  char *vertexShaderSource = NULL, *fragmentShaderSource = NULL;

  while(!glfwWindowShouldClose(window)) {
    readFile("shader.vert", &vertexShaderSource);
    readFile("shader.frag", &fragmentShaderSource);

    glUseProgram(shaderProgram);
    reloadProgram(&shaderProgram, vertexShaderSource, fragmentShaderSource);

    free(vertexShaderSource);
    free(fragmentShaderSource);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
}
