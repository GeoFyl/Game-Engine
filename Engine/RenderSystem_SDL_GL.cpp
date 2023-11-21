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

    renderer_2D_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
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

    int width = SystemsAPI::Window()->GetScreenWidth();
    int height = SystemsAPI::Window()->GetScreenHeight();
    target_2D_ = SDL_CreateTexture(renderer_2D_, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderer_2D_, target_2D_);
    target_2D_quad_ = std::make_unique<Quad>(width, height);

    ratio_ = (float)width / (float)height;

    // Initialize OpenGL
    openGL_context_ = SDL_GL_CreateContext(window_);
    if (openGL_context_ == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }

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
    glEnable(GL_TEXTURE_2D);

    Resize();

    return 0;
}

void RenderSystem_SDL_GL::Clear()
{
    SDL_SetRenderDrawColor(renderer_2D_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer_2D_);

    // Clear opengl buffers 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0, 0, 1.0f);
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);
}

void Engine::Internal::RenderSystem_SDL_GL::Render() {
    gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0, 0, -5);
    gluSphere(gluNewQuadric(), 2, 10, 10);


    DrawLine(100, 200, 400, 700);
    DrawLine(100, 200, 1200, 50);
    DrawLine(1200, 50, 400, 700);
    //Render2DTarget();
}

void Engine::Internal::RenderSystem_SDL_GL::Render2DTarget() {
    //SDL_RenderPresent(renderer_2D_);

    // Swap to 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Orthographic lookAt (along the z-axis).
    gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    SDL_GL_BindTexture(target_2D_, NULL, NULL);
    target_2D_quad_.get()->render();

    // Swap back to 3D rendering.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov_, ratio_, nearPlane_, farPlane_);
    glMatrixMode(GL_MODELVIEW);
}

void RenderSystem_SDL_GL::Display()
{

    // draw OpenGL 
   // glColor3f(0, 0, 0);
   // 
    //SDL_GL_SwapWindow(window_);

    SDL_RenderCopy(renderer_2D_, target_2D_, NULL, NULL);
    SDL_RenderPresent(renderer_2D_);

   // SDL_RenderPresent(renderer_2D_);  https://gamedev.stackexchange.com/questions/157604/how-to-get-access-to-framebuffer-as-a-uint32-t-in-sdl2
    //make as texture then display same way text was done in cmp203
}


void Engine::Internal::RenderSystem_SDL_GL::Resize() {
    // Define the viewport dimensions 
    GLint width = (GLint)SystemsAPI::Window()->GetScreenWidth();
    GLint height = (GLint)SystemsAPI::Window()->GetScreenHeight();
    // Prevent a divide by zero
    if (height == 0)
        height = 1;

    /*float ratio = (float)width / (float)height;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;*/

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    // Set the correct perspective.
    gluPerspective(fov_, ratio_, nearPlane_, farPlane_);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void RenderSystem_SDL_GL::DrawLine(int x1, int y1, int x2, int y2)
{
    SDL_SetRenderDrawColor(renderer_2D_, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer_2D_, x1, y1, x2, y2);
}
