#include <sf/sformat.hpp>
#include <xaml/result.h>
#include <xaml/string.h>

using namespace std;

string xaml_result_get_message(xaml_result result)
{
    if (result >= 0) return U("Ok");
    switch ((uint32_t)result)
    {
    case XAML_E_FAIL:
        return U("Failed");
    case XAML_E_INVALIDARG:
        return U("Invalid arg");
    case XAML_E_NOINTERFACE:
        return U("Invalid cast");
    case XAML_E_NOTIMPL:
        return U("Not implemented");
    case XAML_E_OUTOFMEMORY:
        return U("Out of memory");
    case XAML_E_KEYNOTFOUND:
        return U("Key not found");
    case XAML_E_OUTOFBOUNDS:
        return U("Argument out of range");
    default:
        return sf::sprint(U("{:x8,s}"), result);
    }
}
