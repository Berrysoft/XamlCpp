#include <xaml/meta/meta.hpp>
#include <xaml/meta/module.hpp>

#ifdef WIN32
#include <system_error>
#include <wil/result_macros.h>
#else

#endif // WIN32

using namespace std;

namespace xaml
{
#ifdef WIN32
    void module::open(string_view_t path)
    {
        set_handle(LoadLibrary(path.data()));
        if (!get_handle())
        {
            throw system_error(GetLastError(), system_category());
        }
    }

    void* module::get_method(string_view name)
    {
        FARPROC p = GetProcAddress(get_handle(), name.data());
        if (!p)
        {
            throw system_error(GetLastError(), system_category());
        }
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

    void module::register_meta() noexcept
    {
        void (*pinit)(void*) = (void (*)(void*))get_method("init_meta");
        pinit(&__get_context());
    }
#else

#endif // WIN32
} // namespace xaml
