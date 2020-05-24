#include <filesystem>
#include <vector>
#include <xaml/internal/filesystem.hpp>
#include <xaml/meta/module.h>

#ifdef XAML_WIN32
#include <Windows.h>
#include <xaml/result_win32.h>
#else
#include <dlfcn.h>
#ifdef XAML_APPLE
#include <mach-o/dyld.h>
#endif // XAML_APPLE
#endif // XAML_WIN32

#if defined(XAML_WIN32) && !defined(XAML_MINGW)
static constexpr bool has_prefix = false;
#else
static constexpr bool has_prefix = true;
#endif // XAML_WIN32 && !XAML_MINGW
static inline std::filesystem::path module_prefix{ "lib" };

#ifdef XAML_WIN32
static inline std::filesystem::path module_extension{ ".dll" };
#elif defined(XAML_APPLE)
static inline std::filesystem::path module_extension{ ".dylib" };
#else
static inline std::filesystem::path module_extension{ ".so" };
#endif // XAML_WIN32

using namespace std;
using namespace std::filesystem;

template <typename Char>
static vector<path> split(basic_string_view<Char> str, Char separater)
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
        if (index == decltype(str)::npos) break;
        offset = index + 1;
    }
    return result;
}

static path program_location()
{
#ifdef XAML_WIN32
    wstring path(1024, L'\0');
    if (GetModuleFileNameW(nullptr, path.data(), (DWORD)path.size()) == 0) return {};
    return path;
#elif defined(XAML_APPLE)
    string path(1024, '\0');
    uint32_t size = (uint32_t)path.size();
    if (_NSGetExecutablePath(path.data(), &size) == 0)
        return path;
    path.resize((int32_t)size);
    if (_NSGetExecutablePath(path.data(), &size) != 0)
        return {};
    return path;
#else
    return read_symlink("/proc/self/exe");
#endif // XAML_WIN32
}

static vector<path> get_module_search_path()
{
#ifdef XAML_WIN32
    wstring buffer(32767, L'\0');
    DWORD count = GetEnvironmentVariableW(L"PATH", buffer.data(), (DWORD)buffer.length());
    buffer.resize(count);
    vector<path> result = split<wchar_t>(buffer, L';');
#else
#ifdef XAML_APPLE
    constexpr string_view ld_library_path = "DYLD_LIBRARY_PATH";
#else
    constexpr string_view ld_library_path = "LD_LIBRARY_PATH";
#endif // XAML_APPLE
    char* ldp = getenv(ld_library_path.data());
    vector<path> result = split<char>(ldp ? ldp : string_view{}, ':');
#endif // XAML_WIN32
    result.push_back(".");
    result.push_back("../lib");
#ifdef CMAKE_INTDIR
    result.push_back("../../lib/" CMAKE_INTDIR);
#endif // CMAKE_INTDIR
    auto location = program_location().parent_path();
    if (!location.empty())
    {
        result.push_back(location);
        result.push_back(location / "../lib");
#ifdef CMAKE_INTDIR
        result.push_back(location / "../../lib" / CMAKE_INTDIR);
#endif // CMAKE_INTDIR
    }
    return result;
}

static path get_full_path(path const& name)
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
    for (auto& dir : search_dirs)
    {
        path fullname = pname;
        fullname += module_extension;
        path p = path{ dir } / fullname;
        if (exists(p)) return p;
    }
    return name;
}

static path::string_type get_module_name(path file)
{
    auto result = file.filename().replace_extension().native();
    if constexpr (has_prefix)
    {
        result = result.substr(3);
    }
    return result;
}

struct xaml_module_impl : xaml_implement<xaml_module_impl, xaml_module, xaml_object>
{
    xaml_ptr<xaml_string> m_name;

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        return m_name.query(ptr);
    }

#ifdef XAML_WIN32
    HMODULE m_handle{};

    xaml_result XAML_CALL open(xaml_string* path) noexcept override
    {
        try
        {
            if (m_handle)
            {
                XAML_RETURN_IF_WIN32_BOOL_FALSE(FreeLibrary(m_handle));
            }
            auto p = get_full_path(to_path(path));
            XAML_RETURN_IF_FAILED(xaml_string_new(get_module_name(p), &m_name));
            m_handle = LoadLibraryW(p.c_str());
            if (!m_handle) return HRESULT_FROM_WIN32(GetLastError());
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }

    xaml_result XAML_CALL get_method(xaml_string* name, void** ptr) noexcept override
    {
        if (!m_handle) return XAML_E_NOTIMPL;
        string_view data;
        XAML_RETURN_IF_FAILED(to_string_view(name, &data));
        auto proc = GetProcAddress(m_handle, data.data());
        if (!proc) return XAML_E_NOTIMPL;
        *ptr = (void*)proc;
        return XAML_S_OK;
    }
#else
    void* m_handle{};

    xaml_result XAML_CALL open(xaml_string* path) noexcept override
    {
        try
        {
            if (m_handle)
            {
                int res = dlclose(m_handle);
                if (res) return XAML_E_FAIL;
            }
            auto p = get_full_path(to_path(path));
            XAML_RETURN_IF_FAILED(xaml_string_new(get_module_name(p), &m_name));
            m_handle = dlopen(p.c_str(), RTLD_LAZY);
            if (!m_handle) return XAML_E_FAIL;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }

    xaml_result XAML_CALL get_method(xaml_string* name, void** ptr) noexcept override
    {
        if (!m_handle) return XAML_E_NOTIMPL;
        string_view data;
        XAML_RETURN_IF_FAILED(to_string_view(name, &data));
        auto proc = dlsym(m_handle, data.data());
        if (!proc) return XAML_E_NOTIMPL;
        *ptr = (void*)proc;
        return XAML_S_OK;
    }
#endif // XAML_WIN32
};

xaml_result XAML_CALL xaml_module_new(xaml_module** ptr) noexcept
{
    return xaml_object_new<xaml_module_impl>(ptr);
}
