#include <xaml/ui/controls/button.h>
#include <xaml/ui/controls/check_box.h>
#include <xaml/ui/controls/combo_box.h>
#include <xaml/ui/controls/entry.h>
#include <xaml/ui/controls/grid.h>
#include <xaml/ui/controls/label.h>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/controls/password_entry.h>
#include <xaml/ui/controls/progress.h>
#include <xaml/ui/controls/radio_box.h>
#include <xaml/ui/controls/stack_panel.h>
#include <xaml/ui/controls/uniform_grid.h>
#include <xaml/version.h>

EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_module_version(xaml_version* pver) noexcept
{
    *pver = xaml_version_current;
    return XAML_S_OK;
}

EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_module_register(xaml_meta_context* ctx) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_layout_base_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_grid_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_uniform_grid_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_stack_panel_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_button_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_check_box_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_radio_box_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_label_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_entry_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_password_entry_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_items_base_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_combo_box_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_menu_item_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_popup_menu_item_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_check_menu_item_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_radio_menu_item_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_separator_menu_item_register(ctx));
    // TODO: enums
    return XAML_S_OK;
}

//static ostream& compile_grid_length(ostream& stream, grid_length length)
//{
//    stream << "{ " << length.value << ", ";
//    stream << "::xaml::grid_layout::" << enum_meta<grid_layout, char>{}(length.layout);
//    return stream << " }";
//}
//
//extern "C"
//{
//    XAML_UI_CONTROLS_META_API void xaml_version(version* ver) noexcept { *ver = current_version; }
//
//    XAML_UI_CONTROLS_META_API void init_meta(void* pctx) noexcept
//    {
//        meta_context& ctx = *(meta_context*)pctx;
//        ctx.add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
//        register_class<
//            grid, uniform_grid, stack_panel,
//            button, check_box, radio_box,
//            label,
//            menu_item, check_menu_item, radio_menu_item, separator_menu_item,
//            entry, password_entry,
//            combo_box>(ctx);
//        REGISTER_ENUM(xaml, grid_layout, "xaml/ui/controls/grid.hpp");
//        REGISTER_ENUM(xaml, orientation, "xaml/ui/controls/layout_base.hpp");
//    }
//
//    XAML_UI_CONTROLS_META_API int can_compile(void const* t) noexcept
//    {
//        guid const& type = *(guid const*)t;
//        return (type == type_guid_v<meta_box<vector<grid_length>>>) ? 1 : 0;
//    }
//
//    XAML_UI_CONTROLS_META_API void compile(void const* t, const char* code, int32_t code_len, void* res) noexcept
//    {
//        if (can_compile(t))
//        {
//            auto lengths = value_converter_traits<array_view<grid_length>>::convert(box_value(string_view{ code, code_len }));
//            ostringstream stream;
//            stream << "{ ";
//            auto bit = lengths.begin();
//            auto eit = lengths.end();
//            if (bit != eit)
//            {
//                compile_grid_length(stream, *bit);
//                for (++bit; bit != eit; ++bit)
//                {
//                    compile_grid_length(stream << ", ", *bit);
//                }
//            }
//            stream << " }";
//            *(string*)res = stream.str();
//            return;
//        }
//        *(string*)res = {};
//    }
//}
