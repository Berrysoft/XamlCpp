#include <gtk/gtk.h>
#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    application::application(int argc, char_t** argv)
    {
        gtk_init(&argc, &argv);
        m_cmd_lines.assign(argv, argv + argc);
    }

    int application::run()
    {
        gtk_main();
        return 0;
    }
} // namespace xaml
