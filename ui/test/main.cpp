#include <iostream>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace xaml;

#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, PWSTR, INT)
#else
int main()
#endif // XAML_UI_WINDOWS
{
    auto app = make_shared<application>();
    app->init();
    auto wnd = make_shared<window>();
    wnd->set_text(U("Test Window"));
    wnd->show();
    wnd->set_size({ 400, 300 });
    return app->run();
}
