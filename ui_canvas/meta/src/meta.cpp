#include <xaml/ui/controls/canvas.hpp>

using namespace std;
using namespace xaml;

extern "C"
{
    XAML_UI_CANVAS_META_API void init_meta(void* pctx) noexcept
    {
        meta_context& ctx = *(meta_context*)pctx;
        ctx.add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
        register_class<canvas>(ctx);
    }

    const char* const s_headers[] = {
        "xaml/ui/controls/canvas.hpp",
        nullptr
    };
    XAML_UI_CANVAS_META_API const char* const* include_headers() noexcept { return s_headers; }
}
