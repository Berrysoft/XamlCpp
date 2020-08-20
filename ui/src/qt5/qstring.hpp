#ifndef XAML_UI_QT5_QSTRING_HPP
#define XAML_UI_QT5_QSTRING_HPP

#include <QString>
#include <xaml/string.h>

inline xaml_result to_QString(xaml_string* str, QString* ptr) noexcept
{
    char const* data = nullptr;
    if (str)
    {
        XAML_RETURN_IF_FAILED(str->get_data(&data));
    }
    *ptr = data;
    return XAML_S_OK;
}

#endif // !XAML_UI_QT5_QSTRING_HPP
