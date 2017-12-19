#include "opengl.h"
#include "mesh_processing_app.h"
#include <iostream>
Viewer* v;

int WIDTH = 600;
int HEIGHT = 600;

int g_pixel_ratio = 1;

static void char_callback(GLFWwindow* /*window*/, unsigned int key)
{
  v->charPressed(key);
}

static void scroll_callback(GLFWwindow* /*window*/, double x, double y)
{
  v->mouseScroll(x,y);
}

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int mods)
{
  if ((key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
  }

  v->keyEvent(key,action,mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  v->mousePressed(window, button, action, mods);
}

void cursorPos_callback(GLFWwindow* /*window*/, double x, double y)
{
  v->mouseMoved(x*g_pixel_ratio,y*g_pixel_ratio);
}

void reshape_callback(GLFWwindow* window, int width, int height)
{
  v->reshape(width,height);
  v->updateScene();
  glfwSwapBuffers(window);
}

// initialize GLFW framework
GLFWwindow* initGLFW()
{
  if (!glfwInit())
      exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (GLint)GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Surface Modeling", NULL, NULL);
  if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glbinding::Binding::initialize();
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  // Handle retina display:
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  if(w==2*WIDTH)
    g_pixel_ratio = 2;

  // callbacks
  glfwSetKeyCallback(window, key_callback);
  glfwSetCharCallback(window, char_callback);
  glfwSetFramebufferSizeCallback(window, reshape_callback);
  glfwSetCursorPosCallback(window, cursorPos_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);

  return window;
}

static void error_callback(int /*error*/, const char* description)
{
  fputs(description, stderr);
}


int main (int argc, char **argv)
{
  glfwSetErrorCallback(error_callback);

  GLFWwindow* window = initGLFW();
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);

  v = new MeshProcessingApp();

  v->init(w,h,argc,argv);

  double t0 = glfwGetTime();
  double t1 = t0;
  while (!glfwWindowShouldClose(window))
  {
    // render the scene
    t1 = glfwGetTime();
    if((t1-t0)>0.04) {
      v->updateScene();
      glfwSwapBuffers(window);
      t0 = t1;
    }
    glfwPollEvents();
  }

  delete v;

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

