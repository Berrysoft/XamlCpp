#ifdef UNICODE
#include <boost/nowide/convert.hpp>
#endif // UNICODE

#include <xaml/string.h>

using namespace std;

#ifdef UNICODE
wstring XAML_CALL to_wstring(string_view str)
{
    return boost::nowide::widen(str.data(), str.size());
}

xaml_result XAML_CALL to_wstring(string_view str, wstring* pvalue) noexcept
try
{
    *pvalue = boost::nowide::widen(str.data(), str.size());
    return XAML_S_OK;
}
XAML_CATCH_RETURN()
#endif // UNICODE
