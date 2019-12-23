#include <gtk/gtk.h>
#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    static shared_ptr<application> s_current;

    shared_ptr<application> application::init(int argc, char** argv)
    {
        s_current = make_shared<application>(argc, argv);
        s_current->init_components();
        return s_current;
    }

    shared_ptr<application> application::current() { return s_current; }

    void application::init_components()
    {
        gtk_init(0, nullptr);
    }

    int application::run()
    {
        gtk_main();
        return 0;
    }
} // namespace xaml
