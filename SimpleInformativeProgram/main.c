#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char *TITLE = "Simple informative Program";
const int WIDTH = 640, HEIGHT = 480;

void check(int result, char *error) {
  if(result) return;
  printf("Can`t be initialized - %s\n", error);
  exit(1);
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

  glfwTerminate();
  return 0;
}
