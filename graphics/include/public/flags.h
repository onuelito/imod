#ifndef PUBLIC_IMODGP_FLAGS
#define PUBLIC_IMODGP_FLAGS

#define IMODGP_WINDOW_X11       0
#define IMODGP_WINDOW_WAYLAND   1


#define IMODGP_API_OPENGL_ES3       0
#define IMODGP_API_VULKAN           1

/* Lists of event types from toplevel that will be handled by
 * developpers after using IMODGP_PollEvents(...) */
#define IMODGP_EVENT_NONE           0
#define IMODGP_EVENT_KEYPRESS       1
#define IMODGP_EVENT_KEYRELEASE     2


#endif
