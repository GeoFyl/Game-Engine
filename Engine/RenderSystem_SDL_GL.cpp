#include "pch.h"
#include "RenderSystem_SDL_GL.h"
#include "SystemsLocator.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <entt/entity/registry.hpp>

using namespace Toffee::Internal;

RenderSystem_SDL_GL* render_SDL = new RenderSystem_SDL_GL;

RenderSystem_SDL_GL::RenderSystem_SDL_GL() {
    ProvideSystem(this);
    render_SDL = nullptr;
}

int RenderSystem_SDL_GL::Initialise()
{
    // We need a reference to the quad mesh for drawing UI 
    UI_mesh_ = reinterpret_cast<Mesh*>(ToffeeAPI::Resources()->GetResource("Quad"));
    return true;
}

int RenderSystem_SDL_GL::Shutdown()
{
    //Destroy render context. Textures are destroyed by resource manager.
    SDL_GL_DeleteContext(openGL_context_);
    return 0;
}

// Turn on/off VSync.
void Toffee::Internal::RenderSystem_SDL_GL::ToggleVsync() {
    vsync_ = !vsync_;
    SDL_GL_SetSwapInterval(vsync_);
}

// Set the viewing transformation matrix.
void Toffee::Internal::RenderSystem_SDL_GL::SetLookAt(float cam_x, float cam_y, float cam_z, float look_at_x, float look_at_y, float look_at_z, float up_x, float up_y, float up_z) {
    gluLookAt(cam_x, cam_y, cam_z, look_at_x, look_at_y, look_at_z, up_x, up_y, up_z);

    look_at_[0] = cam_x;
    look_at_[1] = cam_y;
    look_at_[2] = cam_z;
    look_at_[3] = look_at_x;
    look_at_[4] = look_at_y;
    look_at_[5] = look_at_z;
    look_at_[6] = up_x;
    look_at_[7] = up_y;
    look_at_[8] = up_z;
}

// Create the rendering context
int RenderSystem_SDL_GL::CreateRenderer()
{
    int result = 0;

    window_ = static_cast<SDL_Window*>(ToffeeAPI::Window()->GetWindow());

    if (window_ == nullptr)
    {
        printf("There is no window!\n");
        return -1;
    }

    // Initialize OpenGL
    openGL_context_ = SDL_GL_CreateContext(window_);
    if (openGL_context_ == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_GL_SetSwapInterval(vsync_);

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
    glEnable(GL_BLEND);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Size viewport correctly
    width_ = ToffeeAPI::Window()->GetWidth();
    height_ = ToffeeAPI::Window()->GetHeight();
    ratio_ = width_ / height_;
    Resize(width_, height_);

    return 0;
}

void RenderSystem_SDL_GL::Clear() {
    // Clear opengl buffers 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1.0f);
    glLoadIdentity();
}

using namespace Toffee::Components;
void Toffee::Internal::RenderSystem_SDL_GL::Render() {

    // Set viewport, lighting and wireframe state
    gluLookAt(look_at_[0], look_at_[1], look_at_[2], look_at_[3], look_at_[4], look_at_[5], look_at_[6], look_at_[7], look_at_[8]);
    if (lighting_) glEnable(GL_LIGHTING);
    if (wireframe_) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // --- Lighting  ---

    // Loop through all objects with a light component in the current scene
    int i = GL_LIGHT0;
    auto lights_list = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->view<const Transform, const Light>();
    for (auto light : lights_list) {
        if (i > GL_LIGHT0 + 7) break; // Only up to 8 lights are supported

        // Get transform and light data
        auto& transform = lights_list.get<Transform>(light);
        auto& light_data = lights_list.get<Light>(light);

        // Set ambient and diffuse components 
        // (yes, not the most efficient to update these every frame, but there's little to no noticeable effect on modern hardware)
        glLightfv(i, GL_DIFFUSE, light_data.diffuse);
        glLightfv(i, GL_AMBIENT, light_data.ambient);

        // Set position 
        float pos[4] = { transform.matrix[12],transform.matrix[13],transform.matrix[14],light_data.has_position };
        glLightfv(i, GL_POSITION, pos);

        // If light is a spotlight, set those values, else set to defaults
        if (light_data.spot_cutoff > 0) {
            glLightfv(i, GL_SPOT_DIRECTION, light_data.spot_dir);
            glLightf(i, GL_SPOT_CUTOFF, light_data.spot_cutoff);
            glLightf(i, GL_SPOT_EXPONENT, light_data.spot_exp);
        }
        else {
            glLightf(i, GL_SPOT_CUTOFF, 180);
            glLightf(i, GL_SPOT_EXPONENT, 0);
        }
        // Enable this light
        glEnable(i);
        i++;
    }   

    // --- Meshes ---

    // Loop through all objects with a transform and a renderable component in the current scene
    auto renderables_list = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->view<const Transform, Renderable>();
    for (auto& object : renderables_list) {
        auto& transform = renderables_list.get<Transform>(object);
        auto& renderable = renderables_list.get<Renderable>(object);

        glPushMatrix();

        // Apply the transformation matrix and scaling
        glMultMatrixf(transform.matrix);
        glScalef(transform.scale_x, transform.scale_y, transform.scale_z);
        
        // Prepare to draw vertices, normals and a texture if there is one
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        if (renderable.texture) { // Check if this object has a texture

            // Retrieve handle to the texture and bind it
            GLuint tex = *reinterpret_cast<Texture*>(renderable.texture);
            glBindTexture(GL_TEXTURE_2D, tex);

            // Enable texturing, set filters
            glEnable(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Set texture coordinate array
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, renderable.mesh->texCoords.data());
        }
        // Set vertex and normal arrays
        glVertexPointer(3, GL_FLOAT, 0, renderable.mesh->vertex.data());
        glNormalPointer(GL_FLOAT, 0, renderable.mesh->normals.data());

        // Set material ambient and diffuse components
        glMaterialfv(GL_FRONT, GL_AMBIENT, renderable.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, renderable.diffuse);

        // Draw the object
        glDrawArrays(renderable.triangles ? GL_TRIANGLES : GL_QUADS, 0, renderable.mesh->vertex.size() / 3);

        // Cleanup
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }

    // --- User Interface ---

    // Get the list of UI elements
    std::vector<UIElement>* ui_elements = ToffeeAPI::UI()->GetElements();

    if (!ui_elements->empty()) { // Only need to do this if there actually are any UI elements

        // Swap to 2D rendering
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio_, ratio_, -1.0, 1.0, 5, 100); // Orthographic projection
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Orthographic lookAt (along the z-axis).

        // Enable texturing, disable lighting and depth test (UI always on top of everything else)
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Loop through elements
        for (auto& element : *ui_elements) {
            if (element.texture && element.visible) {
                // Set vertex and texture coordinate arrays
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glVertexPointer(3, GL_FLOAT, 0, UI_mesh_->vertex.data());
                glTexCoordPointer(2, GL_FLOAT, 0, UI_mesh_->texCoords.data());

                // Translate so position on screen is as required
                glPushMatrix();
                glTranslatef(2 * element.pos_x / height_ - ratio_, 1 - (2 * element.pos_y / height_), 0);
                glScalef(element.width * 2 / height_, element.height * 2 / height_, 1);
                glBindTexture(GL_TEXTURE_2D, *reinterpret_cast<Texture*>(element.texture));

                // Point filtering
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                // Draw the UI
                glDrawArrays(GL_QUADS, 0, 4);

                glPopMatrix();

                // Cleanup
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
        }
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        // Swap back to 3D rendering.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov_, ratio_, nearPlane_, farPlane_);
        glMatrixMode(GL_MODELVIEW);
    }
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
