#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/controls/meta.hpp>
#include <xaml/ui/controls/password_entry.hpp>

using namespace std;
using namespace xaml;

extern "C"
{
    XAML_UI_CONTROLS_META_API void init_meta(void* ctx) noexcept
    {
        shared_ptr<meta_context>* pctx = (shared_ptr<meta_context>*)ctx;
        ui_controls_init_traits::init_meta(*pctx);
    }
}

namespace xaml
{
    void ui_controls_init_traits::init_meta(shared_ptr<meta_context> const& ctx) noexcept
    {
        init_context(ctx);
        register_class<
            grid,
            canvas,
            button_base,
            button,
            label,
            entry,
            password_entry>();
        REGISTER_ENUM(xaml, grid_layout);
    }
} // namespace xaml
