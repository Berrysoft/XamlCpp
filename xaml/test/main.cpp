#include <iostream>
#include <xaml/deserialize.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
    init_meta();
    try
    {
        deserializer des("test.xaml");
        if (des)
        {
            auto wnd = static_pointer_cast<window>(des.deserialize());
#ifdef XAML_UI_WINDOWS
            auto app = application::init(lpCmdLine);
#else
            auto app = application::init(argc, argv);
#endif // XAML_UI_WINDOWS
            wnd->show();
            return app->run();
        }
    }
    catch (exception& ex)
    {
        cerr << ex.what() << endl;
    }
}
