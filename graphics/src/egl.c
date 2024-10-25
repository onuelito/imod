#include <stdio.h>
#include <stdlib.h>

#include <EGL/egl.h>

#include "private/egl.h"
#include "private/specs.h"

#if defined(XDG_SESSION_TYPE_WAYLAND)
#include "reserved/wayland/data.h"
#elif defined(XDG_SESSION_TYPE_X11)
#include "reserved/x11/data.h"
#endif

#include <GLES3/gl3.h>


struct EGLData {
    EGLDisplay  display;
    EGLConfig   config;
    EGLSurface  surface;
    EGLContext  context;
};

static void
CreateEGLSurface(struct WindowData *data)
{
    struct imodGP_Window *window = data->window;
    struct EGLData *edata = data->egl;

#if defined(XDG_SESSION_TYPE_WAYLAND)
    struct WaylandData *wdata = data->wayland;
    wdata->wl_egl_window = wl_egl_window_create(wdata->surface, window->width, window->height);
    if (wdata->wl_egl_window == EGL_NO_SURFACE) {
        fprintf(stderr, "Failed to create Wayland EGL window\n");
        exit(1);
    }

    edata->surface = eglCreateWindowSurface(edata->display, edata->config, 
            (EGLNativeWindowType)wdata->wl_egl_window, NULL);
#elif defined(XDG_SESSION_TYPE_X11)
    struct X11Data *xdata = data->x11;

    edata->surface = eglCreateWindowSurface(edata->display, edata->config,
            (EGLNativeWindowType)xdata->window, NULL);
#endif

    if (!eglMakeCurrent(edata->display, edata->surface, edata->surface, edata->context)) {
        fprintf(stderr, "Failed to make current context EGL\n");
        exit(1);
    }
}

void EGL_CreateDataImodGP(struct WindowData *data)
{
    printf("Creating EGL data\n");
    struct EGLData *egl_data = malloc(sizeof(struct EGLData));

    EGLint major, minor;
    EGLint num_config, size;

    EGLConfig egl_config;
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE,   8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE,  8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_NONE,
    };

    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
    };



#if defined(XDG_SESSION_TYPE_WAYLAND)
    EGLDisplay display = eglGetDisplay((EGLNativeDisplayType)data->wayland->display);
#elif defined(XDG_SESSION_TYPE_X11)
    EGLDisplay display = eglGetDisplay((EGLNativeDisplayType)data->x11->display);
#endif
    EGLContext  context;

    if (display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to create EGL Display\n");
        exit(1);
    }

    if (!eglInitialize(display, &major, &minor)) {
        fprintf(stderr, "Failed to initialize EGL Display\n");
        exit(1);
    }

    if (!eglChooseConfig(display, config_attribs, &egl_config, 1, &num_config)) {
        fprintf(stderr, "Failed to choose egl config\n");
        exit(1);
    }

    if (num_config != 1) {
        fprintf(stderr, "Got more than 1 EGL Config. Got %d\n", num_config);
        exit(1);
    }

    context = eglCreateContext(display, egl_config, EGL_NO_CONTEXT, context_attribs);
    if (context == EGL_NO_CONTEXT) {
        fprintf(stderr, "Failed to create EGL context\n");
        exit(1);
    }

    /* Wrap-up */
    egl_data->display = display;
    egl_data->context = context;
    egl_data->config = egl_config;
    data->egl = egl_data;

    CreateEGLSurface(data);
}


void EGL_DeleteDataImodGP(struct WindowData *data)
{
    free(data->egl);
}


void EGL_MakeCurrentImodGP(struct WindowData *data)
{
    struct EGLData *edata = data->egl;
    eglMakeCurrent(edata->display, edata->surface, edata->surface, edata->context);
}


void EGL_SwapBuffersImodGP(struct WindowData *data)
{
    struct EGLData *edata = data->egl;
    eglSwapBuffers(edata->display, edata->surface);
}


void EGL_ResizeWindowImodGP(struct WindowData *data)
{
    struct imodGP_Window *window = data->window;
    struct EGLData *edata = data->egl;
#if defined(XDG_SESSION_TYPE_WAYLAND)
    struct WaylandData *wdata = data->wayland;

    EGL_MakeCurrentImodGP(data);
    glViewport(0, 0, window->pending_width, window->pending_height);
    wl_egl_window_resize(wdata->wl_egl_window, window->pending_width, window->pending_height, 0, 0);

#elif defined(XDG_SESSION_TYPE_X11)
    EGL_MakeCurrentImodGP(data);
    glViewport(0, 0, window->pending_width, window->pending_height);
#endif

    window->width = window->pending_width;
    window->height = window->pending_height;
}
