#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "public/flags.h"
#include "public/window.h"

#include "private/specs.h"
#include "private/window.h"

#include "private/x11/window.h"
#include "private/wayland/window.h"

struct imodGP_Window *imodGP_CreateWindow(struct imodGP_CreateInfo createInfo)
{
    struct imodGP_Window *window = malloc(sizeof(struct imodGP_Window));

    /* Checking for API support */
#if defined(OPENGL_ES3_API_UNSUPPORTED)
    if (createInfo.graphicsAPI == IMODGP_API_OPENGL_ES3) {
        fprintf(stderr, "OpenGLESv3 is currently not supported on your device\n");
        exit(1);
#endif

#if defined(VULKAN_API_UNSUPPORTED)
    if (createInfo.graphicsAPI == IMODGP_API_VULKAN) {
        fprintf(stderr, "Vulkan is currently not supported in imodGP please use OpenGL ES3\n");
        exit(1);
    }
#endif

    /* Assigning create info data */
    window->graphicsAPI = createInfo.graphicsAPI;
    window->pending_height = createInfo.height;
    window->pending_width = createInfo.width;
    window->height = createInfo.height;
    window->width = createInfo.width;
    window->title = createInfo.title;
    window->running = true;

    /* Binding the imodGP window to the supported window
     * system type */
#if defined(XDG_SESSION_TYPE_WAYLAND)
    wayland_BindImodGP(window);
#elif defined(XDG_SESSION_TYPE_X11)
    X11_BindImodGP(window);
#else
    fprintf(stderr, "undefined XDG_SESSION_TYPE. Make sure you are running Wayland or X11\n");
    free(window);
    exit(1);
#endif

    return window;
}


void imodGP_DestroyWindow(struct imodGP_Window *window)
{
#if defined(XDG_SESSION_TYPE_WAYLAND)
    Wayland_UbindImodGP(window);
#elif defined(XDG_SESSION_TYPE_X11)
    X11_UbindImodGP(window);
#endif
    free(window);
}


void imodGP_MakeCurrent(struct imodGP_Window *window)
{
#if defined(XDG_SESSION_TYPE_WAYLAND)
    Wayland_MakeCurrentImodGP(window);
#elif defined(XDG_SESSION_TYPE_X11)
    X11_MakeCurrentImodGP(window);
#endif
}


void imodGP_SwapBuffers(struct imodGP_Window *window)
{
#if defined(XDG_SESSION_TYPE_WAYLAND)
    Wayland_SwapBuffersImodGP(window);
#elif defined(XDG_SESSION_TYPE_X11)
    X11_SwapBuffersImodGP(window);
#endif
}


bool imodGP_IsRunning(struct imodGP_Window *window)
{
    return window->running;
}
