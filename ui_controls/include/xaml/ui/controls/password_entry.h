#ifndef XAML_UI_CONTROLS_PASSWORD_ENTRY_H
#define XAML_UI_CONTROLS_PASSWORD_ENTRY_H

#include <xaml/ui/controls/entry.h>

#define XAML_PASSWORD_ENTRY_VTBL(type)                         \
    XAML_VTBL_INHERIT(XAML_ENTRY_VTBL(type));                  \
    XAML_PROP(password_char, type, xaml_char_t*, xaml_char_t); \
    XAML_EVENT(password_char_changed, type)

XAML_DECL_INTERFACE_(xaml_password_entry, xaml_entry)
{
    XAML_DECL_VTBL(xaml_password_entry, XAML_PASSWORD_ENTRY_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_password_entry_new(xaml_password_entry**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_password_entry_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_password_entry_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_PASSWORD_ENTRY_H
