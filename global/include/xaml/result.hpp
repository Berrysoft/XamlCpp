#ifndef XAML_RESULT_HPP
#define XAML_RESULT_HPP

#include <exception>
#include <xaml/result.h>

struct xaml_result_error : std::exception
{
private:
    xaml_result m_result;

public:
    xaml_result_error(xaml_result result) noexcept : exception() {}
    ~xaml_result_error() override {}

    constexpr xaml_result get_result() const noexcept { return m_result; }
};

#define XAML_THROW_IF_FAILED(expr)         \
    do                                     \
    {                                      \
        xaml_result hr = (expr);           \
        if (XAML_FAILED(hr))               \
            throw xaml_result_error{ hr }; \
    } while (0)

#endif // !XAML_RESULT_HPP
