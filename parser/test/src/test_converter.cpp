#include <test_converter.h>
#include <xaml/meta/meta_macros.h>

struct xaml_test_converter_impl : xaml_implement<xaml_test_converter_impl, xaml_test_converter, xaml_converter, xaml_object>
{
    xaml_result XAML_CALL convert(xaml_object* value, xaml_guid const&, xaml_object*, xaml_string*, xaml_object** ptr) noexcept override
    {
        xaml_ptr<xaml_string> str;
        XAML_RETURN_IF_FAILED(value->query(&str));
        xaml_ptr<xaml_string> prefix;
        XAML_RETURN_IF_FAILED(xaml_string_new_view(U("Text: "), &prefix));
        xaml_ptr<xaml_string> result;
        XAML_RETURN_IF_FAILED(xaml_string_concat(prefix, str, &result));
        return result->query(ptr);
    }

    xaml_result XAML_CALL convert_back(xaml_object*, xaml_guid const&, xaml_object*, xaml_string*, xaml_object**) noexcept
    {
        return XAML_E_NOTIMPL;
    }
};

xaml_result XAML_CALL xaml_test_converter_new(xaml_test_converter** ptr) noexcept
{
    return xaml_object_new<xaml_test_converter_impl>(ptr);
}

xaml_result XAML_CALL xaml_test_converter_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_test_converter, "test_converter.h");
    XAML_TYPE_INFO_ADD_CTOR(xaml_test_converter_new);
    return ctx->add_type(__info);
}
