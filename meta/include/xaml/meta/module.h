#ifndef XAML_META_MODULE_H
#define XAML_META_MODULE_H

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_module, { 0x03e2b52a, 0x7f9d, 0x4cc8, { 0x84, 0x70, 0x30, 0x0e, 0x3e, 0x6d, 0x5e, 0x5f } })

#define XAML_MODULE_VTBL(type)                  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));  \
    XAML_METHOD(open, type, xaml_string*);      \
    XAML_METHOD(get_name, type, xaml_string**); \
    XAML_METHOD(get_method, type, xaml_string*, void**)

XAML_DECL_INTERFACE_(xaml_module, xaml_object)
{
    XAML_DECL_VTBL(xaml_module, XAML_MODULE_VTBL);

#ifdef __cplusplus
    template <typename F>
    xaml_result XAML_CALL get_method(std::string_view name, F * *ptr) noexcept
    {
        xaml_ptr<xaml_string> name_str;
        XAML_RETURN_IF_FAILED(xaml_string_new_view(name, &name_str));
        return get_method(name_str, (void**)ptr);
    }
#endif // __cplusplus
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_module_new(xaml_module**) XAML_NOEXCEPT;

#endif // !XAML_META_MODULE_H
