#ifndef XAML_TEST_C_HELPER_H
#define XAML_TEST_C_HELPER_H

#include <xaml/map.h>
#include <xaml/observable_vector.h>
#include <xaml/string.h>

XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_string))
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_string))
XAML_VECTOR_CHANGED_ARGS_1_TYPE(XAML_T_O(xaml_string))
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_O(xaml_vector_changed_args_1__xaml_string))
XAML_OBSERVABLE_VECTOR_1_TYPE(XAML_T_O(xaml_string))
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_V(int32_t), XAML_T_O(xaml_object))
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__int32_t__xaml_object))
XAML_HASHER_1_TYPE(XAML_T_V(int32_t))
XAML_MAP_2_TYPE(XAML_T_V(int32_t), XAML_T_O(xaml_object))

EXTERN_C xaml_result XAML_CALL xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_string_new(
    xaml_result (*p)(xaml_object*, xaml_vector_changed_args_1__xaml_string*) XAML_NOEXCEPT,
    xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_string**) XAML_NOEXCEPT;

EXTERN_C xaml_result XAML_CALL xaml_observable_vector_1__xaml_string_new(xaml_observable_vector_1__xaml_string** ptr) XAML_NOEXCEPT;

EXTERN_C xaml_result XAML_CALL xaml_map_2__int32_t__xaml_object_new(xaml_map_2__int32_t__xaml_object** ptr) XAML_NOEXCEPT;

#endif // !XAML_TEST_C_HELPER_H
