#include <stdio.h>

#include "reserved/x11/event.h"



Atom xev_delete_window;

void X11_GetEvent(struct WindowData *data)
{
    struct imodGP_Window *window = data->window;
    struct X11Data *xdata = data->x11;

    XNextEvent(xdata->display, &xdata->event);
    XWindowAttributes newattr;

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

        default:
            break;
    }
}
