#ifndef XAML_UI_RESULT_HANDLER_H
#define XAML_UI_RESULT_HANDLER_H

#include <xaml/delegate.h>
#include <xaml/result.h>

EXTERN_C XAML_API void XAML_CALL xaml_result_handler_default(xaml_result, xaml_result_raise_level, xaml_char_t const*) XAML_NOEXCEPT;
EXTERN_C XAML_API void XAML_CALL xaml_result_handler_empty(xaml_result, xaml_result_raise_level, xaml_char_t const*) XAML_NOEXCEPT;

typedef void(XAML_CALL* xaml_result_handler)(xaml_result, xaml_result_raise_level, xaml_char_t const*) XAML_NOEXCEPT;

EXTERN_C XAML_API xaml_result XAML_CALL xaml_result_handler_set(xaml_result_handler) XAML_NOEXCEPT;

#ifdef __cplusplus
using __xaml_result_handler_prototype = void(xaml_result, xaml_result_raise_level, xaml_char_t const*);
XAML_API xaml_result XAML_CALL xaml_result_handler_set(std::function<__xaml_result_handler_prototype> const&) XAML_NOEXCEPT;
#endif // __cplusplus

#endif // !XAML_UI_RESULT_HANDLER_H
