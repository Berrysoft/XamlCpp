#include <Windows.h>
#include <map>
#include <shared/application.hpp>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/result_win32.h>
#include <xaml/ui/application.h>
#include <xaml/ui/win/control.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/win/window.h>

using namespace std;

static LOGFONT s_default_font;
static map<UINT, wil::unique_hfont> s_dpi_fonts;

static xaml_result take_over_message(BOOL* pres) noexcept
{
    MSG msg;
    BOOL bRet = GetMessage(&msg, nullptr, 0, 0);
    if (bRet < 0)
        return HRESULT_FROM_WIN32(GetLastError());
    else if (bRet > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    *pres = bRet;
    return XAML_S_OK;
}

static BOOL register_window_class() noexcept
{
    WNDCLASSEX cls = {};
    cls.cbSize = sizeof(WNDCLASSEX);
    cls.lpfnWndProc = xaml_window_callback;
    cls.cbClsExtra = 0;
    cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
    cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    cls.hbrBackground = nullptr;
    cls.lpszClassName = U("XamlWindow");
    cls.hInstance = GetModuleHandle(NULL);
    return RegisterClassEx(&cls);
}

xaml_result xaml_application_impl::init(int argc, xaml_char_t** argv) noexcept
{
    m_font_provider.m_outer = this;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    for (int i = 0; i < argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new_view(argv[i], &arg));
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(arg));
    }
    XamlInitializeDpiFunc();
    XAML_RETURN_IF_WIN32_BOOL_FALSE(XamlSetProcessBestDpiAwareness());
    XamlInitializeDarkModeFunc();
    XamlSetPreferredAppMode(XAML_PREFERRED_APP_MODE_ALLOW_DARK);
    XAML_RETURN_IF_WIN32_BOOL_FALSE(XamlSystemDefaultFontForDpi(&s_default_font, USER_DEFAULT_SCREEN_DPI));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(register_window_class());
    return XAML_S_OK;
}

xaml_result xaml_application_impl::run(int* pvalue) noexcept
{
    while (true)
    {
        BOOL res;
        XAML_RETURN_IF_FAILED(take_over_message(&res));
        if (!res) break;
        if (!m_main_wnd) PostQuitMessage(m_quit_value);
    }
    *pvalue = m_quit_value;
    return XAML_S_OK;
}

xaml_result xaml_application_impl::quit(int value) noexcept
{
    m_quit_value = value;
    if (m_main_wnd)
    {
        PostQuitMessage(value);
    }
    return XAML_S_OK;
}

xaml_result xaml_application_impl::get_theme(xaml_application_theme* ptheme) noexcept
{
    *ptheme = XamlIsDarkModeAllowedForApp() ? xaml_application_theme_dark : xaml_application_theme_light;
    return XAML_S_OK;
}

xaml_result xaml_application_impl::get_default_font(UINT udpi, HFONT* pfont) noexcept
{
    auto it = s_dpi_fonts.find(udpi);
    if (it != s_dpi_fonts.end())
    {
        *pfont = it->second.get();
    }
    else
    {
        LOGFONT f = s_default_font;
        f.lfHeight = MulDiv(f.lfHeight, (int)udpi, USER_DEFAULT_SCREEN_DPI);
        f.lfWidth = MulDiv(f.lfWidth, (int)udpi, USER_DEFAULT_SCREEN_DPI);
        auto p = s_dpi_fonts.emplace(udpi, CreateFontIndirect(&f));
        *pfont = p.second ? p.first->second.get() : NULL;
    }
    return XAML_S_OK;
}
