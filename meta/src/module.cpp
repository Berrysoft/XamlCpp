#include <string>
#include <vector>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <system_error>
#else
#include <dlfcn.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif // __APPLE__
#endif // WIN32 || __MINGW32__

#if defined(WIN32) && !defined(__MINGW32__)
static constexpr bool has_prefix = false;
#else
static constexpr bool has_prefix = true;
#endif // WIN32 && !__MINGW32__
static inline std::filesystem::path module_prefix{ "lib" };

using namespace std;
using namespace std::filesystem;

namespace xaml
{
    using path_char_t = typename path::string_type::value_type;
    using path_string_t = typename path::string_type;
    using path_string_view_t = basic_string_view<path_char_t>;

    static vector<path> split(path_string_view_t str, path_char_t separater)
    {
        if (str.empty()) return {};
        size_t index = 0, offset = 0;
        vector<path> result;
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

    static path program_location()
    {
#if defined(WIN32) || defined(__MINGW32__)
        wstring path(1024, L'\0');
        if (GetModuleFileNameW(nullptr, path.data(), (DWORD)path.size()) == 0) return {};
        return path;
#elif defined(__APPLE__)
        string path(1024, '\0');
        uint32_t size = (uint32_t)path.size();
        if (_NSGetExecutablePath(path.data(), &size) == 0)
            return path;
        path.resize((size_t)size);
        if (_NSGetExecutablePath(path.data(), &size) != 0)
            return {};
        return path;
#else
        return read_symlink("/proc/self/exe");
#endif // WIN32 || __MINGW32__
    }

    static vector<path> get_module_search_path()
    {
#if defined(WIN32) || defined(__MINGW32__)
        wstring buffer(32767, L'\0');
        DWORD count = GetEnvironmentVariableW(L"PATH", buffer.data(), (DWORD)buffer.length());
        buffer.resize(count);
        vector<path> result = split(buffer, ';');
#else
#ifdef __APPLE__
        constexpr path_string_view_t ld_library_path = "DYLD_LIBRARY_PATH";
#else
        constexpr path_string_view_t ld_library_path = "LD_LIBRARY_PATH";
#endif // __APPLE__
        char* ldp = getenv(ld_library_path.data());
        vector<path> result = split(ldp ? ldp : path_string_view_t{}, ':');
#endif // WIN32 || __MINGW32__
        result.push_back(".");
        result.push_back("../lib");
        auto location = program_location().parent_path();
        if (!location.empty())
        {
            result.push_back(location);
            result.push_back(location / ".." / "lib");
        }
        return result;
    }

    static path get_full_path(path const& name, vector<path> const& sds)
    {
        if (name.is_absolute()) return name;
        path pname = name;
        if constexpr (has_prefix)
        {
            if (!pname.native().starts_with(module_prefix.native()))
            {
                pname = module_prefix;
                pname += name;
            }
        }
        else
        {
            if (pname.native().starts_with(module_prefix.native()))
            {
                pname = pname.native().substr(3);
            }
        }
        auto search_dirs = get_module_search_path();
        search_dirs.insert(search_dirs.end(), sds.begin(), sds.end());
        for (auto& dir : search_dirs)
        {
            path fullname = pname;
            fullname += module_extension;
            path p = path{ dir } / fullname;
            if (exists(p)) return p;
        }
        return name;
    }

#if defined(WIN32) || defined(__MINGW32__)
    void module::open(path const& name)
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
    void module::open(path const& name)
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
