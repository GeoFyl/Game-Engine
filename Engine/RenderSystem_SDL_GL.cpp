#include "pch.h"
#include "RenderSystem_SDL_GL.h"
#include "WindowSystem_SDL.h"
#include "SystemsLocator.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace Engine::Internal;

RenderSystem_SDL_GL* render_SDL = new RenderSystem_SDL_GL;

RenderSystem_SDL_GL::RenderSystem_SDL_GL() {
    ProvideSystem(this);
    render_SDL = nullptr;
}

RenderSystem_SDL_GL::~RenderSystem_SDL_GL()
{
}

int RenderSystem_SDL_GL::Initialise()
{
    //OpenGL settings
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0, 0, 0, 1);			// Cornflour Blue Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glClearStencil(0);									// Clear stencil buffer
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glDisable(GL_LIGHTING);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 0;
}

int RenderSystem_SDL_GL::Shutdown()
{
    //Destroy render systems
    SDL_DestroyRenderer(renderer_2D_);
    SDL_GL_DeleteContext(openGL_context_);
    return 0;
}

int RenderSystem_SDL_GL::CreateRenderer()
{
    int result = 0;

    window_ = static_cast<WindowSystem_SDL*>(SystemsAPI::Window())->GetSDLWindow();

    if (window_ == nullptr)
    {
        printf("There is no window!\n");
        return -1;
    }

    renderer_2D_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_2D_ == NULL)
    {
        printf("2D renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    else
    {
        //Initialize renderer color
        SDL_SetRenderDrawColor(renderer_2D_, 0x00, 0x00, 0x00, 0x00);
    }

    // Initialize OpenGL
    openGL_context_ = SDL_GL_CreateContext(window_);
    if (openGL_context_ == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    // Define the viewport dimensions 
    glViewport(0, 0, (GLint)SystemsAPI::Window()->GetScreenWidth(), (GLint)SystemsAPI::Window()->GetScreenHeight());
}

void RenderSystem_SDL_GL::Clear()
{
    SDL_SetRenderDrawColor(renderer_2D_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer_2D_);
}

void RenderSystem_SDL_GL::Display()
{
    // Clear the colorbuffer 
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // draw OpenGL 
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 1);
    glPushMatrix();
    //glTranslatef(0, 0, 5);
    gluSphere(gluNewQuadric(), 2, 10, 10);
    glPopMatrix();
    glEnd();


    SDL_GL_SwapWindow(window_);

    //SDL_RenderPresent(renderer_2D_);
}

void RenderSystem_SDL_GL::DrawLine(int x1, int y1, int x2, int y2)
{
    SDL_SetRenderDrawColor(renderer_2D_, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer_2D_, x1, y1, x2, y2);
}
