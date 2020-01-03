#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    shared_ptr<application> s_current;

    XAML_API shared_ptr<application> application::init(int argc, char_t** argv)
    {
        s_current = shared_ptr<application>(new application(argc, argv));
        s_current->init_components();
        return s_current;
    }

    XAML_API shared_ptr<application> application::current() { return s_current; }
} // namespace xaml