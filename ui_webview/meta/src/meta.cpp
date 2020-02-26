#include <xaml/ui/webview/meta.hpp>
#include <xaml/ui/webview/webview.hpp>

using namespace std;
using namespace xaml;

extern "C" XAML_UI_WEBVIEW_META_API void init_meta(void* ctx) noexcept
{
    meta_context* pctx = (meta_context*)ctx;
    ui_webview_init_traits::init_meta(*pctx);
}

namespace xaml
{
    void ui_webview_init_traits::init_meta(meta_context& ctx) noexcept
    {
        register_class<webview>(ctx);
    }
} // namespace xaml
