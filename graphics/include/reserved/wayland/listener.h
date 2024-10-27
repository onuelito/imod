#ifndef RESERVED_WAYLAND_LISTENER
#define RESERVED_WAYLAND_LISTENER

#include <wayland-client.h>


extern const struct wl_registry_listener registry_listener;


extern const struct xdg_wm_base_listener xdg_wm_base_listener;


extern const struct xdg_surface_listener xdg_surface_listener;


extern const struct xdg_toplevel_listener xdg_toplevel_listener;


extern const struct wl_seat_listener wl_seat_listener;


extern const struct wl_keyboard_listener wl_keyboard_listener;


/* Unstable protocols, will be removed when stable alternative is available in Wayland */
extern const struct zxdg_toplevel_decoration_v1_listener zxdg_toplevel_decoration_v1_listener;

#endif
