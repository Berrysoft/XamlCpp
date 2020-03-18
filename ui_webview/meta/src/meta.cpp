#include <xaml/ui/controls/webview.hpp>
#include <xaml/version.hpp>

using namespace xaml;

extern "C"
{
    XAML_UI_WEBVIEW_META_API void xaml_version(version* ver) noexcept { *ver = current_version; }

    XAML_UI_WEBVIEW_META_API void init_meta(void* pctx) noexcept
    {
        meta_context& ctx = *(meta_context*)pctx;
        ctx.add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
        register_class<webview>(ctx);
    }
}
