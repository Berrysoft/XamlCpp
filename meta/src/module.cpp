#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <filesystem>
#include <system_error>
#else
#include <dlfcn.h>

#ifdef __APPLE__
#include <boost/filesystem.hpp>
#else
#include <filesystem>
#endif // __APPLE__
#endif // WIN32 || __MINGW32__

constexpr std::string_view module_prefix{ "lib" };

using namespace std;

#ifndef __APPLE__
using namespace std::filesystem;
#else
using namespace boost::filesystem;
#endif // !__APPLE__

namespace xaml
{
    static inline path get_path(string_view p)
    {
        return path{ p.begin(), p.end() };
    }

    static inline path get_path(wstring_view p)
    {
        return path{ p.begin(), p.end() };
    }

    static path get_right_path(string_view name)
    {
        path p = get_path(name);
        if (!p.has_extension()) p.replace_extension(get_path(module_extension));
        return p;
    }

    static path get_in_lib_path(string_view name)
    {
        path p{ name.begin(), name.end() };
        if (p.has_parent_path())
        {
            if (p.has_filename())
            {
                return p.parent_path() / path{ U("..") } / path{ U("lib") } / p.filename();
            }
            else
            {
                return p;
            }
        }
        else
        {
            return path{ U("..") } / path{ U("lib") } / p;
        }
    }

    static path get_full_path(string_view name, bool in_lib = false)
    {
        path p = get_right_path(name);
        if (exists(p))
        {
            return p;
        }
        else
        {
            if (name.length() >= module_prefix.length() && module_prefix == name.substr(0, 3))
            {
                p = get_right_path(name.substr(4));
            }
            else
            {
                auto filename = path{ U("lib") };
                filename += p.filename();
                p.remove_filename();
                p /= filename;
                string nname = p.string();
                p = get_right_path(nname);
            }
            if (exists(p))
                return p;
            else if (!in_lib)
            {
                p = get_in_lib_path(name);
                return get_full_path(p.string(), true);
            }
            else
                return get_path(name);
        }
    }

#if defined(WIN32) || defined(__MINGW32__)
    void module::open(string_view name)
    {
        close();
        auto p = get_full_path(name);
        set_handle(LoadLibraryW(p.c_str()));
        if (!get_handle())
        {
            throw system_error(GetLastError(), system_category());
        }
    }

    void* module::get_method(string_view name) const
    {
        FARPROC p = GetProcAddress(get_handle(), name.data());
        return (void*)p;
    }

    void module::close()
    {
        if (get_handle())
        {
            FreeLibrary(get_handle());
            set_handle(nullptr);
        }
    }
#else
    void module::open(string_view name)
    {
        close();
        auto p = get_full_path(name);
        set_handle(dlopen(p.c_str(), RTLD_LAZY));
        if (!get_handle())
        {
            throw system_error(error_code{}, dlerror());
        }
    }

    void* module::get_method(string_view name) const
    {
        void* p = dlsym(get_handle(), name.data());
        return p;
    }

    void module::close()
    {
        if (get_handle())
        {
            dlclose(get_handle());
            set_handle(nullptr);
        }
    }
#endif // WIN32 || __MINGW32__
} // namespace xaml
