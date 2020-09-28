#include <xaml/meta/module.h>
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
#include <xaml/ui/controls/text_box.h>
#include <xaml/ui/controls/uniform_grid.h>

struct xaml_module_info_impl : xaml_implement<xaml_module_info_impl, xaml_module_info>
{
    xaml_ptr<xaml_vector<xaml_string>> m_dependencies;

    xaml_result XAML_CALL get_version(xaml_version* pver) noexcept override
    {
        *pver = xaml_version_current;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_dependencies(xaml_vector_view<xaml_string>** ptr) noexcept override
    {
        return m_dependencies->query(ptr);
    }

    xaml_result XAML_CALL register_types(xaml_meta_context* ctx) noexcept override
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
        XAML_RETURN_IF_FAILED(xaml_text_box_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_items_base_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_combo_box_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_menu_item_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_popup_menu_item_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_check_menu_item_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_radio_menu_item_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_separator_menu_item_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_orientation_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_grid_layout_register(ctx));
        XAML_META_CONTEXT_ADD_BASIC_TYPE(xaml_grid_length);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_dependencies));
        {
            xaml_ptr<xaml_string> dep;
            XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_ui"), &dep));
            XAML_RETURN_IF_FAILED(m_dependencies->append(dep));
        }
        return XAML_S_OK;
    }
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_module_get_info(xaml_module_info** ptr) noexcept
{
    return xaml_object_init<xaml_module_info_impl>(ptr);
}
