#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "reserved/wayland/data.h"
#include "reserved/wayland/listener.h"

/*********************************************************/
/* GLOBALS LISTENERS */
/*********************************************************/
static void 
registry_handler(void *data,
        struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version)
{
    struct WindowData *windata = data;
    struct WaylandData *wdata = windata->wayland;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        wdata->compositor = wl_registry_bind(registry, id, &wl_compositor_interface, version);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        wdata->xdg.base = wl_registry_bind(registry, id, &xdg_wm_base_interface, version);
        xdg_wm_base_add_listener(wdata->xdg.base, &xdg_wm_base_listener, NULL);

    } else if (strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
        wdata->decoration_manager = 
            wl_registry_bind(registry, id, &zxdg_decoration_manager_v1_interface, version);
    }

}

static void 
registry_remover(void *data,
        struct wl_registry *registry, uint32_t id)
{
    /* left empty on purpose */
}


const struct wl_registry_listener registry_listener = {
    .global = registry_handler,
    .global_remove = registry_remover,
};




/*********************************************************/
/* XDG BASE LISTENERS */
/*********************************************************/
static void
xdg_wm_base_ping(void *data,
        struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};


/*********************************************************/
/* XDG SURFACE LISTENERS */
/*********************************************************/
static void
xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    xdg_surface_ack_configure(xdg_surface, serial);
}


const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

/*********************************************************/
/* XDG TOPLEVEL LISTENERS */
/*********************************************************/
static void
xdg_toplevel_configure(void *data,
        struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states)
{
    struct WindowData *windata = data;
    struct imodGP_Window *window = windata->window;


    window->pending_width = width;
    window->pending_height = height;
}


static void
xdg_toplevel_close(void *data,
        struct xdg_toplevel *xdg_toplevel)
{
    struct WindowData *windata = data;
    struct imodGP_Window *window = windata->window;

    window->running = false;
}

const struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
};

/*********************************************************/
/* IMPORTANT */
/* BELOW ARE A LIST OF UNSTABLE PROTCOLS THAT WILL BE
 * REPLACED WHEN A STABLE ALTERNATIVE WILL BE AVAILABLE
 * IN WAYLAND */
/*********************************************************/

/*********************************************************/
/* TOPLEVEL DECORATION LISTENERS */
/*********************************************************/

static void
zxdg_toplevel_decoration_configure(void *data,
        struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1, uint32_t mode)
{

}





const struct zxdg_toplevel_decoration_v1_listener zxdg_toplevel_decoration_v1_listener = {
    .configure = zxdg_toplevel_decoration_configure,
};
