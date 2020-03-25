#include <locale>
#include <xaml/strings.hpp>

using namespace std;

namespace xaml
{
    string __wtomb(wstring_view str)
    {
        mbstate_t mb{};
        string internal(str.size() * 2, '\0');
        auto& f = use_facet<codecvt<wchar_t, char, mbstate_t>>(locale{});
        const wchar_t* from_next;
        char* to_next;
        auto result = f.out(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        if (result != codecvt_base::ok) return {};
        internal.resize(to_next - internal.data());
        return internal;
    }

    wstring __mbtow(string_view str)
    {
        mbstate_t mb{};
        wstring internal(str.size(), L'\0');
        auto& f = use_facet<codecvt<wchar_t, char, mbstate_t>>(locale{});
        const char* from_next;
        wchar_t* to_next;
        auto result = f.in(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        if (result != codecvt_base::ok) return {};
        internal.resize(to_next - internal.data());
        return internal;
    }
} // namespace xaml
