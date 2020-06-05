#include <boost/nowide/filesystem.hpp>
#include <vector>
#include <xaml/meta/module.h>

#ifdef XAML_WIN32
    #include <Windows.h>
    #include <xaml/result_win32.h>
#else
    #include <dlfcn.h>
#endif // XAML_WIN32

#if defined(XAML_WIN32) && !defined(XAML_MINGW)
static constexpr bool has_prefix = false;
#else
static constexpr bool has_prefix = true;
#endif // XAML_WIN32 && !XAML_MINGW
static inline const std::string module_prefix{ "lib" };

#ifdef XAML_WIN32
static inline const boost::nowide::filesystem::path module_extension{ ".dll" };
#elif defined(XAML_APPLE)
static inline const boost::nowide::filesystem::path module_extension{ ".dylib" };
#else
static inline const boost::nowide::filesystem::path module_extension{ ".so" };
#endif // XAML_WIN32

using namespace std;
using boost::nowide::filesystem::path;

static path get_full_path(path const& name)
{
    path pname = name.filename();
    if (name != pname) return name;
    if constexpr (has_prefix)
    {
        pname = module_prefix + pname.string();
    }
    pname.replace_extension(module_extension);
    return pname;
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
    try
    {
        if (m_handle)
        {
            XAML_RETURN_IF_WIN32_BOOL_FALSE(FreeLibrary(m_handle));
        }
        auto p = get_full_path(to_string_view(path));
        XAML_RETURN_IF_FAILED(xaml_string_new(get_module_name(p), &m_name));
        m_handle = LoadLibraryW(p.c_str());
        if (!m_handle) return HRESULT_FROM_WIN32(GetLastError());
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

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
    try
    {
        if (m_handle)
        {
            int res = dlclose(m_handle);
            if (res) return XAML_E_FAIL;
        }
        auto p = get_full_path(to_string_view(path));
        XAML_RETURN_IF_FAILED(xaml_string_new(get_module_name(p), &m_name));
        m_handle = dlopen(p.c_str(), RTLD_LAZY);
        if (!m_handle)
        {
            xaml_result_raise_message(XAML_E_FAIL, xaml_result_raise_error, dlerror());
            return XAML_E_FAIL;
        }
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

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
