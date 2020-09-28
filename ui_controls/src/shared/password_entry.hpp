#ifndef XAML_UI_CONTROLS_SHARED_PASSWORD_ENTRY_HPP
#define XAML_UI_CONTROLS_SHARED_PASSWORD_ENTRY_HPP

#include <shared/entry.hpp>
#include <xaml/ui/controls/password_entry.h>

struct xaml_password_entry_internal : xaml_entry_internal
{
    XAML_EVENT_IMPL(password_char_changed, xaml_object, char);
    XAML_PROP_EVENT_IMPL(password_char, char, char*, char)

    virtual xaml_result XAML_CALL draw_password_char() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_password_entry_impl : xaml_entry_implement<xaml_password_entry_impl, xaml_password_entry_internal, xaml_password_entry>
{
    XAML_EVENT_INTERNAL_IMPL(password_char_changed, xaml_object, char)
    XAML_PROP_INTERNAL_IMPL(password_char, char*, char)
};

#endif // !XAML_UI_CONTROLS_SHARED_PASSWORD_ENTRY_HPP
