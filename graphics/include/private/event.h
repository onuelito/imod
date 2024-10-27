#ifndef PRIVATE_KEYBOARD_EVENT
#define PRIVATE_KEYBOARD_EVENT

#include "public/event.h"
#include "public/window.h"

#define MAX_WINDOW_EVENTS       64

void Event_ClearWindowEvents(struct imodGP_Window *window);

void Event_PushImodGP(struct imodGP_Window *window, struct imodGP_Event event);

#endif
