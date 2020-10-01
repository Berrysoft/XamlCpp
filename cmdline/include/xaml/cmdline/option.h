#ifndef XAML_CMDLINE_OPTION_H
#define XAML_CMDLINE_OPTION_H

#ifdef __cplusplus
    #include <cstdio>
    #include <ostream>
#else
    #include <stdio.h>
#endif // __cplusplus

#include <xaml/event.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_cmdline_option_for_each_args, { 0x8cd6511a, 0x5725, 0x4e59, { 0x92, 0x94, 0x31, 0x07, 0x1a, 0xed, 0xd1, 0x58 } })

#define XAML_CMDLINE_OPTION_FOR_EACH_ARGS_VTBL(type) \
    XAML_VTBL_INHERIT(XAML_EVENT_ARGS_VTBL(type));   \
    XAML_METHOD(get_short_arg, type, char*);         \
    XAML_METHOD(get_long_arg, type, xaml_string**);  \
    XAML_METHOD(get_prop, type, xaml_string**);      \
    XAML_METHOD(get_help_text, type, xaml_string**)

XAML_DECL_INTERFACE_(xaml_cmdline_option_for_each_args, xaml_event_args)
{
    XAML_DECL_VTBL(xaml_cmdline_option_for_each_args, XAML_CMDLINE_OPTION_FOR_EACH_ARGS_VTBL);
};

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_for_each_args_new(char, xaml_string*, xaml_string*, xaml_string*, xaml_cmdline_option_for_each_args**) XAML_NOEXCEPT;

#ifndef xaml_delegate_2__xaml_object__xaml_cmdline_option_for_each_args_defined
    #define xaml_delegate_2__xaml_object__xaml_cmdline_option_for_each_args_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_O(xaml_cmdline_option_for_each_args))
#endif // !xaml_delegate_2__xaml_object__xaml_cmdline_option_for_each_args_defined

XAML_CLASS(xaml_cmdline_option, { 0x240bec4e, 0xefc2, 0x45ed, { 0xa5, 0x01, 0x04, 0xc6, 0x44, 0xf3, 0xf7, 0x0d } })

#define XAML_CMDLINE_OPTION_VTBL(type)                                          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                  \
    XAML_METHOD(find_short_arg, type, char, xaml_string**);                     \
    XAML_METHOD(find_long_arg, type, xaml_string*, xaml_string**);              \
    XAML_METHOD(get_default_property, type, xaml_string**);                     \
    XAML_METHOD(add_arg, type, char, xaml_string*, xaml_string*, xaml_string*); \
    XAML_METHOD(for_each_entry, type, XAML_DELEGATE_2_NAME(xaml_object, xaml_cmdline_option_for_each_args)*)

XAML_DECL_INTERFACE_(xaml_cmdline_option, xaml_object)
{
    XAML_DECL_VTBL(xaml_cmdline_option, XAML_CMDLINE_OPTION_VTBL);

#ifdef __cplusplus
    xaml_result XAML_CALL add_arg(char short_arg, std::string_view long_arg, std::string_view prop, std::string_view help_text) noexcept
    {
        xaml_ptr<xaml_string> long_arg_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(long_arg, &long_arg_str));
        xaml_ptr<xaml_string> prop_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(prop, &prop_str));
        xaml_ptr<xaml_string> help_text_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(help_text, &help_text_str));
        return add_arg(short_arg, long_arg_str, prop_str, help_text_str);
    }
#endif // __cplusplus
};

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_new(xaml_cmdline_option**) XAML_NOEXCEPT;
EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_print(XAML_STD FILE*, xaml_cmdline_option*) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_print(std::basic_ostream<char>&, xaml_cmdline_option*) noexcept;
#endif // __cplusplus

#endif // !XAML_CMDLINE_OPTION_H
