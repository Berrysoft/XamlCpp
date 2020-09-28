#ifndef XAML_UI_CONTROLS_PROGRESS_H
#define XAML_UI_CONTROLS_PROGRESS_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_progress, { 0x505267df, 0x2287, 0x4c64, { 0xab, 0xf2, 0xb2, 0xea, 0xcf, 0xde, 0x8a, 0xe1 } })

#define XAML_PROGRESS_VTBL(type)                                   \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                    \
    XAML_PROP(value, type, XAML_STD int32_t*, XAML_STD int32_t);   \
    XAML_EVENT(value_changed, type, xaml_object, int32_t);         \
    XAML_PROP(minimum, type, XAML_STD int32_t*, XAML_STD int32_t); \
    XAML_PROP(maximum, type, XAML_STD int32_t*, XAML_STD int32_t); \
    XAML_PROP(is_indeterminate, type, bool*, bool);                \
    XAML_EVENT(is_indeterminate_changed, type, xaml_object, bool)

XAML_DECL_INTERFACE_(xaml_progress, xaml_control)
{
    XAML_DECL_VTBL(xaml_progress, XAML_PROGRESS_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_progress_new(xaml_progress**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_progress_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_progress_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_PROGRESS_H
