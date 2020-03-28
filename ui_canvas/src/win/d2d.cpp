#include <Windows.h>
#include <wil/resource.h>
#include <xaml/ui/win/d2d.h>

using namespace std;

using pfD2D1CreateFactory = HRESULT(WINAPI*)(D2D1_FACTORY_TYPE, REFIID, CONST D2D1_FACTORY_OPTIONS*, void**);
static pfD2D1CreateFactory pD2D1CreateFactory = nullptr;

using pfDWriteCreateFactory = HRESULT(WINAPI*)(DWRITE_FACTORY_TYPE, REFIID, IUnknown**);
static pfDWriteCreateFactory pDWriteCreateFactory = nullptr;

static wil::unique_hmodule d2d1 = nullptr;
static wil::unique_hmodule dwrite = nullptr;

void WINAPI XamlInitializeD2DFunc()
{
    if (!d2d1)
    {
        d2d1.reset(LoadLibraryEx(L"d2d1.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32));
        if (d2d1)
        {
            pD2D1CreateFactory = (pfD2D1CreateFactory)GetProcAddress(d2d1.get(), "D2D1CreateFactory");
        }
    }
    if (!dwrite)
    {
        dwrite.reset(LoadLibraryEx(L"DWrite.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32));
        if (dwrite)
        {
            pDWriteCreateFactory = (pfDWriteCreateFactory)GetProcAddress(dwrite.get(), "DWriteCreateFactory");
        }
    }
}

HRESULT WINAPI XamlD2D1CreateFactory(D2D1_FACTORY_TYPE factoryType, REFIID riid, CONST D2D1_FACTORY_OPTIONS* pFactoryOptions, void** ppIFactory)
{
    if (pD2D1CreateFactory)
    {
        return pD2D1CreateFactory(factoryType, riid, pFactoryOptions, ppIFactory);
    }
    else
    {
        return HRESULT_FROM_WIN32(ERROR_DLL_NOT_FOUND);
    }
}

HRESULT WINAPI XamlDWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory)
{
    if (pDWriteCreateFactory)
    {
        return pDWriteCreateFactory(factoryType, iid, factory);
    }
    else
    {
        return HRESULT_FROM_WIN32(ERROR_DLL_NOT_FOUND);
    }
}
