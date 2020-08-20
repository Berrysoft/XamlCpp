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

inline xaml_result xaml_string_new(QString const& str, xaml_string** ptr) noexcept
{
    auto arr = str.toUtf8();
    return xaml_string_new_length(arr.data(), arr.size(), ptr);
}

#endif // !XAML_UI_QT5_QSTRING_HPP
