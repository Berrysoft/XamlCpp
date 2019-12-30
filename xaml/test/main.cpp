#include <iostream>
#include <xaml/deserialize.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

#ifdef UNICODE
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif // UNICODE
{
    init_meta();
    try
    {
        deserializer des("test.xaml");
        if (des)
        {
            auto wnd = static_pointer_cast<window>(des.deserialize());
            auto app = application::init(argc, argv);
            wnd->show();
            return app->run();
        }
    }
    catch (exception& ex)
    {
        cerr << ex.what() << endl;
    }
}
