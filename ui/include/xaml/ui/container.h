#ifndef XAML_UI_CONTAINER_H
#define XAML_UI_CONTAINER_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_container, { 0x211a9bce, 0xc31a, 0x42ab, { 0x8f, 0x4e, 0x2d, 0x45, 0x0f, 0xb2, 0xc3, 0xa9 } })

#define XAML_CONTAINER_VTBL(type)                          \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));            \
    XAML_PROP(child, type, xaml_control**, xaml_control*); \
    XAML_EVENT(child_changed, type, xaml_object, xaml_control)

XAML_DECL_INTERFACE_(xaml_container, xaml_control)
{
    XAML_DECL_VTBL(xaml_container, XAML_CONTAINER_VTBL);
};

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_container_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_container_register(xaml_meta_context*) XAML_NOEXCEPT;

#ifndef xaml_enumerator__xaml_control__defined
    #define xaml_enumerator__xaml_control__defined
XAML_ENUMERATOR_T_TYPE(XAML_T_O(xaml_control))
#endif // !xaml_enumerator__xaml_control__defined

#ifndef xaml_vector_view__xaml_control__defined
    #define xaml_vector_view__xaml_control__defined
XAML_VECTOR_VIEW_T_TYPE(XAML_T_O(xaml_control))
#endif // !xaml_vector_view__xaml_control__defined

XAML_CLASS(xaml_multicontainer, { 0xc9a53763, 0x404b, 0x4531, { 0xba, 0x56, 0x56, 0x01, 0xc9, 0x49, 0x05, 0x8d } })

#define XAML_MULTICONTAINER_VTBL(type)                                        \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                               \
    XAML_METHOD(get_children, type, XAML_VECTOR_VIEW_T_NAME(xaml_control)**); \
    XAML_CPROP(child, type, xaml_control*, xaml_control*)

XAML_DECL_INTERFACE_(xaml_multicontainer, xaml_control)
{
    XAML_DECL_VTBL(xaml_multicontainer, XAML_MULTICONTAINER_VTBL);
};

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_multicontainer_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_multicontainer_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTAINER_H
