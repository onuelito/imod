#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/mman.h>

#include "public/flags.h"

#include "private/event.h"

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

    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        wdata->seat = wl_registry_bind(registry, id, &wl_seat_interface, version);
        wl_seat_add_listener(wdata->seat, &wl_seat_listener, windata);

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
/* SEAT TOPLEVEL LISTENERS */
/*********************************************************/
static void
wl_seat_capabilities(void *data, 
        struct wl_seat *wl_seat, uint32_t capabilities)
{
    struct WindowData *windata = data;
    struct WaylandData *wdata = windata->wayland;
    bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;

    if (have_keyboard && wdata->keyboard == NULL) {
        wdata->keyboard = wl_seat_get_keyboard(wdata->seat);
        wl_keyboard_add_listener(wdata->keyboard, &wl_keyboard_listener, windata);
    } else if(!have_keyboard && wdata->keyboard != NULL) {
        wl_keyboard_release(wdata->keyboard);
        wdata->keyboard = NULL;
    }
}

static void
wl_seat_name(void *data,
        struct wl_seat *wl_seat, const char *name)
{
    printf("Seat: %s\n", name);
}

const struct wl_seat_listener wl_seat_listener = {
    .capabilities = wl_seat_capabilities,
    .name = wl_seat_name,
};

/*********************************************************/
/* KEYBOARD TOPLEVEL LISTENERS */
/*********************************************************/
static void
wl_keyboard_keymap(void *data,
        struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size)
{
    struct WindowData *windata = data;
    struct WaylandData *wdata = windata->wayland;
    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
        fprintf(stderr, "Wrong keyboard format\n");
        return;
    }

    char *map_shm = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (map_shm == MAP_FAILED) {
        fprintf(stderr, "Failed to map memory\n");
        exit(1);
    }

    struct xkb_keymap *xkb_keymap = xkb_keymap_new_from_string(
            wdata->xkb.context, map_shm,
            XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    munmap(map_shm, size);
    close(fd);

    struct xkb_state *xkb_state = xkb_state_new(xkb_keymap);
    xkb_keymap_unref(wdata->xkb.keymap);
    xkb_state_unref(wdata->xkb.state);

    wdata->xkb.keymap = xkb_keymap;
    wdata->xkb.state = xkb_state;
}

static void
wl_keyboard_enter(void *data,
        struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys)
{

}

static void
wl_keyboard_leave(void *data,
        struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface)
{

}

static void
wl_keyboard_key(void *data,
        struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
    struct WindowData *windata = data;
    struct imodGP_Window *window = windata->window;
    struct WaylandData *wdata = windata->wayland;

    uint32_t keycode = key + 8;
    xkb_keysym_t sym = xkb_state_key_get_one_sym(wdata->xkb.state, keycode);

    bool pressed = (state == WL_KEYBOARD_KEY_STATE_PRESSED);
    if (pressed) {
        struct imodGP_Event event = {.type = IMODGP_EVENT_KEYPRESS};
        struct _eventKeyPress keypress = {.keycode = sym};
        event.keypress = keypress;

        Event_PushImodGP(window, event);
    } else {
        struct imodGP_Event event = {.type = IMODGP_EVENT_KEYRELEASE};
        struct _eventKeyRelease keyrelease = {.keycode = sym};
        event.keyrelease = keyrelease;

        Event_PushImodGP(window, event);
    }
}

static void
wl_keyboard_modifiers(void *data,
        struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched,
        uint32_t mods_locked, uint32_t group)
{

}

static void
wl_keyboard_repeat_info(void *data,
        struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay)
{

}

const struct wl_keyboard_listener wl_keyboard_listener = {
    .keymap = wl_keyboard_keymap,
    .enter = wl_keyboard_enter,
    .leave = wl_keyboard_leave,
    .key = wl_keyboard_key,
    .modifiers = wl_keyboard_modifiers,
    .repeat_info = wl_keyboard_repeat_info,
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
