#ifndef PUBLIC_GRAPHICS_EVENT
#define PUBLIC_GRAPHICS_EVENT

#include "./window.h"

struct _eventKeyPress {
    int keycode;
    int modifier;
};

struct _eventKeyRelease {
    int keycode;
    int modifier;
};

struct imodGP_Event {
    int type;
    struct imodGP_Window *window;
    struct _eventKeyPress keypress;
    struct _eventKeyRelease keyrelease;
};

int imodGP_Pending(struct imodGP_Window *window);

void imodGP_NextEvent(struct imodGP_Window *window, struct imodGP_Event *event);

#endif
