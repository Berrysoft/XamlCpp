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
#include <xaml/version.hpp>

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
    XAML_UI_CONTROLS_META_API void xaml_version(version* ver) noexcept { *ver = current_version; }

    XAML_UI_CONTROLS_META_API void init_meta(void* pctx) noexcept
    {
        meta_context& ctx = *(meta_context*)pctx;
        ctx.add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
        register_class<
            grid, uniform_grid, stack_panel,
            button, check_box, radio_box,
            label,
            menu_item, check_menu_item, radio_menu_item, separator_menu_item,
            entry, password_entry,
            combo_box>(ctx);
        REGISTER_ENUM(xaml, grid_layout, "xaml/ui/controls/grid.hpp");
        REGISTER_ENUM(xaml, orientation, "xaml/ui/controls/layout_base.hpp");
    }

    XAML_UI_CONTROLS_META_API int can_compile(void* t) noexcept
    {
        type_index& type = *(type_index*)t;
        return (type == type_index(typeid(array_view<grid_length>))) ? 1 : 0;
    }

    XAML_UI_CONTROLS_META_API void compile(void* t, const char* code, size_t code_len, void* res) noexcept
    {
        if (can_compile(t))
        {
            auto lengths = value_converter_traits<array_view<grid_length>>::convert(box_value(string_view{ code, code_len }));
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
            return;
        }
        *(string*)res = {};
    }
}
