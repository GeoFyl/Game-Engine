#include "pch.h"
#include "RenderSystem_SDL_GL.h"
#include "SystemsLocator.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <entt/entity/registry.hpp>

using namespace Engine::Internal;

RenderSystem_SDL_GL* render_SDL = new RenderSystem_SDL_GL;

RenderSystem_SDL_GL::RenderSystem_SDL_GL() {
    ProvideSystem(this);
    render_SDL = nullptr;
}

RenderSystem_SDL_GL::~RenderSystem_SDL_GL()
{
}

void Engine::Internal::RenderSystem_SDL_GL::ToggleVsync() {
    vsync_ = !vsync_;
    SDL_GL_SetSwapInterval(!vsync_);
}

int RenderSystem_SDL_GL::Initialise()
{
    UI_mesh_ = reinterpret_cast<Mesh*>(SystemsAPI::Resources()->GetResource("Quad"));
    return true;
}

int RenderSystem_SDL_GL::Shutdown()
{
    //Destroy render systems
    SDL_GL_DeleteContext(openGL_context_);
    return 0;
}

int RenderSystem_SDL_GL::CreateRenderer()
{
    int result = 0;

    window_ = static_cast<SDL_Window*>(SystemsAPI::Window()->GetWindow());

    if (window_ == nullptr)
    {
        printf("There is no window!\n");
        return -1;
    }

    width_ = SystemsAPI::Window()->GetWidth();
    height_ = SystemsAPI::Window()->GetHeight();
    ratio_ = width_ / height_;

    // Initialize OpenGL
    openGL_context_ = SDL_GL_CreateContext(window_);
    if (openGL_context_ == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_GL_SetSwapInterval(1);

    //OpenGL settings
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0, 0, 0, 1);			// Cornflour Blue Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glClearStencil(0);									// Clear stencil buffer
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glDisable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   // glEnable(GL_TEXTURE_2D);

    Resize(width_, height_);

    return 0;
}

void RenderSystem_SDL_GL::Clear() {
    // Clear opengl buffers 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1.0f);
    glLoadIdentity();
}

using namespace Engine::Components;
void Engine::Internal::RenderSystem_SDL_GL::Render() {
    gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    if (lighting_) glEnable(GL_LIGHTING);
    if (wireframe_) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int i = GL_LIGHT0;
    auto lights_list = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->view<const Transform, const Light>();
    for (auto light : lights_list) {
        if (i > GL_LIGHT0 + 7) break;
        auto& transform = lights_list.get<Transform>(light);
        auto& light_data = lights_list.get<Light>(light);
        glLightfv(i, GL_DIFFUSE, light_data.diffuse);
        glLightfv(i, GL_AMBIENT, light_data.ambient);
        float pos[4] = { transform.matrix[12],transform.matrix[13],transform.matrix[14],light_data.has_position };
        glLightfv(i, GL_POSITION, pos);
        if (light_data.spot_cutoff > 0) {
            glLightfv(i, GL_SPOT_DIRECTION, light_data.spot_dir);
            glLightf(i, GL_SPOT_CUTOFF, light_data.spot_cutoff);
            glLightf(i, GL_SPOT_EXPONENT, light_data.spot_exp);
        }
        glEnable(i);
        i++;
    }   

    auto renderables_list = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->view<const Transform, Renderable>();
    for (auto& object : renderables_list) {
        auto& transform = renderables_list.get<Transform>(object);
        auto& renderable = renderables_list.get<Renderable>(object);
        glPushMatrix();

        glMultMatrixf(transform.matrix);
        glScalef(transform.scale_x, transform.scale_y, transform.scale_z);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        if (renderable.texture) {

            GLuint tex = *reinterpret_cast<Texture*>(renderable.texture);

            glBindTexture(GL_TEXTURE_2D, tex);

            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexCoordPointer(2, GL_FLOAT, 0, renderable.mesh->texCoords.data());
        }

        glVertexPointer(3, GL_FLOAT, 0, renderable.mesh->vertex.data());
        glNormalPointer(GL_FLOAT, 0, renderable.mesh->normals.data());

        glMaterialfv(GL_FRONT, GL_AMBIENT, renderable.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, renderable.diffuse);

        glDrawArrays(renderable.triangles ? GL_TRIANGLES : GL_QUADS, 0, renderable.mesh->vertex.size() / 3);

       

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
      //  glDisable(GL_BLEND);
       // glEnable(GL_DEPTH_TEST);

        glPopMatrix();
    }

    // Render UI

    std::vector<UIElement>* ui_elements = SystemsAPI::UI()->GetElements();

    // Swap to 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio_, ratio_, -1.0, 1.0, 5, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Orthographic lookAt (along the z-axis).
    gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);




    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);


    for (auto& element : *ui_elements) {
       // uint8_t ptr[50000];

        if (element.texture && element.visible) {
            // Set position.
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, UI_mesh_->vertex.data());
            glTexCoordPointer(2, GL_FLOAT, 0, UI_mesh_->texCoords.data());

            glPushMatrix();
            glTranslatef(2 * element.pos_x / height_ - ratio_, 1 - (2 * element.pos_y / height_), 0);
            glScalef(element.width * 2 / height_, element.height * 2 / height_, 1);
            glBindTexture(GL_TEXTURE_2D, *reinterpret_cast<Texture*>(element.texture));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glDrawArrays(GL_QUADS, 0, 4);

            glPopMatrix();

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    //glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    // Swap back to 3D rendering.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov_, ratio_, nearPlane_, farPlane_);
    glMatrixMode(GL_MODELVIEW);


}


void RenderSystem_SDL_GL::Display() {
    // draw OpenGL 
    SDL_GL_SwapWindow(window_);
}


void RenderSystem_SDL_GL::Resize(float w, float h) {

    // Prevent a divide by zero
    if (h == 0) h = 1;
    width_ = w;
    height_ = h;        

    ratio_ = w / h;
    fov_ = 45.0f;
    nearPlane_ = 0.1f;
    farPlane_ = 100.0f;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width_, height_);

    // Set the correct perspective.
    gluPerspective(fov_, ratio_, nearPlane_, farPlane_);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}
