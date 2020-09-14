#include <nowide/args.hpp>
#include <nowide/iostream.hpp>
#include <sf/format.hpp>
#include <xaml/ui/application.h>

#ifdef XAML_UI_WINRT
    #include <wrl.h>

    #include <Windows.UI.Xaml.h>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
#endif // XAML_UI_WINRT

using namespace std;

static int xaml_main_end(xaml_result hr, int* pcode) noexcept
{
    if (XAML_SUCCEEDED(hr))
    {
        return *pcode;
    }
    else
    {
        sf::println(nowide::cerr, U("Unhandled exception: {}"), xaml_result_get_message(hr));
        return (int)hr;
    }
}

static xaml_result xaml_main_begin(int argc, char** argv, int* pcode) noexcept
{
#ifdef XAML_UI_WINRT
    ComPtr<IApplicationStatics> app_statics;
    XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Application).Get(), &app_statics));
    return app_statics->Start(
        Callback<IApplicationInitializationCallback>(
            [=](IApplicationInitializationCallbackParams* e) {
                xaml_ptr<xaml_application> app;
                return xaml_application_init_with_args(argc, argv, &app);
            })
            .Get());
#else
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init_with_args(argc, argv, &app));
    return xaml_main(app, pcode);
#endif // XAML_UI_WINRT
}

int main(int argc, char** argv)
{
    nowide::args _(argc, argv);
    int code;
    return xaml_main_end(xaml_main_begin(argc, argv, &code), &code);
}
