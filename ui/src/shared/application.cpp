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
        s_current->init_components();
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

    application::application(LPTSTR lpCmdLine)
    {
        int argc;
#ifdef UNICODE
        unique_ptr<LPWSTR[], local_free_deleter> argv{ CommandLineToArgvW(lpCmdLine, &argc) };
#else
        wstring cl = __mbtow(lpCmdLine);
        unique_ptr<LPWSTR[], local_free_deleter> argv{ CommandLineToArgvW(cl.c_str(), &argc) };
#endif // UNICODE
        if (argv)
        {
            for (int i = 0; i < argc; i++)
            {
#ifdef UNICODE
                m_cmd_lines.push_back(argv[i]);
#else
                m_cmd_lines.push_back(__wtomb(argv[i]));
#endif // UNICODE
            }
        }
    }

    shared_ptr<application> application::init(LPTSTR lpCmdLine)
    {
        s_current = shared_ptr<application>(new application(lpCmdLine));
        s_current->init_components();
        return s_current;
    }
#endif // WIN32 || __MINGW32__
} // namespace xaml
