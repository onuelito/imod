#ifndef PUBLIC_IMODGP_WINDOW
#define PUBLIC_IMODGP_WINDOW

#include <stdint.h>
#include <stdbool.h>

struct imodGP_CreateInfo {
    char *title;
    uint16_t width, height;
    int graphicsAPI;
};

struct imodGP_Window;

/* Creates a window with configuration of the developper choice. */
struct imodGP_Window *imodGP_CreateWindow(struct imodGP_CreateInfo);

/* Deletes the window and free all of its memory */
void imodGP_DestroyWindow(struct imodGP_Window *window);

/* Sets buffer to the current window's. OpenGL ES3 has a single window
 * so this function can be called once */
void imodGP_MakeCurrent(struct imodGP_Window *window);

/* Update graphics API context */
void imodGP_SwapBuffers(struct imodGP_Window *window);


bool imodGP_IsRunning(struct imodGP_Window *window);

#endif
