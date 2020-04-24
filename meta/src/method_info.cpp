#include <xaml/meta/method_info.h>

using namespace std;

struct xaml_method_info_impl : xaml_implement<xaml_method_info_impl, xaml_method_info, xaml_delegate, xaml_object>
{
private:
    xaml_ptr<xaml_string> m_name;
    function<xaml_result(xaml_vector_view*)> m_func;

public:
    xaml_method_info_impl(xaml_ptr<xaml_string>&& name, function<xaml_result(xaml_vector_view*)>&& func) noexcept : m_name(name), m_func(move(func)) {}

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        return m_name->query(ptr);
    }

    xaml_result XAML_CALL invoke(xaml_vector_view* args, xaml_object** ptr) noexcept override
    {
        xaml_result hr = m_func(args);
        if (XAML_SUCCEEDED(hr)) *ptr = nullptr;
        return hr;
    }
};

xaml_result xaml_method_info_new(xaml_string* name, xaml_result(XAML_CALL* func)(xaml_vector_view*), xaml_method_info** ptr) noexcept
{
    return xaml_object_new<xaml_method_info_impl>(ptr, name, func);
}

xaml_result xaml_method_info_new(xaml_string* name, std::function<xaml_result(xaml_vector_view*)>&& func, xaml_method_info** ptr) noexcept
{
    return xaml_object_new<xaml_method_info_impl>(ptr, name, move(func));
}
