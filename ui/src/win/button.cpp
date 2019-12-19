#include <wil/result_macros.h>
#include <xaml/ui/button.hpp>

namespace xaml
{
    button::button() : control() {}

    void button::create()
    {
        window_create_params params = {};
        params.class_name = U("BUTTON");
        params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
        params.x = 0;
        params.y = 0;
        params.width = 100;
        params.height = 50;
        params.parent = get_parent().get();
        this->control::create(params);
    }

    button::~button() {}

    string_t button::get_text() const
    {
        int count = GetWindowTextLength(get_handle());
        string_t result(count, L'\0');
        GetWindowText(get_handle(), result.data(), count);
        return result;
    }

    void button::set_text(string_view_t value)
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), value.data()));
    }

    bool button::get_is_default() const
    {
        auto style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        return (style & BS_DEFPUSHBUTTON) == BS_DEFPUSHBUTTON;
    }

    void button::set_is_default(bool value)
    {
        auto style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        if (value)
            style |= BS_DEFPUSHBUTTON;
        else
            style &= ~BS_DEFPUSHBUTTON;
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
    }
} // namespace xaml
