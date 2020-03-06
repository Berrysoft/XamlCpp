#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/check_box.hpp>
#include <xaml/ui/controls/combo_box.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/controls/menu_item.hpp>
#include <xaml/ui/controls/password_entry.hpp>
#include <xaml/ui/controls/progress.hpp>
#include <xaml/ui/controls/radio_box.hpp>
#include <xaml/ui/controls/stack_panel.hpp>
#include <xaml/ui/controls/uniform_grid.hpp>

using namespace std;
using namespace xaml;

static ostream& compile_grid_length(ostream& stream, grid_length length)
{
    stream << "{ " << length.value << ", ";
    stream << "::xaml::grid_layout::" << enum_meta<grid_layout, char>{}(length.layout);
    return stream << " }";
}

extern "C"
{
    XAML_UI_CONTROLS_META_API void init_meta(void* pctx) noexcept
    {
        meta_context& ctx = *(meta_context*)pctx;
        register_class<
            grid, uniform_grid, stack_panel,
            button, check_box, radio_box,
            label,
            menu_item, check_menu_item, radio_menu_item, separator_menu_item,
            entry, password_entry,
            combo_box>(ctx);
        REGISTER_ENUM(xaml, grid_layout);
        REGISTER_ENUM(xaml, orientation);
    }

    XAML_UI_CONTROLS_META_API int can_compile(void* t) noexcept
    {
        type_index& type = *(type_index*)t;
        return (type == type_index(typeid(array_view<grid_length>))) ? 1 : 0;
    }

    XAML_UI_CONTROLS_META_API void compile(void* t, const char* code, void* res) noexcept
    {
        auto lengths = value_converter_traits<array_view<grid_length>>::convert(code);
        ostringstream stream;
        stream << "{ ";
        auto bit = lengths.begin();
        auto eit = lengths.end();
        if (bit != eit)
        {
            compile_grid_length(stream, *bit);
            for (++bit; bit != eit; ++bit)
            {
                compile_grid_length(stream << ", ", *bit);
            }
        }
        stream << " }";
        *(string*)res = stream.str();
    }

    const char* const s_headers[] = {
        "xaml/ui/controls/button.hpp",
        "xaml/ui/controls/check_box.hpp",
        "xaml/ui/controls/combo_box.hpp",
        "xaml/ui/controls/entry.hpp",
        "xaml/ui/controls/grid.hpp",
        "xaml/ui/controls/label.hpp",
        "xaml/ui/controls/menu_item.hpp",
        "xaml/ui/controls/password_entry.hpp",
        "xaml/ui/controls/progress.hpp",
        "xaml/ui/controls/radio_box.hpp",
        "xaml/ui/controls/stack_panel.hpp",
        "xaml/ui/controls/uniform_grid.hpp",
        nullptr
    };
    XAML_UI_CONTROLS_META_API const char* const* include_headers() noexcept { return s_headers; }
}
