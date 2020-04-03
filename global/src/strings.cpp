#include <codecvt>
#include <locale>
#include <xaml/strings.hpp>

using namespace std;

namespace xaml
{
#if defined(WIN32) || defined(__MINGW32__)
    using codecvt_utf8_wchar = codecvt_utf8_utf16<wchar_t>;
#else
    using codecvt_utf8_wchar = codecvt_utf8<wchar_t>;
#endif // WIN32 || __MINGW32__

    string to_string(wstring_view str)
    {
        wstring_convert<codecvt_utf8_wchar, wchar_t> wconv;
        return wconv.to_bytes(str.data(), str.data() + str.size());
    }

    string to_string(u16string_view str)
    {
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> wconv;
        return wconv.to_bytes(str.data(), str.data() + str.size());
    }

    wstring to_wstring(string_view str)
    {
        wstring_convert<codecvt_utf8_wchar, wchar_t> wconv;
        return wconv.from_bytes(str.data(), str.data() + str.size());
    }

    u16string to_u16string(string_view str)
    {
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> wconv;
        return wconv.from_bytes(str.data(), str.data() + str.size());
    }
} // namespace xaml
