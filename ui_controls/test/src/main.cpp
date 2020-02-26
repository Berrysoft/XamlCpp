#include <test_window.hpp>
#include <xaml/ui/application.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <tchar.h>
#endif // WIN32 || __MINGW32__

using namespace std;
using namespace xaml;
using namespace xaml::test;

// Use different main signature for different platforms.
#if defined(WIN32) || defined(__MINGW32__)
INT APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR lpCmdLine, INT)
#else
int main(int argc, char const* const* argv)
#endif // WIN32 || __MINGW32__
{
    // Initialize and get default application object.
#if defined(WIN32) || defined(__MINGW32__)
    auto app = application::init(lpCmdLine);
#else
    auto app = application::init(argc, argv);
#endif // WIN32 || __MINGW32__
    // Construct a window.
    auto wnd = make_shared<test_window>();
    // Initialize the window.
    wnd->init_components();
    // Show the window.
    wnd->show();
    // Run the main loop and wait.
    return app->run();
}
