#ifndef XAML_UI_CONTROLS_ITEMS_BASE_H
#define XAML_UI_CONTROLS_ITEMS_BASE_H

#include <xaml/markup/template_base.h>
#include <xaml/observable_vector.h>
#include <xaml/ui/control.h>

#ifndef xaml_enumerator_1__xaml_object_defined
    #define xaml_enumerator_1__xaml_object_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_enumerator_1__xaml_object_defined

#ifndef xaml_vector_changed_args_1__xaml_object_defined
    #define xaml_vector_changed_args_1__xaml_object_defined
XAML_VECTOR_CHANGED_ARGS_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_vector_changed_args_1__xaml_object_defined

#ifndef xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_object_defined
    #define xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_object_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_O(xaml_vector_changed_args_1__xaml_object))
#endif // !xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_object_defined

#ifndef xaml_observable_vector_1__xaml_object_defined
    #define xaml_observable_vector_1__xaml_object_defined
XAML_OBSERVABLE_VECTOR_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_observable_vector_1__xaml_object_defined

XAML_CLASS(xaml_items_base, { 0xfad67a33, 0x4a5f, 0x4915, { 0xa1, 0x77, 0xe6, 0x1e, 0x89, 0xc7, 0x5e, 0x26 } })

#define XAML_ITEMS_BASE_VTBL(type)                                                                                     \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                                                                        \
    XAML_PROP(items, type, XAML_OBSERVABLE_VECTOR_1_NAME(xaml_object)**, XAML_OBSERVABLE_VECTOR_1_NAME(xaml_object)*); \
    XAML_EVENT(items_changed, type, xaml_object, XAML_OBSERVABLE_VECTOR_1_NAME(xaml_object));                          \
    XAML_PROP(items_template, type, xaml_template_base**, xaml_template_base*);                                        \
    XAML_PROP(sel_id, type, XAML_STD int32_t*, XAML_STD int32_t);                                                      \
    XAML_EVENT(sel_id_changed, type, xaml_object, int32_t)

XAML_DECL_INTERFACE_(xaml_items_base, xaml_control)
{
    XAML_DECL_VTBL(xaml_items_base, XAML_ITEMS_BASE_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_items_base_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_items_base_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_ITEMS_BASE_H
