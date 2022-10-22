#ifndef X11_ERROR_H
#define X11_ERROR_H

#include "../internal.h"
#include <map>
#include <string>

namespace engine {
    std::map<unsigned int, std::string> x11_base_errors = {
        // TODO: XKB adds the "BadKeyboard" error but im not able to find it
        // Find declaration and add it here
        { 0, "Success" },
        { 1, "BadRequest" },	   	/* bad request code */
        { 2, "BadValue" },	   	/* int parameter out of range */
        { 3, "BadWindow" },	   	/* parameter not a Window */
        { 4, "BadPixmap" },	   	/* parameter not a Pixmap */
        { 5, "BadAtom" },		   	/* parameter not an Atom */
        { 6, "BadCursor" },	   	/* parameter not a Cursor */
        { 7, "BadFont" },		   	/* parameter not a Font */
        { 8, "BadMatch" },   	/* parameter mismatch */
        { 9, "BadDrawable" },  	/* parameter not a Pixmap or Window */
        { 10, "BadAccess" },	  	/* depending on context:
        { 11, "BadAlloc" },	  	/* insufficient resources */
        { 12, "BadColor" },	  	/* no such colormap */
        { 13, "BadGC" },		  	/* parameter not a GC */
        { 14, "BadIDChoice" },	  	/* choice not in range or already used */
        { 15, "BadName" },  	/* font or color name doesn't exist */
        { 16, "BadLength" },	  	/* Request length incorrect */
        { 17, "BadImplementation" },	/* server is defective */
        { 128, "FirstExtensionError" },
        { 255 , "LastExtensionError" }
    };

    std::map<unsigned int, std::string> xkb_open_display_errors = {
        { 0, "XkbOD_Success" },
        { 1, "XkbOD_BadLibraryVersion" },
        { 2, "XkbOD_ConnectionRefused" },
        { 3, "XkbOD_NonXkbServer" },
        { 4, "XkbOD_BadServerVersion" }
    };

    std::string_view get_xkb_open_display_error_string(unsigned int error_flag) {
        auto iter = xkb_open_display_errors.find(error_flag);
        if(iter != xkb_open_display_errors.end()) {
            return iter->second;
        }

        return std::string_view{};
    }

    std::string_view get_x11_base_error_string(unsigned int error_flag) {
        auto iter = x11_base_errors.find(error_flag);
        if(iter != x11_base_errors.end()) {
            return iter->second;
        }

        return std::string_view{};
    }

    bool check_for_x11_error(unsigned int result, unsigned int error_flag) {
        return (result & error_flag) == result;
    }

}

#endif