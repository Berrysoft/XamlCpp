#include <Windows.h>
#include <xaml/ui/win/d2d.h>

typedef HRESULT(WINAPI* pfD2D1CreateFactory)(D2D1_FACTORY_TYPE, REFIID, CONST D2D1_FACTORY_OPTIONS*, void**);
static pfD2D1CreateFactory pD2D1CreateFactory = NULL;

typedef HRESULT(WINAPI* pfDWriteCreateFactory)(DWRITE_FACTORY_TYPE, REFIID, IUnknown**);
static pfDWriteCreateFactory pDWriteCreateFactory = NULL;

static HMODULE d2d1 = NULL;
static HMODULE dwrite = NULL;

void WINAPI XamlInitializeD2DFunc()
{
    if (!d2d1)
    {
        d2d1 = LoadLibraryEx(L"d2d1.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (d2d1)
        {
            pD2D1CreateFactory = (pfD2D1CreateFactory)GetProcAddress(d2d1, "D2D1CreateFactory");
        }
    }
    if (!dwrite)
    {
        dwrite = LoadLibraryEx(L"DWrite.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (dwrite)
        {
            pDWriteCreateFactory = (pfDWriteCreateFactory)GetProcAddress(dwrite, "DWriteCreateFactory");
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
