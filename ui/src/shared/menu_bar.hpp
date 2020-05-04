#ifndef XAML_UI_SHARED_MENU_BAR_HPP
#define XAML_UI_SHARED_MENU_BAR_HPP

#include <shared/container.hpp>
#include <xaml/ui/menu_bar.h>

#ifdef XAML_UI_WINDOWS
#include <wil/resource.h>
#include <xaml/ui/win/menu_bar.h>
#elif defined(XAML_UI_GTK3)
#include <xaml/ui/gtk3/menu_bar.h>
#endif // XAML_UI_WINDOWS

struct xaml_menu_bar_internal : xaml_multicontainer_internal
{
    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_submenu() noexcept;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL draw_visible() noexcept override;

protected:
    wil::unique_hmenu m_menu{};

public:
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
#elif defined(XAML_UI_GTK3)
    XAML_PROP_IMPL(menu, GtkMenuBar*, GtkMenuBar**, GtkMenuBar*)
#endif // XAML_UI_WINDOWS
};

struct xaml_menu_bar_impl : xaml_multicontainer_implement<xaml_menu_bar_impl, xaml_menu_bar_internal, xaml_menu_bar>
{
#ifdef XAML_UI_WINDOWS
    XAML_PROP_INTERNAL_IMPL(menu, HMENU*, HMENU)

    struct xaml_win32_menu_bar_impl : xaml_inner_implement<xaml_win32_menu_bar_impl, xaml_menu_bar_impl, xaml_win32_menu_bar>
    {
        xaml_result XAML_CALL get_handle(HMENU* pvalue) noexcept override { return m_outer->get_menu(pvalue); }
        xaml_result XAML_CALL set_handle(HMENU value) noexcept override { return m_outer->set_menu(value); }
    } m_native_menu_bar;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_menu_bar>)
        {
            add_ref();
            *ptr = static_cast<xaml_win32_menu_bar*>(&m_native_menu_bar);
            return XAML_S_OK;
        }
        else
        {
            return xaml_multicontainer_implement::query(type, ptr);
        }
    }
#elif defined(XAML_UI_GTK3)
    XAML_PROP_INTERNAL_IMPL(menu, GtkMenuBar**, GtkMenuBar*)

    struct xaml_gtk3_menu_bar_impl : xaml_inner_implement<xaml_gtk3_menu_bar_impl, xaml_menu_bar_impl, xaml_gtk3_menu_bar>
    {
        xaml_result XAML_CALL get_handle(GtkMenuBar** pvalue) noexcept override { return m_outer->get_menu(pvalue); }
        xaml_result XAML_CALL set_handle(GtkMenuBar* value) noexcept override { return m_outer->set_menu(value); }
    } m_native_menu_bar;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_gtk3_menu_bar>)
        {
            add_ref();
            *ptr = static_cast<xaml_gtk3_menu_bar*>(&m_native_menu_bar);
            return XAML_S_OK;
        }
        else
        {
            return xaml_multicontainer_implement::query(type, ptr);
        }
    }
#endif // XAML_UI_WINDOWS

    xaml_menu_bar_impl() noexcept : xaml_multicontainer_implement()
    {
        m_native_menu_bar.m_outer = this;
    }
};

#endif // !XAML_UI_SHARED_MENU_BAR_HPP
