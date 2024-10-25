#ifndef RESERVED_X11_WINDOW_DATA
#define RESERVED_X11_WINDOW_DATA

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "private/egl.h"
#include "private/window.h"

struct X11Data {
    Display    *display;
    Window      window;
    XEvent      event;
};

struct WindowData {
    struct imodGP_Window *window;

    struct X11Data *x11;
    struct EGLData *egl;
};


void X11_CreateData(struct imodGP_Window *window);

void X11_DeleteData(struct imodGP_Window *window);

void X11_DispatchEvents(struct imodGP_Window *window);


#endif
