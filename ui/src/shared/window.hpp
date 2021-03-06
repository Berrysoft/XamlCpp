#ifndef XAML_UI_SHARED_WINDOW_HPP
#define XAML_UI_SHARED_WINDOW_HPP

#ifdef XAML_UI_WINDOWS
    #include <xaml/ui/win/window.h>
#elif defined(XAML_UI_COCOA)
    #include <xaml/ui/cocoa/window.h>
#elif defined(XAML_UI_GTK3)
    #include <xaml/ui/gtk3/window.h>
#elif defined(XAML_UI_QT)
    #include <QMainWindow>
    #include <xaml/ui/qt5/window.hpp>
#endif // XAML_UI_WINDOWS

#include <atomic>
#include <shared/container.hpp>
#include <xaml/ui/window.h>

struct xaml_window_internal : xaml_container_internal
{
    std::atomic<bool> m_resizing{ false };

    XAML_UI_API xaml_result XAML_CALL init() noexcept override;

    XAML_PROP_PTR_IMPL(menu_bar, xaml_control)

    XAML_UI_API xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    XAML_UI_API xaml_result XAML_CALL show() noexcept;
    XAML_UI_API xaml_result XAML_CALL close() noexcept;
    XAML_UI_API xaml_result XAML_CALL hide() noexcept;

    XAML_EVENT_IMPL(is_resizable_changed, xaml_object, bool)
    XAML_PROP_EVENT_IMPL(is_resizable, bool, bool*, bool)
    XAML_EVENT_IMPL(location_changed, xaml_object, xaml_point)
    XAML_PROP_EVENT_IMPL(location, xaml_point, xaml_point*, xaml_point const&)

    xaml_result XAML_CALL get_x(double* pvalue) noexcept
    {
        *pvalue = m_location.x;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_x(double value) noexcept
    {
        if (m_location.x != value)
        {
            m_location.x = value;
            return m_location_changed->invoke(m_outer_this, m_location);
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_y(double* pvalue) noexcept
    {
        *pvalue = m_location.y;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_y(double value) noexcept
    {
        if (m_location.y != value)
        {
            m_location.y = value;
            return m_location_changed->invoke(m_outer_this, m_location);
        }
        return XAML_S_OK;
    }

    XAML_EVENT_IMPL(title_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(title)

    XAML_EVENT_IMPL(closing, xaml_object, xaml_box<bool>)

    XAML_UI_API xaml_result XAML_CALL get_client_region(xaml_rectangle*) noexcept;
    XAML_UI_API xaml_result XAML_CALL get_dpi(double*) noexcept;

    XAML_UI_API xaml_result XAML_CALL draw_size() noexcept override;
    XAML_UI_API virtual xaml_result XAML_CALL draw_title() noexcept;
    XAML_UI_API virtual xaml_result XAML_CALL draw_child() noexcept;
    XAML_UI_API virtual xaml_result XAML_CALL draw_resizable() noexcept;
    XAML_UI_API virtual xaml_result XAML_CALL draw_menu_bar() noexcept;

    xaml_result XAML_CALL size_to_fit() noexcept override { return XAML_S_OK; }
#ifdef XAML_UI_WINDOWS
    XAML_UI_API xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;

    XAML_UI_API xaml_result XAML_CALL get_real_location(xaml_point*) noexcept;
    XAML_UI_API xaml_result XAML_CALL set_real_location(xaml_point const&) noexcept;

    XAML_UI_API xaml_result XAML_CALL get_real_client_region(xaml_rectangle*) noexcept;
#elif defined(XAML_UI_COCOA)
    XAML_PROP_IMPL(window_handle, OBJC_OBJECT(NSWindow), OBJC_OBJECT(NSWindow)*, OBJC_OBJECT(NSWindow))
    XAML_PROP_IMPL(menu_bar_handle, OBJC_OBJECT(NSMenu), OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))
    XAML_PROP_IMPL(default_menu_item_handle, OBJC_OBJECT(NSMenuItem), OBJC_OBJECT(NSMenuItem)*, OBJC_OBJECT(NSMenuItem))
    XAML_PROP_IMPL(default_menu_item_menu_handle, OBJC_OBJECT(NSMenu), OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))

    XAML_UI_API xaml_result XAML_CALL draw_visible() noexcept override;

    void on_did_resize() noexcept;
    void on_did_move() noexcept;
    bool on_should_close() noexcept;
#elif defined(XAML_UI_GTK3)
    XAML_PROP_IMPL(window_handle, GtkWidget*, GtkWidget**, GtkWidget*)
    XAML_PROP_IMPL(vbox_handle, GtkWidget*, GtkWidget**, GtkWidget*)
    XAML_PROP_IMPL(menu_bar_handle, GtkWidget*, GtkWidget**, GtkWidget*)

    static void on_destroy(GtkWidget*, xaml_window_internal*) noexcept;
    static gboolean on_delete_event(GtkWidget*, GdkEvent*, xaml_window_internal*) noexcept;
    static gboolean on_configure_event(GtkWidget*, GdkEvent*, xaml_window_internal*) noexcept;
#elif defined(XAML_UI_QT)
    void on_resize_event(QResizeEvent* event) noexcept;
    void on_move_event(QMoveEvent* event) noexcept;
    void on_close_event(QCloseEvent* event) noexcept;
#endif // XAML_UI_WINDOWS

    XAML_UI_API xaml_window_internal() noexcept;
    XAML_UI_API ~xaml_window_internal();
};

template <typename T, typename Internal, typename Base>
struct xaml_window_implement : xaml_container_implement<T, Internal, Base>
{
    XAML_PROP_PTR_INTERNAL_IMPL(menu_bar, xaml_control)

    xaml_result XAML_CALL show() noexcept override { return this->m_internal.show(); }
    xaml_result XAML_CALL close() noexcept override { return this->m_internal.close(); }
    xaml_result XAML_CALL hide() noexcept override { return this->m_internal.hide(); }

    XAML_EVENT_INTERNAL_IMPL(is_resizable_changed, xaml_object, bool)
    XAML_PROP_INTERNAL_IMPL(is_resizable, bool*, bool)

    XAML_EVENT_INTERNAL_IMPL(location_changed, xaml_object, xaml_point)
    XAML_PROP_INTERNAL_IMPL(location, xaml_point*, xaml_point const&)

    XAML_PROP_INTERNAL_IMPL(x, double*, double)
    XAML_PROP_INTERNAL_IMPL(y, double*, double)

    XAML_EVENT_INTERNAL_IMPL(title_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(title, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(closing, xaml_object, xaml_box<bool>)

    XAML_PROP_INTERNAL_IMPL_BASE(client_region, xaml_rectangle*)
    XAML_PROP_INTERNAL_IMPL_BASE(dpi, double*)
#ifdef XAML_UI_WINDOWS
    XAML_PROP_INTERNAL_IMPL(real_location, xaml_point*, xaml_point const&)

    XAML_PROP_INTERNAL_IMPL_BASE(real_client_region, xaml_rectangle*)

    struct xaml_win32_window_impl : xaml_win32_control_implement<xaml_win32_window_impl, T, xaml_win32_window>
    {
        xaml_result XAML_CALL get_real_location(xaml_point* pvalue) noexcept override { return this->m_outer->get_real_location(pvalue); }
        xaml_result XAML_CALL set_real_location(xaml_point const& value) noexcept override { return this->m_outer->set_real_location(value); }
        xaml_result XAML_CALL get_real_client_region(xaml_rectangle* pvalue) noexcept override { return this->m_outer->get_real_client_region(pvalue); }
    } m_native_window;

    using native_window_type = xaml_win32_window;
#elif defined(XAML_UI_COCOA)
    XAML_PROP_INTERNAL_IMPL(window_handle, OBJC_OBJECT(NSWindow)*, OBJC_OBJECT(NSWindow))
    XAML_PROP_INTERNAL_IMPL(menu_bar_handle, OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))
    XAML_PROP_INTERNAL_IMPL(default_menu_item_handle, OBJC_OBJECT(NSMenuItem)*, OBJC_OBJECT(NSMenuItem))
    XAML_PROP_INTERNAL_IMPL(default_menu_item_menu_handle, OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))

    struct xaml_cocoa_window_impl : xaml_cocoa_control_implement<xaml_cocoa_window_impl, T, xaml_cocoa_window>
    {
        xaml_result XAML_CALL get_window(OBJC_OBJECT(NSWindow)* pvalue) noexcept override { return this->m_outer->get_window_handle(pvalue); }
        xaml_result XAML_CALL set_window(OBJC_OBJECT(NSWindow) value) noexcept override { return this->m_outer->set_window_handle(value); }
        xaml_result XAML_CALL get_menu_bar(OBJC_OBJECT(NSMenu)* pvalue) noexcept override { return this->m_outer->get_menu_bar_handle(pvalue); }
        xaml_result XAML_CALL set_menu_bar(OBJC_OBJECT(NSMenu) value) noexcept override { return this->m_outer->set_menu_bar_handle(value); }
    } m_native_window;

    using native_window_type = xaml_cocoa_window;
#elif defined(XAML_UI_GTK3)
    XAML_PROP_INTERNAL_IMPL(window_handle, GtkWidget**, GtkWidget*)
    XAML_PROP_INTERNAL_IMPL(vbox_handle, GtkWidget**, GtkWidget*)
    XAML_PROP_INTERNAL_IMPL(menu_bar_handle, GtkWidget**, GtkWidget*)

    struct xaml_gtk3_window_impl : xaml_gtk3_control_implement<xaml_gtk3_window_impl, T, xaml_gtk3_window>
    {
        xaml_result XAML_CALL get_window(GtkWidget** pvalue) noexcept override { return this->m_outer->get_window_handle(pvalue); }
        xaml_result XAML_CALL set_window(GtkWidget* value) noexcept override { return this->m_outer->set_window_handle(value); }
        xaml_result XAML_CALL get_vbox(GtkWidget** pvalue) noexcept override { return this->m_outer->get_vbox_handle(pvalue); }
        xaml_result XAML_CALL set_vbox(GtkWidget* value) noexcept override { return this->m_outer->set_vbox_handle(value); }
        xaml_result XAML_CALL get_menu_bar(GtkWidget** pvalue) noexcept override { return this->m_outer->get_menu_bar_handle(pvalue); }
        xaml_result XAML_CALL set_menu_bar(GtkWidget* value) noexcept override { return this->m_outer->set_menu_bar_handle(value); }
    } m_native_window;

    using native_window_type = xaml_gtk3_window;
#elif defined(XAML_UI_QT)
    struct xaml_qt5_window_impl : xaml_qt5_control_implement<xaml_qt5_window_impl, T, xaml_qt5_window>
    {
    } m_native_window;

    using native_window_type = xaml_qt5_window;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<native_window_type>)
        {
            this->add_ref();
            *ptr = static_cast<native_window_type*>(&m_native_window);
            return XAML_S_OK;
        }
        else
        {
            return xaml_container_implement<T, Internal, Base>::query(type, ptr);
        }
    }

    xaml_window_implement() noexcept
    {
        m_native_window.m_outer = static_cast<T*>(this);
    }
};

struct xaml_window_impl : xaml_window_implement<xaml_window_impl, xaml_window_internal, xaml_window>
{
};

#endif // !XAML_UI_SHARED_WINDOW_HPP
