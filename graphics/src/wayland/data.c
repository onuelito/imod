/* Data manipulation source file */

#include <stdio.h>
#include <stdlib.h>

#include "public/flags.h"

#include "private/window.h"
#include "private/specs.h"

#include "reserved/wayland/data.h"
#include "reserved/wayland/listener.h"


#if defined(OPENGL_ES3_SUPPORTED)
#include "private/egl.h"
#endif

static struct WaylandData
*AllocateWaylandData()
{
    struct WaylandData *data = malloc(sizeof(struct WaylandData));
    return data;
}


static void
FreeWaylandData(struct WaylandData *data)
{
    /* Unstable protocols to remove when stable available */
    zxdg_toplevel_decoration_v1_destroy(data->toplevel_decoration);
    zxdg_decoration_manager_v1_destroy(data->decoration_manager);

    /* Stable Protocols */
    xdg_toplevel_destroy(data->xdg.toplevel);
    xdg_surface_destroy(data->xdg.surface);
    wl_surface_destroy(data->surface);
    wl_compositor_destroy(data->compositor);
    wl_display_disconnect(data->display);
}

void Wayland_CreateData(struct imodGP_Window *window)
{
    printf("Creating wayland data\n");

    struct WindowData *wdata = malloc(sizeof(struct WindowData));
    wdata->window = window;
    /* Creating the Wayland toplevel */
    {
        struct WaylandData *data = AllocateWaylandData();
        wdata->wayland = data;
        if ((data->display = wl_display_connect(NULL)) == NULL) {
            fprintf(stderr, "Failed to connect to Wayland display\n");
            exit(1);
        }

        data->registry = wl_display_get_registry(data->display);
        wl_registry_add_listener(data->registry, &registry_listener, wdata);

        wl_display_dispatch(data->display);
        wl_display_roundtrip(data->display);

        if (data->compositor == NULL || data->xdg.base == NULL) {
            fprintf(stderr, "Failed to create XDG shell\n");
            exit(1);
        }
        data->surface = wl_compositor_create_surface(data->compositor);

        /* Creating XDG shell and its members */
        data->xdg.surface = xdg_wm_base_get_xdg_surface(data->xdg.base, data->surface);
        xdg_surface_add_listener(data->xdg.surface, &xdg_surface_listener, NULL);

        data->xdg.toplevel = xdg_surface_get_toplevel(data->xdg.surface);
        /* TODO: WindowData instead of NULL */
        xdg_toplevel_add_listener(data->xdg.toplevel, &xdg_toplevel_listener, wdata);
        xdg_toplevel_set_title(data->xdg.toplevel, window->title);


        data->toplevel_decoration = 
            zxdg_decoration_manager_v1_get_toplevel_decoration(data->decoration_manager, 
                    data->xdg.toplevel);
        zxdg_toplevel_decoration_v1_add_listener(data->toplevel_decoration, 
                &zxdg_toplevel_decoration_v1_listener, NULL);


        wl_surface_commit(data->surface);
        wl_display_roundtrip(data->display);

    }


    /* Creating to API data OpenGL ES3 / Vulkan */
    switch (window->graphicsAPI) {
#if defined(OPENGL_ES3_API_SUPPORTED)
        case IMODGP_API_OPENGL_ES3:
            EGL_CreateDataImodGP(wdata);
#endif
        default:
            break;
    }

    window->data = wdata;
}


void Wayland_DeleteData(struct imodGP_Window *window)
{
    switch (window->graphicsAPI) {
#if defined (OPENGL_ES3_API_SUPPORTED)
        case IMODGP_API_OPENGL_ES3:         
            EGL_DeleteDataImodGP(window->data);
#endif
        default:
            break;
    }

    FreeWaylandData(window->data->wayland);
    free(window->data);
}

void Wayland_DispatchEvents(struct imodGP_Window *window)
{
    wl_display_dispatch_pending(window->data->wayland->display);
}
