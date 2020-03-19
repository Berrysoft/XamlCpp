#include <test_window.hpp>
#include <xaml/ui/application.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <tchar.h>
#endif // WIN32 || __MINGW32__

using namespace std;
using namespace xaml;
using namespace xaml::test;

#if defined(WIN32) || defined(__MINGW32__)
INT APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR lpCmdLine, INT)
#else
int main(int argc, char const* const* argv)
#endif // WIN32 || __MINGW32__
{
#if defined(WIN32) || defined(__MINGW32__)
    auto app = application::init();
#else
    auto app = application::init(argc, argv);
#endif // WIN32 || __MINGW32__
    auto wnd = make_shared<test_window>();
    wnd->init_components();
    wnd->show();
    return app->run();
}
