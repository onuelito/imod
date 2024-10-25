#include <stdio.h>

#include "public/flags.h"

#include "private/specs.h"
#include "private/x11/window.h"

#include "reserved/x11/data.h"

#if defined(OPENGL_ES3_API_SUPPORTED)
#include "private/egl.h"
#endif

void X11_BindImodGP(struct imodGP_Window *window)
{
   printf("Creating X11 window\n"); 
   X11_CreateData(window);
}


void X11_UbindImodGP(struct imodGP_Window *window)
{
    printf("Unbinding X11 from imodgp window\n");
    X11_DeleteData(window);
}

void X11_MakeCurrentImodGP(struct imodGP_Window *window)
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

void X11_SwapBuffersImodGP(struct imodGP_Window *window)
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

    X11_DispatchEvents(window);

    if (window->width != window->pending_width || window->height != window->pending_height) {
        switch (window->graphicsAPI) {
#if defined (OPENGL_ES3_API_SUPPORTED)
            case IMODGP_API_OPENGL_ES3:
                EGL_ResizeWindowImodGP(window->data);
                break;
#endif
#if defined(VULKAN_API_SUPPORTED)
            case IMODGP_API_VULKAN:
#endif
            default:
                break;

        }
    }
}
