#ifndef RESERVED_WAYLAND_WINDOW_DATA
#define RESERVED_WAYLAND_WINDOW_DATA

/* TODO: Move this into private */
#include <xkbcommon/xkbcommon.h>

#include <wayland-client.h>
#include <wayland-egl.h>

#include "private/egl.h"
#include "private/window.h"

#include "protocols/xdg-shell-client-protocol.h"
#include "protocols/xdg-decoration-unstable-v1-protocol.h"

struct WaylandData {
    struct wl_display       *display;
    struct wl_surface       *surface;
    struct wl_registry      *registry;
    struct wl_compositor    *compositor;
    struct wl_seat          *seat;
    struct wl_keyboard      *keyboard;
    struct wl_egl_window    *wl_egl_window;

    /* xdg shell interface */
    struct {
        struct xdg_wm_base  *base;
        struct xdg_surface  *surface;
        struct xdg_toplevel *toplevel;
    } xdg;

    /* Keyboard data */
    struct {
        struct xkb_state *state;
        struct xkb_context *context;
        struct xkb_keymap *keymap;
    } xkb;

    /* Unstable protocols. Will be changed when a stable alternative
     * is added to wayland */
    struct zxdg_decoration_manager_v1 *decoration_manager;
    struct zxdg_toplevel_decoration_v1 *toplevel_decoration;
};

struct WindowData {
    /* reference to the window */
    struct imodGP_Window *window;

    struct WaylandData *wayland;
    struct EGLData *egl;
};


/* Create WindowData members and bind to listeners */
void Wayland_CreateData(struct imodGP_Window *window);

void Wayland_DeleteData(struct imodGP_Window *window);

void Wayland_DispatchEvents(struct imodGP_Window *window);

#endif
