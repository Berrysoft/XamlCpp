#ifndef XAML_UI_COCOA_STRINGS_HPP
#define XAML_UI_COCOA_STRINGS_HPP

#include <xaml/strings.hpp>
#include <xaml/ui/objc.hpp>

namespace xaml
{
    inline NSString* to_native(string_view_t str)
    {
        return [NSString stringWithCharacters:(unichar const*)str.data()
                                       length:str.length()];
    }

    inline string_t from_native(NSString* str)
    {
        string_t result(str.length, U('\0'));
        [str getCharacters:(unichar*)result.data()
                     range:NSMakeRange(0, result.length())];
        return result;
    }
} // namespace xaml

#endif // !XAML_UI_COCOA_STRINGS_HPP
