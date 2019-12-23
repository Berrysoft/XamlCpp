#include <iostream>
#include <xaml/deserialize.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

int main()
{
    init_meta();
    try
    {
        deserializer des("test.xaml");
        if (des)
        {
            auto wnd = static_pointer_cast<window>(des.deserialize());
            auto app = make_shared<application>();
            app->init();
            wnd->set_size({ 800, 600 });
            wnd->show();
            app->run();
        }
    }
    catch (exception& ex)
    {
        cerr << ex.what() << endl;
    }
}
