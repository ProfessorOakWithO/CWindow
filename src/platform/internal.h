#ifndef INTERNAL_H
#define INTERNAL_H

#if defined(__linux__)
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/Xutil.h>
    #include <X11/Xos.h>
    #include <X11/XKBlib.h>
    #include <X11/extensions/XKBstr.h>
    #include <X11/extensions/XKBproto.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#endif

#endif