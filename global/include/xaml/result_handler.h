#ifndef XAML_UI_RESULT_HANDLER_H
#define XAML_UI_RESULT_HANDLER_H

#include <xaml/delegate.h>
#include <xaml/result.h>

EXTERN_C XAML_API xaml_result XAML_CALL xaml_result_handler_add(void(XAML_CALL*)(xaml_result, xaml_char_t const*) XAML_NOEXCEPT, XAML_STD int32_t*) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_result_handler_remove(XAML_STD int32_t) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_result_handler_add(std::function<void(xaml_result, xaml_char_t const*)> const&, XAML_STD int32_t*) XAML_NOEXCEPT;
#endif // __cplusplus

#endif // !XAML_UI_RESULT_HANDLER_H
