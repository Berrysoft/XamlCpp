#ifndef XAML_UI_SHARED_FILEBOX_HPP
#define XAML_UI_SHARED_FILEBOX_HPP

#include <xaml/ui/filebox.h>

template <typename I>
struct xaml_filebox_impl : xaml_implement<xaml_filebox_impl<I>, I>
{
    XAML_PROP_PTR_IMPL(title, xaml_string)
    XAML_PROP_PTR_IMPL(filename, xaml_string)
    XAML_PROP_PTR_IMPL(filters, xaml_vector_view)
    XAML_PROP_IMPL(multiple, bool, bool*, bool)

protected:
    xaml_ptr<xaml_vector> m_results{};

public:
    xaml_result XAML_CALL get_result(xaml_string** ptr) noexcept
    {
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(m_results->get_at(0, &obj));
        return obj->query(ptr);
    }

    xaml_result XAML_CALL get_results(xaml_vector_view** ptr) noexcept
    {
        return m_results->query(ptr);
    }

    xaml_result XAML_CALL show(xaml_window*) noexcept override;
};

extern template struct xaml_filebox_impl<xaml_open_filebox>;
extern template struct xaml_filebox_impl<xaml_save_filebox>;

#endif // !XAML_UI_SHARED_FILEBOX_HPP
