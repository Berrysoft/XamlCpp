#ifndef XAML_RESULT_H
#define XAML_RESULT_H

#ifdef __cplusplus
    #include <cassert>
    #include <cstdint>
    #include <stdexcept>
#else
    #include <assert.h>
    #include <stdint.h>
#endif // __cplusplus

#include <xaml/utility.h>

#if defined(XAML_WIN32) && !defined(XAML_MINGW)
typedef long xaml_result;
#else
typedef XAML_STD int32_t xaml_result;
#endif // XAML_WIN32 && !XAML_MINGW

#define XAML_SUCCEEDED(expr) (!(expr))
#define XAML_FAILED(expr) (expr)

typedef enum xaml_result_raise_level
{
    xaml_result_raise_info,
    xaml_result_raise_warning,
    xaml_result_raise_error
} xaml_result_raise_level;

EXTERN_C XAML_API void XAML_CALL xaml_result_raise(xaml_result, xaml_result_raise_level, char const*, XAML_STD int32_t) XAML_NOEXCEPT;
EXTERN_C XAML_API void XAML_CALL xaml_result_raise_message(xaml_result, xaml_result_raise_level, char const*) XAML_NOEXCEPT;

#ifdef NDEBUG
    #define XAML_RAISE(hr, ...) (void)(hr)
#else
    #define XAML_RAISE(...) xaml_result_raise(__VA_ARGS__, U_(__FILE__), __LINE__)
#endif // NDEBUG

#ifndef XAML_RAISE_LEVEL
    #define XAML_RAISE_LEVEL xaml_result_raise_info
#endif // !XAML_RAISE_LEVEL

#define XAML_RETURN_IF_FAILED(expr)           \
    do                                        \
    {                                         \
        xaml_result hr = (expr);              \
        XAML_UNLIKELY if (XAML_FAILED(hr))    \
        {                                     \
            XAML_RAISE(hr, XAML_RAISE_LEVEL); \
            return hr;                        \
        }                                     \
    } while (0)

#define XAML_GOTO_IF_FAILED(expr, label)      \
    do                                        \
    {                                         \
        hr = (expr);                          \
        XAML_UNLIKELY if (XAML_FAILED(hr))    \
        {                                     \
            XAML_RAISE(hr, XAML_RAISE_LEVEL); \
            goto label;                       \
        }                                     \
    } while (0)

#define XAML_ASSERT_SUCCEEDED(expr)                \
    do                                             \
    {                                              \
        XAML_MAYBE_UNUSED xaml_result hr = (expr); \
        (void)hr;                                  \
        assert(XAML_SUCCEEDED(hr));                \
    } while (0)

#define XAML_S_OK 0

#define XAML_E_FAIL 0x80004005
#define XAML_E_INVALIDARG 0x80070057
#define XAML_E_NOINTERFACE 0x80004002
#define XAML_E_NOTIMPL 0x80004001
#define XAML_E_OUTOFMEMORY 0x8007000E
#define XAML_E_KEYNOTFOUND 0x80131577
#define XAML_E_OUTOFBOUNDS 0x80028CA1

#ifdef __cplusplus
XAML_API std::string xaml_result_get_message(xaml_result);

struct xaml_result_error : std::runtime_error
{
private:
    xaml_result m_result;

public:
    xaml_result_error(xaml_result result)
        : runtime_error(xaml_result_get_message(result)), m_result(result) {}

    xaml_result_error(xaml_result_error const& other) noexcept
        : runtime_error(static_cast<std::runtime_error const&>(other)), m_result(other.m_result) {}

    xaml_result_error& operator=(xaml_result_error const& other) noexcept
    {
        std::runtime_error::operator=(static_cast<std::runtime_error const&>(other));
        m_result = other.m_result;
        return *this;
    }

    constexpr xaml_result get_result() const noexcept { return m_result; }
};

    #define XAML_THROW_IF_FAILED(expr)         \
        do                                     \
        {                                      \
            xaml_result hr = (expr);           \
            XAML_UNLIKELY if (XAML_FAILED(hr)) \
            {                                  \
                throw xaml_result_error{ hr }; \
            }                                  \
        } while (0)

    #define XAML_CATCH_RETURN()                                            \
        catch (xaml_result_error const& e) { return e.get_result(); }      \
        catch (std::bad_alloc const&) { return XAML_E_OUTOFMEMORY; }       \
        catch (std::out_of_range const&) { return XAML_E_OUTOFBOUNDS; }    \
        catch (std::invalid_argument const&) { return XAML_E_INVALIDARG; } \
        catch (...) { return XAML_E_FAIL; }
#else
    #define XAML_THROW_IF_FAILED(expr) XAML_ASSERT_SUCCEEDED(expr)
#endif // __cplusplus

#endif // !XAML_RESULT_H
