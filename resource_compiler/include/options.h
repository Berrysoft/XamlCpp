#ifndef XAML_RC_OPTIONS_H
#define XAML_RC_OPTIONS_H

#include <xaml/cmdline/options_base.h>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>

#ifndef xaml_enumerator_1__xaml_string_defined
    #define xaml_enumerator_1__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_enumerator_1__xaml_string_defined

#ifndef xaml_vector_view_1__xaml_string_defined
    #define xaml_vector_view_1__xaml_string_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_vector_view_1__xaml_string_defined

XAML_CLASS(xaml_rc_options, { 0x50dd3e88, 0x5d41, 0x4418, { 0x90, 0x96, 0x7f, 0xd8, 0xb7, 0x76, 0xf9, 0xca } })

#define XAML_RC_OPTIONS_VTBL(type)                                         \
    XAML_VTBL_INHERIT(XAML_CMDLINE_OPTIONS_BASE_VTBL(type));               \
    XAML_METHOD(get_inputs, type, XAML_VECTOR_VIEW_1_NAME(xaml_string)**); \
    XAML_CPROP(input, type, xaml_string*, xaml_string*);                   \
    XAML_PROP(output, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_rc_options, xaml_cmdline_options_base)
{
    XAML_DECL_VTBL(xaml_rc_options, XAML_RC_OPTIONS_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_rc_options_new(xaml_rc_options**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_rc_options_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_RC_OPTIONS_H
