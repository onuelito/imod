#ifndef PRIVATE_IMOD_WAYLAND
#define PRIVATE_IMOD_WAYLAND

/* Module reserved communication header file to manipulate Wayland
 * window instance. */

#include "public/window.h"

/* adds wayland window data to the imod window */
void wayland_BindImodGP(struct imodGP_Window *window);

/* removes wayland window data from imod window */
void Wayland_UbindImodGP(struct imodGP_Window *window);


void Wayland_MakeCurrentImodGP(struct imodGP_Window *window);


void Wayland_SwapBuffersImodGP(struct imodGP_Window *window);

#endif
