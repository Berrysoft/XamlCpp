#include <filesystem>
#include <string>
#include <vector>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <system_error>

constexpr std::wstring_view module_prefix{};
#else
#include <dlfcn.h>

constexpr std::string_view module_prefix{ "lib" };
#endif // WIN32 || __MINGW32__

using namespace std;
using namespace std::filesystem;

namespace xaml
{
    template <typename Char>
    static vector<basic_string<Char>> split(basic_string_view<Char> str, Char separater)
    {
        size_t index = 0, offset = 0;
        vector<basic_string<Char>> result;
        for (;;)
        {
            index = str.find_first_of(separater, offset);
            if (index > offset)
            {
                auto sub = str.substr(offset, index - offset);
                if (!sub.empty())
                    result.emplace_back(sub);
            }
            if (index == basic_string_view<Char>::npos) break;
            offset = index + 1;
        }
        return result;
    }

#if defined(WIN32) || defined(__MINGW32__)
    static vector<wstring> get_search_path()
    {
        wstring buffer(32767, U('\0'));
        DWORD count = GetEnvironmentVariableW(L"PATH", buffer.data(), (DWORD)buffer.length());
        buffer.resize(count);
        vector<wstring> result = split<wchar_t>(buffer, U(';'));
        result.push_back(L".");
        return result;
    }
#else
    static vector<string> get_search_path()
    {
#ifdef __APPLE__
        constexpr string_view ld_library_path = "DYLD_LIBRARY_PATH";
#else
        constexpr string_view ld_library_path = "LD_LIBRARY_PATH";
#endif // __APPLE__
        char* ldp = getenv(ld_library_path.data());
        vector<string> result = split<char>(ldp ? ldp : "", ':');
        result.push_back("../lib");
        return result;
    }
#endif // WIN32 || __MINGW32__

    static path get_full_path(string_view name, bool in_lib = false)
    {
        path pname = name;
        if (pname.is_absolute()) return pname;
        auto search_dirs = get_search_path();
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
