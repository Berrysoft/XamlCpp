#include <xaml/meta/binding.hpp>
#include <xaml/ui/application.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <shellapi.h>
#endif // WIN32 || __MINGW32__

using namespace std;

namespace xaml
{
    static shared_ptr<application> s_current;

    shared_ptr<application> application::init(int argc, char_t** argv)
    {
        s_current = shared_ptr<application>(new application(argc, argv));
        return s_current;
    }

    shared_ptr<application> application::current() { return s_current; }

    void application::add_module(string_view path)
    {
        auto m = make_shared<module>(path);
        m->register_meta();
        m_modules.emplace(path, m);
    }

    application::~application()
    {
        unbind_all();
        cleanup_context();
    }

#if defined(WIN32) || defined(__MINGW32__)
    struct local_free_deleter
    {
        void operator()(void* pointer) const noexcept { LocalFree((HLOCAL)pointer); }
    };

    shared_ptr<application> application::init(LPTSTR lpCmdLine)
    {
        int argc;
#ifdef UNICODE
        unique_ptr<LPWSTR[], local_free_deleter> argv{ CommandLineToArgvW(lpCmdLine, &argc) };
        s_current = shared_ptr<application>(new application(argc, const_cast<char_t**>(argv.get())));
#else
        unique_ptr<LPWSTR[], local_free_deleter> args{ CommandLineToArgvW(__mbtow(lpCmdLine).c_str(), &argc) };
        vector<string_t> argv_storage;
        vector<char_t const*> argv;
        for (int i = 0; i < argc; i++)
        {
            argv_storage.push_back(__wtomb(args[i]));
            argv.push_back(argv_storage.back().c_str());
        }
        s_current = shared_ptr<application>(new application(argc, const_cast<char_t**>(argv.data())));
#endif // UNICODE
        return s_current;
    }
#endif // WIN32 || __MINGW32__
} // namespace xaml
