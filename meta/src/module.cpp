#include <filesystem>
#include <string>
#include <vector>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <system_error>
#else
#include <dlfcn.h>
#endif // WIN32 || __MINGW32__

#if defined(WIN32) && !defined(__MINGW32__)
constexpr xaml::path_string_view_t module_prefix{};
#else
constexpr xaml::path_string_view_t module_prefix{ P("lib") };
#endif // WIN32 && !__MINGW32__

using namespace std;
using namespace std::filesystem;

namespace xaml
{
    static vector<path_string_t> split(path_string_view_t str, path_char_t separater)
    {
        if (str.empty()) return {};
        size_t index = 0, offset = 0;
        vector<path_string_t> result;
        for (;;)
        {
            index = str.find_first_of(separater, offset);
            if (index > offset)
            {
                auto sub = str.substr(offset, index - offset);
                if (!sub.empty())
                    result.emplace_back(sub);
            }
            if (index == path_string_view_t::npos) break;
            offset = index + 1;
        }
        return result;
    }

#if defined(WIN32) || defined(__MINGW32__)
    vector<path_string_t> get_module_search_path()
    {
        wstring buffer(32767, U('\0'));
        DWORD count = GetEnvironmentVariableW(L"PATH", buffer.data(), (DWORD)buffer.length());
        buffer.resize(count);
        vector<path_string_t> result = split(buffer, U(';'));
        result.push_back(P("."));
        return result;
    }
#else
    vector<path_string_t> get_module_search_path()
    {
#ifdef __APPLE__
        constexpr path_string_view_t ld_library_path = "DYLD_LIBRARY_PATH";
#else
        constexpr path_string_view_t ld_library_path = "LD_LIBRARY_PATH";
#endif // __APPLE__
        char* ldp = getenv(ld_library_path.data());
        vector<path_string_t> result = split(ldp ? ldp : "", ':');
        result.push_back(P("../lib"));
        return result;
    }
#endif // WIN32 || __MINGW32__

    static path get_full_path(path_string_view_t name, vector<path_string_t> const& sds)
    {
        path pname = name;
        if (pname.is_absolute()) return pname;
        auto search_dirs = get_module_search_path();
        search_dirs.insert(search_dirs.end(), sds.begin(), sds.end());
        for (auto& dir : search_dirs)
        {
            path fullname{ module_prefix };
            fullname += pname;
            fullname += module_extension;
            path p = path{ dir } / fullname;
            if (exists(p)) return p;
        }
        return name;
    }

#if defined(WIN32) || defined(__MINGW32__)
    void module::open(path_string_view_t name)
    {
        close();
        auto p = get_full_path(name, m_search_dir);
        set_handle(LoadLibraryW(p.c_str()));
        if (!get_handle())
        {
            throw system_error(GetLastError(), system_category());
        }
    }

    void* module::get_method(string_view name) const
    {
        FARPROC p = GetProcAddress((HMODULE)get_handle(), name.data());
        return (void*)p;
    }

    void module::close()
    {
        if (get_handle())
        {
            FreeLibrary((HMODULE)get_handle());
            set_handle(nullptr);
        }
    }
#else
    void module::open(path_string_view_t name)
    {
        close();
        auto p = get_full_path(name, m_search_dir);
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
