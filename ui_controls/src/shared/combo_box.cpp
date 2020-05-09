#include <shared/combo_box.hpp>
#include <xaml/ui/controls/combo_box.h>

using namespace std;

xaml_result xaml_combo_box_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_items_base_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_editable_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_items_changed->add_noexcept<xaml_ptr<xaml_items_base>, xaml_ptr<xaml_observable_vector>>(
        [this](xaml_ptr<xaml_items_base>, xaml_ptr<xaml_observable_vector>) -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_items());
            return XAML_S_OK;
        },
        &token)));

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_new(xaml_combo_box** ptr) noexcept
{
    return xaml_object_init<xaml_combo_box_impl>(ptr);
}

xaml_result XAML_CALL xaml_combo_box_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_combo_box;
    XAML_RETURN_IF_FAILED(xaml_items_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_combo_box_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_editable, bool);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_combo_box, "xaml/ui/controls/combo_box.h");
    XAML_RETURN_IF_FAILED(xaml_combo_box_members(__info.get()));
    return ctx->add_type(__info.get());
}
