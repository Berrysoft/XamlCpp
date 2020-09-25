#ifndef XAML_TEST_C_HELPER_H
#define XAML_TEST_C_HELPER_H

#include <xaml/observable_vector.h>
#include <xaml/string.h>

XAML_ENUMERATOR_T_TYPE(XAML_T_O(xaml_string))
XAML_ENUMERABLE_T_TYPE(XAML_T_O(xaml_string))
XAML_VECTOR_VIEW_T_TYPE(XAML_T_O(xaml_string))
XAML_VECTOR_CHANGED_ARGS_T_TYPE(XAML_T_O(xaml_string))
XAML_DELEGATE_T_T_TYPE(XAML_T_O(xaml_object), XAML_T_O(xaml_vector_changed_args__xaml_string__))
XAML_VECTOR_T_TYPE(XAML_T_O(xaml_string))
XAML_OBSERVABLE_VECTOR_T_TYPE(XAML_T_O(xaml_string))

EXTERN_C xaml_result XAML_CALL xaml_delegate__xaml_object____xaml_vector_changed_args__xaml_string_____new(
    xaml_result (*p)(xaml_object*, xaml_vector_changed_args__xaml_string__*),
    xaml_delegate__xaml_object____xaml_vector_changed_args__xaml_string____**) XAML_NOEXCEPT;

EXTERN_C xaml_result XAML_CALL xaml_observable_vector__xaml_string___new(xaml_observable_vector__xaml_string__** ptr) XAML_NOEXCEPT;

#endif // !XAML_TEST_C_HELPER_H
