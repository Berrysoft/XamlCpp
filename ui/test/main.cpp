#include <iostream>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace xaml;

#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
#ifdef XAML_UI_WINDOWS
    auto app = make_shared<application>(lpCmdLine);
#else
    auto app = make_shared<application>(argc, argv);
#endif // XAML_UI_WINDOWS
    app->init();
    auto wnd = make_shared<window>();
    wnd->set_title(U("Test Window"));
    wnd->show();
    wnd->set_location({ 100, 100 });
    wnd->set_size({ 800, 600 });
    return app->run();
}
