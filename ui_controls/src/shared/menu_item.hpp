#ifndef XAML_UI_CONTROLS_SHARED_MENU_ITEM_HPP
#define XAML_UI_CONTROLS_SHARED_MENU_ITEM_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/menu_item.h>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <wil/resource.h>
#include <xaml/ui/win/controls/menu_item.h>
#include <xaml/ui/win/menu_bar.h>
#elif defined(XAML_UI_COCOA)
#include <xaml/ui/cocoa/controls/menu_item.h>
#elif defined(XAML_UI_GTK3)
#include <xaml/ui/gtk3/controls/menu_item.h>
#endif // XAML_UI_WINDOWS

struct xaml_menu_item_internal : xaml_control_internal
{
    XAML_PROP_PTR_IMPL(text, xaml_string)
    XAML_EVENT_IMPL(click)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_WINDOWS
    XAML_PROP_IMPL(menu_id, UINT, UINT*, UINT)
    XAML_PROP_IMPL(menu_parent, HMENU, HMENU*, HMENU)

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept override;

    virtual xaml_result XAML_CALL draw_impl(UINT flags) noexcept;

    virtual xaml_result XAML_CALL draw_append(HMENU pmenu, UINT flags) noexcept;
#elif defined(XAML_UI_COCOA)
    virtual xaml_result XAML_CALL draw_append(OBJC_OBJECT(NSMenu)) noexcept;

    XAML_PROP_IMPL(menu, OBJC_OBJECT(NSMenuItem), OBJC_OBJECT(NSMenuItem) *, OBJC_OBJECT(NSMenuItem))

    void on_action() noexcept;
#elif defined(XAML_UI_GTK3)
    static void on_activate(GtkWidget*, xaml_menu_item_internal*) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

#ifdef XAML_UI_WINDOWS
template <typename T, typename D, typename Base>
struct xaml_win32_menu_item_implement : xaml_inner_implement<T, D, Base>
{
    xaml_result XAML_CALL get_id(UINT* pvalue) noexcept override { return this->m_outer->get_menu_id(pvalue); }
    xaml_result XAML_CALL set_id(UINT value) noexcept override { return this->m_outer->set_menu_id(value); }
    xaml_result XAML_CALL get_parent_handle(HMENU* pvalue) noexcept override { return this->m_outer->get_menu_parent(pvalue); }
    xaml_result XAML_CALL set_parent_handle(HMENU value) noexcept override { return this->m_outer->set_menu_parent(value); }
};
#elif defined(XAML_UI_COCOA)
template <typename T, typename D, typename Base>
struct xaml_cocoa_menu_item_implement : xaml_inner_implement<T, D, Base>
{
    xaml_result XAML_CALL get_menu(OBJC_OBJECT(NSMenuItem) * pvalue) noexcept override { return this->m_outer->get_menu(pvalue); }
    xaml_result XAML_CALL set_menu(OBJC_OBJECT(NSMenuItem) value) noexcept override { return this->m_outer->set_menu(value); }
};
#endif // XAML_UI_WINDOWS

template <typename T, typename Internal, typename... Base>
struct xaml_menu_item_implement : xaml_control_implement<T, Internal, Base..., xaml_menu_item>
{
    XAML_PROP_PTR_INTERNAL_IMPL(text, xaml_string)
    XAML_EVENT_INTERNAL_IMPL(click)

#ifdef XAML_UI_WINDOWS
    struct xaml_win32_menu_item_impl : xaml_win32_menu_item_implement<xaml_win32_menu_item_impl, T, xaml_win32_menu_item>
    {
    } m_native_menu_item;

    using native_menu_item_type = xaml_win32_menu_item;

    XAML_PROP_INTERNAL_IMPL(menu_id, UINT*, UINT)
    XAML_PROP_INTERNAL_IMPL(menu_parent, HMENU*, HMENU)
#elif defined(XAML_UI_COCOA)
    struct xaml_cocoa_menu_item_impl : xaml_cocoa_menu_item_implement<xaml_cocoa_menu_item_impl, T, xaml_cocoa_menu_item>
    {
    } m_native_menu_item;

    using native_menu_item_type = xaml_cocoa_menu_item;

    XAML_PROP_INTERNAL_IMPL(menu, OBJC_OBJECT(NSMenuItem) *, OBJC_OBJECT(NSMenuItem))
#endif // XAML_UI_WINDOWS

#ifndef XAML_UI_GTK3
    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<native_menu_item_type>)
        {
            this->add_ref();
            *ptr = static_cast<native_menu_item_type*>(&m_native_menu_item);
            return XAML_S_OK;
        }
        else
        {
            return xaml_control_implement<T, Internal, Base..., xaml_menu_item>::query(type, ptr);
        }
    }

    xaml_menu_item_implement() noexcept
    {
        m_native_menu_item.m_outer = static_cast<T*>(this);
    }
#endif // !XAML_UI_GTK3
};

struct xaml_menu_item_impl : xaml_menu_item_implement<xaml_menu_item_impl, xaml_menu_item_internal>
{
};

struct xaml_popup_menu_item_internal : xaml_menu_item_internal
{
protected:
    xaml_ptr<xaml_vector> m_submenu;

public:
    xaml_result XAML_CALL get_submenu(xaml_vector_view** ptr) noexcept
    {
        return m_submenu->query(ptr);
    }

    xaml_result XAML_CALL add_submenu(xaml_menu_item*) noexcept;
    xaml_result XAML_CALL remove_submenu(xaml_menu_item*) noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept;
    virtual xaml_result XAML_CALL draw_submenu() noexcept;

#ifdef XAML_UI_WINDOWS
    wil::unique_hmenu m_menu{ nullptr };

    xaml_result XAML_CALL get_menu(HMENU* pvalue) noexcept
    {
        *pvalue = m_menu.get();
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_menu(HMENU value) noexcept
    {
        m_menu.reset(value);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept override;
    xaml_result XAML_CALL draw_append(HMENU pmenu, UINT flags) noexcept override;
#elif defined(XAML_UI_COCOA)
    XAML_PROP_IMPL(menu_handle, OBJC_OBJECT(NSMenu), OBJC_OBJECT(NSMenu) *, OBJC_OBJECT(NSMenu))

    xaml_result XAML_CALL draw_append(OBJC_OBJECT(NSMenu)) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_popup_menu_item_impl : xaml_menu_item_implement<xaml_popup_menu_item_impl, xaml_popup_menu_item_internal, xaml_popup_menu_item>
{
    XAML_PROP_PTR_INTERNAL_IMPL_BASE(submenu, xaml_vector_view)

    XAML_CPROP_INTERNAL_IMPL(submenu, xaml_menu_item*, xaml_menu_item*)

#ifdef XAML_UI_WINDOWS
    struct xaml_win32_popup_menu_item_impl : xaml_win32_menu_item_implement<xaml_win32_popup_menu_item_impl, xaml_popup_menu_item_impl, xaml_win32_popup_menu_item>
    {
        xaml_result XAML_CALL get_handle(HMENU* pvalue) noexcept override { return m_outer->get_menu(pvalue); }
        xaml_result XAML_CALL set_handle(HMENU value) noexcept override { return m_outer->set_menu(value); }
    } m_native_popup_menu_item;

    using native_popup_menu_item_type = xaml_win32_popup_menu_item;

    XAML_PROP_INTERNAL_IMPL(menu, HMENU*, HMENU)
#elif defined(XAML_UI_COCOA)
    struct xaml_cocoa_popup_menu_item_impl : xaml_cocoa_menu_item_implement<xaml_cocoa_popup_menu_item_impl, xaml_popup_menu_item_impl, xaml_cocoa_popup_menu_item>
    {
        xaml_result XAML_CALL get_handle(OBJC_OBJECT(NSMenu) * pvalue) noexcept override { return m_outer->get_menu_handle(pvalue); }
        xaml_result XAML_CALL set_handle(OBJC_OBJECT(NSMenu) value) noexcept override { return m_outer->set_menu_handle(value); }
    } m_native_popup_menu_item;

    using native_popup_menu_item_type = xaml_cocoa_popup_menu_item;

    XAML_PROP_INTERNAL_IMPL(menu_handle, OBJC_OBJECT(NSMenu) *, OBJC_OBJECT(NSMenu))
#endif // XAML_UI_WINDOWS

#ifndef XAML_UI_GTK3
    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<native_popup_menu_item_type>)
        {
            add_ref();
            *ptr = static_cast<native_popup_menu_item_type*>(&m_native_popup_menu_item);
            return XAML_S_OK;
        }
        else
        {
            return xaml_menu_item_implement::query(type, ptr);
        }
    }

    xaml_popup_menu_item_impl() noexcept
    {
        m_native_popup_menu_item.m_outer = this;
    }
#endif // !XAML_UI_GTK3
};

struct xaml_check_menu_item_internal : xaml_menu_item_internal
{
    XAML_EVENT_IMPL(is_checked_changed)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_checked() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_check_menu_item_impl : xaml_menu_item_implement<xaml_check_menu_item_impl, xaml_check_menu_item_internal, xaml_check_menu_item>
{
    XAML_EVENT_INTERNAL_IMPL(is_checked_changed)
    XAML_PROP_INTERNAL_IMPL(is_checked, bool*, bool)
};

struct xaml_radio_menu_item_internal : xaml_menu_item_internal
{
    XAML_EVENT_IMPL(is_checked_changed)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)
    XAML_PROP_PTR_IMPL(group, xaml_string)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_checked() noexcept;
    virtual xaml_result XAML_CALL draw_group() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_radio_menu_item_impl : xaml_menu_item_implement<xaml_radio_menu_item_impl, xaml_radio_menu_item_internal, xaml_radio_menu_item>
{
    XAML_EVENT_INTERNAL_IMPL(is_checked_changed)
    XAML_PROP_INTERNAL_IMPL(is_checked, bool*, bool)

    XAML_PROP_PTR_INTERNAL_IMPL(group, xaml_string)
};

struct xaml_separator_menu_item_internal : xaml_menu_item_internal
{
    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_COCOA
    xaml_result XAML_CALL draw_append(OBJC_OBJECT(NSMenu)) noexcept override;
#endif // XAML_UI_COCOA
};

struct xaml_separator_menu_item_impl : xaml_menu_item_implement<xaml_separator_menu_item_impl, xaml_separator_menu_item_internal, xaml_separator_menu_item>
{
};

#endif // !XAML_UI_CONTROLS_SHARED_MENU_ITEM_HPP
