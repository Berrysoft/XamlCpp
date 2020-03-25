#include <cstdlib>
#include <xaml/strings.hpp>

using namespace std;

namespace xaml
{
#ifdef _MSC_VER
    string __wtomb(wstring_view str)
    {
        size_t count{ 0 };
        errno_t err = wcstombs_s(&count, nullptr, 0, str.data(), str.size());
        if (err || count <= 1) return {};
        string result(count - 1, '\0');
        wcstombs_s(nullptr, result.data(), _TRUNCATE, str.data(), str.size());
        return result;
    }

    wstring __mbtow(string_view str)
    {
        size_t count{ 0 };
        errno_t err = mbstowcs_s(&count, nullptr, 0, str.data(), str.size());
        if (err || count <= 1) return {};
        wstring result(count - 1, L'\0');
        mbstowcs_s(nullptr, result.data(), _TRUNCATE, str.data(), str.size());
        return result;
    }
#else
    string __wtomb(wstring_view str)
    {
        size_t count = wcstombs(nullptr, str.data(), str.size());
        if (count <= 0) return {};
        string result(count, '\0');
        wcstombs(result.data(), str.data(), str.size());
        return result;
    }

    wstring __mbtow(string_view str)
    {
        size_t count = mbstowcs(nullptr, str.data(), str.size());
        if (count <= 0) return {};
        wstring result(count, L'\0');
        mbstowcs(result.data(), str.data(), str.size());
        return result;
    }
#endif // _MSC_VER
} // namespace xaml
