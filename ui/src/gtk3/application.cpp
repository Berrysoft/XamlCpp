#include <gtk/gtk.h>
#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    void application::init_components()
    {
        gtk_init(0, nullptr);
    }

    int application::run()
    {
        gtk_main();
        return 0;
    }

    application::~application()
    {
        cleanup_modules();
    }
} // namespace xaml
