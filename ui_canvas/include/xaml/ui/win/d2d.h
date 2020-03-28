#ifndef XAML_UI_CANVAS_WIN_D2D_H
#define XAML_UI_CANVAS_WIN_D2D_H

#include <d2d1.h>
#include <dwrite.h>
#include <xaml/utility.hpp>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    XAML_UI_CANVAS_API void WINAPI XamlInitializeD2DFunc();

    XAML_UI_CANVAS_API HRESULT WINAPI XamlD2D1CreateFactory(D2D1_FACTORY_TYPE factoryType, REFIID riid, CONST D2D1_FACTORY_OPTIONS* pFactoryOptions, void** ppIFactory);

    XAML_UI_CANVAS_API HRESULT WINAPI XamlDWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory);

#ifdef __cplusplus
}

inline HRESULT XamlD2D1CreateFactory(D2D1_FACTORY_TYPE factoryType, REFIID riid, void** factory) noexcept
{
    return XamlD2D1CreateFactory(factoryType, riid, NULL, factory);
}

template <typename Factory>
HRESULT XamlD2D1CreateFactory(D2D1_FACTORY_TYPE factoryType, Factory** factory) noexcept
{
    return XamlD2D1CreateFactory(factoryType, __uuidof(Factory), reinterpret_cast<void**>(factory));
}

template <typename Factory>
HRESULT XamlDWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, Factory** factory) noexcept
{
    return XamlDWriteCreateFactory(factoryType, __uuidof(Factory), reinterpret_cast<IUnknown**>(factory));
}

#endif // __cplusplus

#endif // !XAML_UI_CANVAS_WIN_D2D_H
