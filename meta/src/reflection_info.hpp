#ifndef XAML_META_REFLECTION_INFO_IMPL_HPP
#define XAML_META_REFLECTION_INFO_IMPL_HPP

#include <xaml/meta/reflection_info.h>
#include <xaml/ptr.hpp>

template <typename T, typename... Base>
struct xaml_reflection_info_implement : xaml_implement<T, Base..., xaml_reflection_info, xaml_object>
{
protected:
    xaml_guid m_type;
    xaml_ptr<xaml_string> m_name;
    xaml_ptr<xaml_string> m_include_file;

public:
    xaml_reflection_info_implement(xaml_guid const& type, xaml_ptr<xaml_string>&& name, xaml_ptr<xaml_string>&& include_file) noexcept
        : m_type(type), m_name(std::move(name)), m_include_file(std::move(include_file)) {}

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        return m_name->query(ptr);
    }

    xaml_result XAML_CALL get_include_file(xaml_string** ptr) noexcept override
    {
        return m_include_file->query(ptr);
    }
};

#endif // !XAML_META_REFLECTION_INFO_IMPL_HPP
