#include <test_window.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/controls/meta.hpp>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

using namespace std;
using namespace xaml;
using namespace xaml::test;

// Use different main signature for different platforms.
#ifdef WIN32
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // WIN32
{
    // Initialize and get default application object.
#ifdef XAML_UI_WINDOWS
    auto app = application::init(lpCmdLine);
#elif defined(WIN32)
    auto app = application::init();
#else
    auto app = application::init(argc, argv);
#endif
    app->add_module<ui_controls_init_traits>();
    // Construct a window.
    auto wnd = make_shared<test_window>();
    // Initialize the window.
    wnd->init_components();
    // Show the window.
    wnd->show();
    // Run the main loop and wait.
    return app->run();
}
