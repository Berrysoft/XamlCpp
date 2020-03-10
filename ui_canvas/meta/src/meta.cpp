#include <xaml/ui/controls/canvas.hpp>

using namespace std;
using namespace xaml;

extern "C"
{
    XAML_UI_CANVAS_META_API void init_meta(void* pctx) noexcept
    {
        meta_context& ctx = *(meta_context*)pctx;
        register_class<canvas>(ctx);
    }
}
