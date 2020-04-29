#include <shared/check_box.hpp>
#include <xaml/ui/controls/check_box.h>

using namespace std;

xaml_result xaml_check_box_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_button_implement::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    std::int32_t token;
    XAML_RETURN_IF_FAILED((m_is_checked_changed->add_noexcept<xaml_ptr<xaml_check_box>, bool>(
        [this](xaml_ptr<xaml_check_box>, bool) -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_checked());
            return XAML_S_OK;
        },
        &token)));

#ifdef XAML_UI_COCOA
    XAML_RETURN_IF_FAILED((m_click->add_noexcept<xaml_ptr<xaml_check_box>>(
        [this](xaml_ptr<xaml_check_box>) -> xaml_result {
            // TODO
        },
        &token)));
#endif // XAML_UI_COCOA
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_check_box_new(xaml_check_box** ptr) noexcept
{
    return xaml_object_init<xaml_check_box_impl>(ptr);
}

xaml_result XAML_CALL xaml_check_box_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_check_box;
    XAML_RETURN_IF_FAILED(xaml_button_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_check_box_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_checked);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_check_box_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_check_box, "xaml/ui/controls/check_box.h");
    XAML_RETURN_IF_FAILED(xaml_check_box_members(__info.get()));
    return ctx->add_type(__info.get());
}
