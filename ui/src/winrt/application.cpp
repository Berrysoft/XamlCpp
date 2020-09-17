#include <shared/application.hpp>
#include <xaml/ui/application.h>

#include <wrl.h>

#include <Windows.UI.Xaml.h>

#include <Windows.h>
#include <windowsx.h>

using namespace std;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel::Activation;
using namespace ABI::Windows::UI::Xaml;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

struct XamlApplication : RuntimeClass<IApplicationOverrides>
{
    xaml_application_impl* m_impl;

    ComPtr<IApplicationOverrides> m_base;
    ComPtr<IApplication> m_instance;

    STDMETHODIMP RuntimeClassInitialize(xaml_application_impl* impl)
    {
        m_impl = impl;
        ComPtr<IApplicationFactory> app_factory;
        XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Application).Get(), &app_factory));
        ComPtr<IInspectable> inner;
        XAML_RETURN_IF_FAILED(app_factory->CreateInstance(this, &inner, &m_instance));
        return inner.As(&m_base);
    }

    STDMETHODIMP OnActivated(IActivatedEventArgs* args) { return m_base->OnActivated(args); }
    STDMETHODIMP OnFileActivated(IFileActivatedEventArgs* args) { return m_base->OnFileActivated(args); }
    STDMETHODIMP OnSearchActivated(ISearchActivatedEventArgs* args) { return m_base->OnSearchActivated(args); }
    STDMETHODIMP OnShareTargetActivated(IShareTargetActivatedEventArgs* args) { return m_base->OnShareTargetActivated(args); }
    STDMETHODIMP OnFileOpenPickerActivated(IFileOpenPickerActivatedEventArgs* args) { return m_base->OnFileOpenPickerActivated(args); }
    STDMETHODIMP OnFileSavePickerActivated(IFileSavePickerActivatedEventArgs* args) { return m_base->OnFileSavePickerActivated(args); }
    STDMETHODIMP OnCachedFileUpdaterActivated(ICachedFileUpdaterActivatedEventArgs* args) { return m_base->OnCachedFileUpdaterActivated(args); }
    STDMETHODIMP OnWindowCreated(IWindowCreatedEventArgs* args) { return m_base->OnWindowCreated(args); }

    STDMETHODIMP OnLaunched(ILaunchActivatedEventArgs*)
    {
        int code;
        return xaml_main(m_impl, &code);
    }
};

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    for (int i = 0; i < argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new(argv[i], &arg));
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(arg));
    }
    ComPtr<XamlApplication> app;
    XAML_RETURN_IF_FAILED(MakeAndInitialize<XamlApplication>(&app, this));
    m_native_app = app->m_instance;
#ifndef NDEBUG
    EventRegistrationToken token;
    XAML_RETURN_IF_FAILED(m_native_app->add_UnhandledException(
        Callback<IUnhandledExceptionEventHandler>(
            [this](IInspectable*, IUnhandledExceptionEventArgs* e) {
                HString message;
                XAML_RETURN_IF_FAILED(e->get_Message(message.GetAddressOf()));
                OutputDebugStringW(message.GetRawBuffer(nullptr));
                __debugbreak();
                return S_OK;
            })
            .Get(),
        &token));
#endif // !NDEBUG
    return XAML_S_OK;
}

xaml_result xaml_application_impl::run(int* pvalue) noexcept
{
    *pvalue = m_quit_value;
    return XAML_S_OK;
}

xaml_result xaml_application_impl::quit(int value) noexcept
{
    m_quit_value = value;
    return m_native_app->Exit();
}

xaml_result xaml_application_impl::get_theme(xaml_application_theme* ptheme) noexcept
{
    ComPtr<IApplicationStatics> app_statics;
    XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Application).Get(), &app_statics));
    ComPtr<IApplication> app;
    XAML_RETURN_IF_FAILED(app_statics->get_Current(&app));
    ApplicationTheme theme;
    XAML_RETURN_IF_FAILED(app->get_RequestedTheme(&theme));
    *ptheme = theme == ApplicationTheme_Dark ? xaml_application_theme_dark : xaml_application_theme_light;
    return XAML_S_OK;
}
