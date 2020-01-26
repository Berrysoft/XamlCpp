#include <filesystem>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>

#ifdef WIN32
#include <system_error>
#include <wil/result_macros.h>

constexpr xaml::string_view_t module_extension{ U(".dll") };
#else
#include <dlfcn.h>
#ifdef __APPLE__
constexpr xaml::string_view_t module_extension{ U(".dylib") };
#else
constexpr xaml::string_view_t module_extension{ U(".so") };
#endif // __APPLE__
#endif // WIN32

constexpr std::string_view module_prefix{ "lib" };

using namespace std;
using namespace std::filesystem;

namespace xaml
{
    static path get_right_path(string_view name)
    {
        path p{ name };
        if (!p.has_extension()) p.replace_extension(module_extension);
        return p;
    }

    static path get_in_lib_path(string_view name)
    {
        path p{ name };
        if (p.is_relative()) p = path{ U("..") } / path(U("lib")) / p;
        return p;
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
                p.replace_filename(path{ U("lib") } += p.filename());
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
                return name;
        }
    }

#ifdef WIN32
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

    void* module::get_method(string_view name)
    {
        FARPROC p = GetProcAddress(get_handle(), name.data());
        return (void*)p;
    }

    void module::close()
    {
        if (get_handle())
        {
            THROW_IF_WIN32_BOOL_FALSE(FreeLibrary(get_handle()));
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

    void* module::get_method(string_view name)
    {
        void* p = dlsym(get_handle(), name.data());
        return p;
    }

    void module::close()
    {
        if (get_handle())
        {
            auto ret = dlclose(get_handle());
            if (ret)
            {
                throw system_error(error_code{}, dlerror());
            }
            set_handle(nullptr);
        }
    }
#endif // WIN32

    void module::register_meta() noexcept
    {
        void (*pinit)(void*) noexcept = (void (*)(void*) noexcept)get_method("init_meta");
        auto context = __get_context();
        pinit(&context);
    }
} // namespace xaml
