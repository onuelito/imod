#ifndef RESERVED_X11_WINDOW_EVENT
#define RESERVED_X11_WINDOW_EVENT

#include "reserved/x11/data.h"

extern Atom xev_delete_window;

void X11_GetEvent(struct WindowData *window);

#endif
