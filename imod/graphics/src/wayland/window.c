#include <stdio.h>

#include "public/flags.h"

#include "private/specs.h"
#include "private/wayland/window.h"

#include "reserved/wayland/data.h"

#if defined(OPENGL_ES3_API_SUPPORTED)
#include "private/egl.h"
#endif

/* For when creating a imodgp_Window with wayland as a
 * backend */
void wayland_BindImodGP(struct imodGP_Window *window)
{
    Wayland_CreateData(window);
    printf("Binding wayland window to imodgp Window\n");
}

/* For when destroying an imodGP window that has wayland
 * as its backend */
void Wayland_UbindImodGP(struct imodGP_Window *window)
{
    printf("Unbinding wayland from imodgp window\n");
    Wayland_DeleteData(window);
}


void Wayland_MakeCurrentImodGP(struct imodGP_Window *window)
{
    switch (window->graphicsAPI) {
#if defined(OPENGL_ES3_API_SUPPORTED)
        case IMODGP_API_OPENGL_ES3:
            EGL_MakeCurrentImodGP(window->data);
            break;
#endif
#if defined(VULKAN_API_SUPPORTED)
        case IMODGP_API_VULKAN:
#endif
        default:
            break;
    }
}


void Wayland_SwapBuffersImodGP(struct imodGP_Window *window)
{
    switch (window->graphicsAPI) {
#if defined(OPENGL_ES3_API_SUPPORTED)
        case IMODGP_API_OPENGL_ES3:
            EGL_SwapBuffersImodGP(window->data);
            break;
#endif
#if defined(VULKAN_API_SUPPORTED)
        case IMODGP_API_VULKAN:
#endif
        default:
            break;

    }

    Wayland_DispatchEvents(window);
    if (window->pending_width != window->width || window->pending_height != window->height) {
#if defined(OPENGL_ES3_API_SUPPORTED)
        EGL_ResizeWindowImodGP(window->data);
#endif
#if defined(VULKAN_API_SUPPORTED)

#endif
    }

}
