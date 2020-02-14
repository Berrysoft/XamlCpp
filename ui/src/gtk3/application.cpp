#include <gtk/gtk.h>
#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    void application::init_components()
    {
        vector<char const*> args;
        for (auto& a : m_cmd_lines)
        {
            args.push_back(a.c_str());
        }
        int argc = (int)args.size();
        char** argv = const_cast<char**>(args.data());
        gtk_init(&argc, &argv);
        m_cmd_lines.clear();
        for (int i = 0; i < argc; i++)
        {
            m_cmd_lines.push_back(argv[i]);
        }
    }

    int application::run()
    {
        gtk_main();
        return 0;
    }
} // namespace xaml
