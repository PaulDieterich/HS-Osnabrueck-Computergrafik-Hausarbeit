#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <GL/GLU.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <stdio.h>
#include "Application.h"
#include "freeimage.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "ui.h"
#include "uiMenu.h"


void PrintOpenGLVersion();

int main () {
   
    FreeImage_Initialise();
    srand(time(NULL));
    //gluPerspective(45.0f, (GLfloat)UNIVERSE_SIZE / (GLfloat)UNIVERSE_SIZE, 0.5f, 3000000.0f);
    //glFrustum((GLdouble)-UNIVERSE_SIZE*20, (GLdouble)UNIVERSE_SIZE*20, (GLdouble)-UNIVERSE_SIZE*20, (GLdouble)UNIVERSE_SIZE*20, (GLdouble)UNIVERSE_SIZE*5000.0, (GLdouble)UNIVERSE_SIZE * 5000.0);
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    const char* glsl_version = "#version 130";
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif   
    bool shouldRestart = false;
    int WindowWidth = 780;
    int WindowHeight = 600;
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Computergrafik - Hochschule Osnabrück", NULL, NULL);
    glfwSetWindowPos(window, WindowWidth / 2, WindowHeight / 2);

    if (!window) {
        fprintf (stderr, "ERROR: can not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);
    
#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif
    PrintOpenGLVersion();
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Maus verstecken
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Main game loop
    gameloop:
    {
       double lastTime = 0; UI ui;
       Application App(window,ui);
       App.start();
      
       ui.addIcons();
       while (!glfwWindowShouldClose (window) && !shouldRestart) {
           
            double now = glfwGetTime();
            double delta = now - lastTime;
            lastTime = now;
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            App.update((float)delta, (float)now, ui, shouldRestart);
            App.draw((float)delta);
           
           ImGui::Render();
           int display_w, display_h;
           glfwGetFramebufferSize(window, &display_w, &display_h);
           glViewport(0, 0, display_w, display_h);
           ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

           glfwSwapBuffers(window);
        }
      App.end();
      // Wenn neu gestartet wird dann wird auch ein neues Application Objekt erzeugt
      if (shouldRestart) {
          log(INFO, "Spiel wird neu gestartet");
          shouldRestart = false;
          goto gameloop;
      }

      glfwDestroyWindow(window);
    }

    glfwTerminate();
    return 0;
}

void PrintOpenGLVersion()
{
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
}
