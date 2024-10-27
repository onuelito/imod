#include <stdio.h>

#include "public/flags.h"
#include "public/event.h"

#include "private/window.h"

/* Call this method at the start to avoid polling odd events */
void Event_ClearWindowEvents(struct imodGP_Window *window)
{
    window->event_pop_index = 0;
    window->event_push_index = 0;
    for (int i = 0; i < MAX_WINDOW_EVENTS; i++)
        window->event_queue[i].type = IMODGP_EVENT_NONE;
}

void Event_PushImodGP(struct imodGP_Window *window, struct imodGP_Event event)
{
    int event_index = (window->event_push_index++) % MAX_WINDOW_EVENTS;
    window->event_push_index %= MAX_WINDOW_EVENTS;
    window->event_queue[event_index] = event;
}

struct imodGP_Event Event_PopImodGP(struct imodGP_Window *window)
{
    struct imodGP_Event event = window->event_queue[window->event_pop_index];
    window->event_queue[window->event_pop_index].type = IMODGP_EVENT_NONE;
    window->event_pop_index++;
    return event;
};

/* Call this function when checking for events */
int imodGP_Pending(struct imodGP_Window *window)
{
    if (window->event_pop_index >= window->event_push_index) {
        window->event_push_index = 0;
        window->event_pop_index = 0;
        return 0;
    }
    return 1;
}

void imodGP_NextEvent(struct imodGP_Window *window, struct imodGP_Event *event)
{
    *event = Event_PopImodGP(window);
}
