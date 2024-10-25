#ifndef PRIVATE_IMODGP_WINDOW
#define PRIVATE_IMODGP_WINDOW

#include <stdint.h>
#include <stdbool.h>

struct WindowData;

struct imodGP_Window {
    char *title;
    bool running;
    uint32_t width, height;

    /* For when wanting to resize the window */
    uint32_t pending_width, pending_height;

    int graphicsAPI;
    struct WindowData *data;
};


#endif
