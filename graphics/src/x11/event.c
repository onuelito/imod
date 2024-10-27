#include <stdio.h>

#include "public/flags.h"

#include "reserved/x11/event.h"

Atom xev_delete_window;

void X11_GetEvent(struct WindowData *data)
{
    struct imodGP_Window *window = data->window;
    struct X11Data *xdata = data->x11;

    XNextEvent(xdata->display, &xdata->event);
    XWindowAttributes newattr;

    int sym = 0;
    struct imodGP_Event event;

    switch (xdata->event.type) {
        case Expose:
            XGetWindowAttributes(xdata->display, xdata->window, &newattr);

            window->pending_width = newattr.width;
            window->pending_height = newattr.height;
            break;

        case ClientMessage:
            if (xdata->event.xclient.data.l[0] == xev_delete_window) {
                window->running = false;
            }
            break;

        case KeyPress:
            event.type = IMODGP_EVENT_KEYPRESS;
            sym = XkbKeycodeToKeysym(xdata->display, xdata->event.xkey.keycode, 0, 0);

            struct _eventKeyPress keypress = {.keycode = sym};
            event.keypress = keypress;

            Event_PushImodGP(window, event);
            break;

        case KeyRelease:
            event.type = IMODGP_EVENT_KEYRELEASE;
            sym = XkbKeycodeToKeysym(xdata->display, xdata->event.xkey.keycode, 0, 0);

            struct _eventKeyRelease keyrelease = {.keycode = sym};
            event.keyrelease = keyrelease;

            Event_PushImodGP(window, event);
            break;

        default:
            break;
    }
}
