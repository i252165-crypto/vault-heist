#include "Event.h"

Event makeEvent(EventType type, int a, int b, int c, int d, int e,
                const char *note)
{
    Event ev;
    ev.type = type;
    ev.a = a;
    ev.b = b;
    ev.c = c;
    ev.d = d;
    ev.e = e;
    ev.note = note;
    return ev;
}
