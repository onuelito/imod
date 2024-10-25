#include <stdio.h>
#include <stdlib.h>

#include "public/flags.h"

#include "private/specs.h"

#include "reserved/x11/data.h"
#include "reserved/x11/event.h"

#if defined(OPENGL_ES3_API_SUPPORTED)
#include "private/egl.h"
#endif

void X11_CreateData(struct imodGP_Window *window)
{
    struct WindowData *wdata = malloc(sizeof(struct WindowData));
    wdata->window = window;
    /* Creating X11 window */
    {
        struct X11Data *data = malloc(sizeof(struct X11Data));
        wdata->x11 = data;
        if ((data->display = XOpenDisplay(NULL)) == NULL) {
            fprintf(stderr, "Failed to open X display\n");
            exit(1);
        }

        int xscreen = DefaultScreen(data->display);

        XSetWindowAttributes xswa;

        xswa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;

        data->window = XCreateWindow( data->display,
                                      RootWindow(data->display, xscreen),
                                      0,
                                      0,
                                      window->width,
                                      window->height,
                                      0,
                                      CopyFromParent,
                                      InputOutput,
                                      CopyFromParent,
                                      CWEventMask,
                                      &xswa);

        XSetWindowAttributes xattr;
        xattr.override_redirect = False;
        XChangeWindowAttributes(data->display, data->window, CWOverrideRedirect, &xattr);

        xev_delete_window = XInternAtom(data->display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(data->display, data->window, &xev_delete_window, 1);

        XMapWindow(data->display, data->window);
        XStoreName(data->display, data->window, window->title);
    }

    window->data = wdata;

    /* Creating Graphics API data */
    switch (window->graphicsAPI) {
#if defined (OPENGL_ES3_API_SUPPORTED)
        case IMODGP_API_OPENGL_ES3:
            EGL_CreateDataImodGP(wdata);
            break;
#endif
#if defined(VULKAN_API_SUPPORTED)
        case IMODGP_API_VULKAN:
#endif
        default:
            break;

    }

}


void X11_DeleteData(struct imodGP_Window *window)
{
    struct WindowData *wdata = window->data;
    struct X11Data *xdata = wdata->x11;

#if defined(OPENGL_ES3_API_SUPPORTED)
    EGL_DeleteDataImodGP(wdata);
#endif

    XUnmapWindow(xdata->display, xdata->window);
    XDestroyWindow(xdata->display, xdata->window);
    XCloseDisplay(xdata->display);
}

void X11_DispatchEvents(struct imodGP_Window *window)
{
    while(XPending(window->data->x11->display))
        X11_GetEvent(window->data);

}
