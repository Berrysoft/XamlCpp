#include <xaml/ui/application.hpp>

#ifdef XAML_WIN32
#include <Windows.h>
#include <cstdlib>
#include <tchar.h>
#endif // XAML_WIN32

using namespace std;

namespace xaml
{
    static shared_ptr<application> s_current;

    shared_ptr<application> application::init(int argc, char_t const* const* argv)
    {
        return s_current = shared_ptr<application>(new application(argc, argv));
    }

    shared_ptr<application> application::init()
    {
#if defined(WIN32) || defined(__MINGW32__)
        return init(__argc, __targv);
#else
        return init(0, nullptr);
#endif // WIN32 || __MINGW32__
    }

    shared_ptr<application> application::current() { return s_current; }

    application::~application() {}
} // namespace xaml
