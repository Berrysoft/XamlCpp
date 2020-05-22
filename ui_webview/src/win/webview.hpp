#ifndef XAML_UI_WEBVIEW_NATIVE_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_NATIVE_WEBVIEW_HPP

#include <Windows.h>
#include <functional>
#include <xaml/ui/controls/webview.h>

struct xaml_win32_webview
{
public:
    virtual ~xaml_win32_webview() {}
    virtual xaml_result create_async(HWND parent, xaml_rectangle const& rect, std::function<xaml_result()>&& callback = {}) noexcept = 0;
    virtual operator bool() const noexcept = 0;
    virtual xaml_result navigate(char const* uri) noexcept = 0;
    virtual xaml_result set_visible(bool vis) noexcept = 0;
    virtual xaml_result set_location(xaml_point const& p) noexcept = 0;
    virtual xaml_result set_size(xaml_size const& s) noexcept = 0;
    virtual xaml_result set_rect(xaml_rectangle const& rect) noexcept = 0;
    virtual xaml_result get_can_go_forward(bool*) noexcept = 0;
    virtual xaml_result go_forward() noexcept = 0;
    virtual xaml_result get_can_go_back(bool*) noexcept = 0;
    virtual xaml_result go_back() noexcept = 0;

#define __NATIVE_WEBVIEW_EVENT(name, arg)                                \
private:                                                                 \
    std::function<xaml_result(arg)> m_##name{};                          \
                                                                         \
public:                                                                  \
    void set_##name(std::function<xaml_result(arg)>&& callback) noexcept \
    {                                                                    \
        m_##name = std::move(callback);                                  \
    }                                                                    \
    xaml_result invoke_##name(arg a) noexcept                            \
    {                                                                    \
        if (m_##name)                                                    \
            return m_##name(a);                                          \
        else                                                             \
            return XAML_S_OK;                                            \
    }

    __NATIVE_WEBVIEW_EVENT(navigated, char const*)
    __NATIVE_WEBVIEW_EVENT(resource_requested, xaml_ptr<xaml_webview_resource_requested_args>)

#undef __NATIVE_WEBVIEW_EVENT
};

#endif // !XAML_UI_WEBVIEW_NATIVE_WEBVIEW_HPP
