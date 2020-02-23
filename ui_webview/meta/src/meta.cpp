#include <xaml/ui/webview/meta.hpp>
#include <xaml/ui/webview/webview.hpp>

using namespace std;
using namespace xaml;

extern "C" XAML_UI_WEBVIEW_META_API void init_meta(void* ctx) noexcept
{
    shared_ptr<meta_context>* pctx = (shared_ptr<meta_context>*)ctx;
    ui_webview_init_traits::init_meta(pctx ? *pctx : nullptr);
}

namespace xaml
{
    void ui_webview_init_traits::init_meta(shared_ptr<meta_context> const& ctx) noexcept
    {
        init_context(ctx);
        register_class<webview>();
    }
} // namespace xaml
