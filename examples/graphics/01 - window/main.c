#include "imod/graphics.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>


/* Sample program to create a window with OpenGL ES3 */

int main()
{
    struct imodGP_CreateInfo createInfo = {
        .width  = 320,
        .height = 240,
        .title  = "Sample Window",
    };

    struct imodGP_Window *window = imodGP_CreateWindow(createInfo);
    imodGP_MakeCurrent(window);

    while (imodGP_IsRunning(window)) {
        
        /* Rendering */
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 1.0);
        }
        imodGP_SwapBuffers(window);

    }

    /* cleanup */
    imodGP_DestroyWindow(window);

    return 0;
}
