#define DECLSPEC_IMPORT __declspec(dllexport)

#include <roapi.h>
#include <xaml/ui/win/winrt.h>

typedef HRESULT(WINAPI* pfRoActivateInstance)(HSTRING, IInspectable**);
static pfRoActivateInstance pRoActivateInstance = NULL;

typedef HRESULT(WINAPI* pfRoGetActivationFactory)(HSTRING, REFIID, void**);
static pfRoGetActivationFactory pRoGetActivationFactory = NULL;

typedef HRESULT(WINAPI* pfWindowsCreateStringReference)(PCWSTR, UINT32, HSTRING_HEADER*, HSTRING*);
static pfWindowsCreateStringReference pWindowsCreateStringReference = NULL;

typedef HRESULT(WINAPI* pfWindowsDeleteString)(HSTRING);
static pfWindowsDeleteString pWindowsDeleteString = NULL;

typedef PCWSTR(WINAPI* pfWindowsGetStringRawBuffer)(HSTRING, UINT32*);
static pfWindowsGetStringRawBuffer pWindowsGetStringRawBuffer = NULL;

static HMODULE winrt = NULL;
static HMODULE winrt_string = NULL;

bool WINAPI XamlInitializeWinRTFunc(void)
{
    if (!winrt)
    {
        winrt = LoadLibraryEx(L"api-ms-win-core-winrt-l1-1-0.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!winrt) return false;
        pRoActivateInstance = (pfRoActivateInstance)GetProcAddress(winrt, "RoActivateInstance");
        pRoGetActivationFactory = (pfRoGetActivationFactory)GetProcAddress(winrt, "RoGetActivationFactory");
    }
    if (!winrt_string)
    {
        winrt_string = LoadLibraryEx(L"api-ms-win-core-winrt-string-l1-1-0.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!winrt_string) return false;
        pWindowsCreateStringReference = (pfWindowsCreateStringReference)GetProcAddress(winrt_string, "WindowsCreateStringReference");
        pWindowsDeleteString = (pfWindowsDeleteString)GetProcAddress(winrt_string, "WindowsDeleteString");
        pWindowsGetStringRawBuffer = (pfWindowsGetStringRawBuffer)GetProcAddress(winrt_string, "WindowsGetStringRawBuffer");
    }
    return true;
}

ROAPI _Check_return_ HRESULT WINAPI RoActivateInstance(_In_ HSTRING activatableClassId, _COM_Outptr_ IInspectable** instance)
{
    if (pRoActivateInstance)
        return pRoActivateInstance(activatableClassId, instance);
    else
        return E_NOTIMPL;
}

ROAPI _Check_return_ HRESULT WINAPI RoGetActivationFactory(_In_ HSTRING activatableClassId, _In_ REFIID iid, _COM_Outptr_ void** factory)
{
    if (pRoGetActivationFactory)
        return pRoGetActivationFactory(activatableClassId, iid, factory);
    else
        return E_NOTIMPL;
}

DECLSPEC_IMPORT STDAPI WindowsCreateStringReference(_In_reads_opt_(length + 1) PCWSTR sourceString, UINT32 length, _Out_ HSTRING_HEADER* hstringHeader, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING* string)
{
    if (pWindowsCreateStringReference)
        return pWindowsCreateStringReference(sourceString, length, hstringHeader, string);
    else
        return E_NOTIMPL;
}

DECLSPEC_IMPORT STDAPI WindowsDeleteString(_In_opt_ HSTRING string)
{
    if (pWindowsDeleteString)
        return pWindowsDeleteString(string);
    else
        return E_NOTIMPL;
}

DECLSPEC_IMPORT STDAPI_(PCWSTR) WindowsGetStringRawBuffer(_In_opt_ HSTRING string, _Out_opt_ UINT32* length)
{
    if (pWindowsGetStringRawBuffer)
        return pWindowsGetStringRawBuffer(string, length);
    else
        return NULL;
}
