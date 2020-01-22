#include <xaml/meta/meta.hpp>
#include <xaml/meta/module.hpp>

#ifdef WIN32
#include <system_error>
#include <wil/result_macros.h>
#else
#include <dlfcn.h>
#endif // WIN32

using namespace std;

namespace xaml
{
#ifdef WIN32
    void module::open(string_view_t path)
    {
        close();
        set_handle(LoadLibrary(path.data()));
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
    void module::open(string_view_t path)
    {
        close();
        set_handle(dlopen(path.data(), RTLD_LAZY));
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

    void module::init_components() noexcept
    {
        void* (*pinit)() noexcept = (void* (*)() noexcept)get_method("init_components");
        if (pinit) m_token = pinit();
    }

    void module::cleanup_components() noexcept
    {
        void (*pcleanup)(void*) noexcept = (void (*)(void*) noexcept)get_method("init_components");
        if (pcleanup) pcleanup(m_token);
    }
} // namespace xaml
