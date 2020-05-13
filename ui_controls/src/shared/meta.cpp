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
    XAML_RETURN_IF_FAILED(xaml_orientation_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_grid_layout_register(ctx));
    XAML_META_CONTEXT_ADD_BASIC_TYPE(xaml_grid_length);
    return XAML_S_OK;
}
