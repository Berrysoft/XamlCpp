#ifndef XAML_META_REFLECTION_INFO_H
#define XAML_META_REFLECTION_INFO_H

#ifdef __cplusplus
#include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_reflection_info, { 0x5abbe92c, 0x833d, 0x4d24, { 0xa7, 0x06, 0x1d, 0x5c, 0xe5, 0x14, 0x1b, 0x0a } })

#define XAML_REFLECTION_INFO_VTBL(type)         \
    XAML_OBJECT_VTBL(type);                     \
    XAML_METHOD(get_type, type, xaml_guid*);    \
    XAML_METHOD(get_name, type, xaml_string**); \
    XAML_METHOD(get_include_file, type, xaml_string**)

XAML_DECL_INTERFACE_(xaml_reflection_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_reflection_info, XAML_REFLECTION_INFO_VTBL);
};

#ifdef __cplusplus
template <typename T, typename... Base>
struct xaml_reflection_info_implement : xaml_implement<T, Base..., xaml_reflection_info, xaml_object>
{
protected:
    xaml_guid m_type;
    xaml_ptr<xaml_string> m_name;
    xaml_ptr<xaml_string> m_include_file;

public:
    xaml_reflection_info_implement(xaml_guid const& type, xaml_ptr<xaml_string>&& name, xaml_ptr<xaml_string>&& include_file)
        : m_type(type), m_name(std::move(name)), m_include_file(std::move(include_file)) {}

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        m_name.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_include_file(xaml_string** ptr) noexcept override
    {
        m_include_file.add_ref_to(ptr);
        return XAML_S_OK;
    }
};
#endif // __cplusplus

#endif // !XAML_META_REFLECTION_INFO_H
