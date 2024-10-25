#ifndef PRIVATE_EGL_DATA
#define PRIVATE_EGL_DATA

#include "private/window.h"

struct EGLData;

/* Create EGL data for a WindowData struct */
void EGL_CreateDataImodGP(struct WindowData *data);

/* Delete EGL data from a WindowData struct */
void EGL_DeleteDataImodGP(struct WindowData *data);

/* Set the current OpenGL ES3 context to the EGL data's */
void EGL_MakeCurrentImodGP(struct WindowData *data);

/* Refresh the render buffer from the EGL data */
void EGL_SwapBuffersImodGP(struct WindowData *data);

void EGL_ResizeWindowImodGP(struct WindowData *data);

#endif
