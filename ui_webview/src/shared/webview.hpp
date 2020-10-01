#ifndef XAML_UI_WEBVIEW_SHARED_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_SHARED_WEBVIEW_HPP

#include <atomic>
#include <shared/control.hpp>
#include <xaml/ui/controls/webview.h>

#ifdef XAML_UI_WINDOWS
    #include <win/webview.hpp>
#elif defined(XAML_UI_GTK3)
    #include <webkit2/webkit2.h>
#endif // XAML_UI_WINDOWS

struct xaml_webview_internal : xaml_control_internal
{
    std::atomic_bool m_navigating{ false };

    XAML_EVENT_IMPL(uri_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(uri)

    XAML_EVENT_IMPL(resource_requested, xaml_object, xaml_webview_resource_requested_args)

    xaml_result XAML_CALL get_can_go_forward(bool*) noexcept;
    xaml_result XAML_CALL get_can_go_back(bool*) noexcept;

    xaml_result XAML_CALL go_forward() noexcept;
    xaml_result XAML_CALL go_back() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_uri() noexcept;

#ifdef XAML_UI_WINDOWS
    std::shared_ptr<xaml_win32_webview> m_webview{ nullptr };
    std::atomic_bool m_created{ false };

    xaml_result XAML_CALL create_edge2(xaml_rectangle const&) noexcept;
    xaml_result XAML_CALL create_edge(xaml_rectangle const&) noexcept;
    xaml_result XAML_CALL create_ie(xaml_rectangle const&) noexcept;

    virtual xaml_result XAML_CALL draw_create() noexcept;
    xaml_result XAML_CALL draw_size() noexcept override;
    xaml_result XAML_CALL draw_visible() noexcept override;
#elif defined(XAML_UI_COCOA)
    void on_navigated() noexcept;
#elif defined(XAML_UI_GTK3)
    static void on_load_changed(WebKitWebView*, WebKitLoadEvent, xaml_webview_internal*) noexcept;
#elif defined(XAML_UI_QT5)
    void on_url_changed(QUrl const&) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_webview_impl : xaml_control_implement<xaml_webview_impl, xaml_webview_internal, xaml_webview>
{
    XAML_EVENT_INTERNAL_IMPL(uri_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(uri, xaml_string)
    XAML_PROP_INTERNAL_IMPL_BASE(can_go_forward, bool*)
    XAML_PROP_INTERNAL_IMPL_BASE(can_go_back, bool*)
    XAML_EVENT_INTERNAL_IMPL(resource_requested, xaml_object, xaml_webview_resource_requested_args)

    xaml_result XAML_CALL go_forward() noexcept override { return m_internal.go_forward(); }
    xaml_result XAML_CALL go_back() noexcept override { return m_internal.go_back(); }
};

struct xaml_webview_web_request_impl : xaml_implement<xaml_webview_web_request_impl, xaml_webview_web_request>
{
    XAML_PROP_PTR_IMPL(method, xaml_string)
    XAML_PROP_PTR_IMPL(uri, xaml_string)
    XAML_PROP_PTR_IMPL(data, xaml_buffer)
};

struct xaml_webview_web_response_impl : xaml_implement<xaml_webview_web_response_impl, xaml_webview_web_response>
{
    XAML_PROP_PTR_IMPL(content_type, xaml_string)
    XAML_PROP_PTR_IMPL(data, xaml_buffer)
};

struct xaml_webview_resource_requested_args_impl : xaml_implement<xaml_webview_resource_requested_args_impl, xaml_webview_resource_requested_args>
{
    XAML_PROP_PTR_IMPL(request, xaml_webview_web_request)
    XAML_PROP_PTR_IMPL(response, xaml_webview_web_response)
};

#endif // !XAML_UI_WEBVIEW_SHARED_WEBVIEW_HPP
