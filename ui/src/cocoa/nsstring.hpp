#ifndef XAML_UI_COCOA_NSSTRING_HPP
#define XAML_UI_COCOA_NSSTRING_HPP

#include <xaml/string.h>
#include <xaml/ui/cocoa/objc.h>

inline xaml_result XAML_CALL get_NSString(char const* data, NSString** ptr) noexcept
{
    if (data)
    {
        *ptr = [NSString stringWithUTF8String:data];
        return XAML_S_OK;
    }
    *ptr = @"";
    return XAML_S_OK;
}

inline xaml_result XAML_CALL get_NSString(xaml_ptr<xaml_string> const& str, NSString** ptr) noexcept
{
    if (str)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(str->get_data(&data));
        if (data)
        {
            *ptr = [NSString stringWithUTF8String:data];
            return XAML_S_OK;
        }
    }
    *ptr = @"";
    return XAML_S_OK;
}

#endif // !XAML_UI_COCOA_NSSTRING_HPP
