#include <cstdlib>
#include <xaml/strings.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#endif // WIN32 || __MINGW32__

using namespace std;

namespace xaml
{
#if defined(WIN32) || defined(__MINGW32__)
    string to_string(wstring_view str)
    {
        int count = WideCharToMultiByte(CP_UTF8, MB_ERR_INVALID_CHARS, str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);
        if (!count) return {};
        string result(count, '\0');
        WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), count, nullptr, nullptr);
        return result;
    }

    wstring to_wstring(string_view str)
    {
        int count = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.data(), (int)str.size(), nullptr, 0);
        if (!count) return {};
        wstring result(count, L'\0');
        MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str.data(), (int)str.size(), result.data(), count);
        return result;
    }
#else
    string to_string(wstring_view str)
    {
        size_t count = wcstombs(nullptr, str.data(), str.size());
        if (count <= 0) return {};
        string result(count, '\0');
        wcstombs(result.data(), str.data(), str.size());
        return result;
    }

    wstring to_wstring(string_view str)
    {
        size_t count = mbstowcs(nullptr, str.data(), str.size());
        if (count <= 0) return {};
        wstring result(count, L'\0');
        mbstowcs(result.data(), str.data(), str.size());
        return result;
    }
#endif // WIN32 || __MINGW32__
} // namespace xaml
