#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/controls/meta.hpp>
#include <xaml/ui/controls/password_entry.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <gdiplus.h>
#endif // XAML_UI_WINDOWS

using namespace std;
using namespace xaml;

#ifdef XAML_UI_WINDOWS
using namespace Gdiplus;
#endif // XAML_UI_WINDOWS

extern "C"
{
    XAML_API void init_meta(void* ctx) noexcept
    {
        shared_ptr<meta_context>* pctx = (shared_ptr<meta_context>*)ctx;
        ui_controls_init_traits::init_meta(*pctx);
    }

    XAML_API void* init_components() noexcept
    {
        return ui_controls_init_traits::init_components();
    }

    XAML_API void cleanup_components(void* token) noexcept
    {
        ui_controls_init_traits::cleanup_components(token);
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

    void* ui_controls_init_traits::init_components() noexcept
    {
#ifdef XAML_UI_WINDOWS
        ULONG_PTR token;
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&token, &gdiplusStartupInput, NULL);
        return (void*)token;
#else
        return nullptr;
#endif // XAML_UI_WINDOWS
    }

    void ui_controls_init_traits::cleanup_components(void* token) noexcept
    {
#ifdef XAML_UI_WINDOWS
        GdiplusShutdown((ULONG_PTR)token);
#endif // XAML_UI_WINDOWS
    }
} // namespace xaml
