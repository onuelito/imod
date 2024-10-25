#ifndef PRIVATE_X11_WINDOW
#define PRIVATE_X11_WINDOW

/* Communication header file for x11 toplevel */

#include "public/window.h"

/* injects x11 data into imodGP window */
void X11_BindImodGP(struct imodGP_Window *window);


void X11_UbindImodGP(struct imodGP_Window *window);


void X11_MakeCurrentImodGP(struct imodGP_Window *window);


void X11_SwapBuffersImodGP(struct imodGP_Window *window);

#endif
