#ifndef XAML_UI_WEBVIEW_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_WEBVIEW_HPP

#include <xaml/ui/control.hpp>

namespace xaml
{
    class webview : public control
    {
    public:
        XAML_UI_WEBVIEW_API webview();
        XAML_UI_WEBVIEW_API ~webview() override;

    public:
        XAML_UI_WEBVIEW_API void __draw(rectangle const& region) override;

    public:
        PROP_STRING(uri)

    public:
#define ADD_WEBVIEW_MEMBERS() \
    ADD_CONTROL_MEMBERS();    \
    ADD_PROP(uri)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, webview);
            ADD_CTOR_DEF();
            ADD_WEBVIEW_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_WEBVIEW_WEBVIEW_HPP
