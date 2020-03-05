#include <xaml/ui/application.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace xaml;

extern "C"
{
    XAML_UI_META_API void init_meta(void* pctx) noexcept
    {
        meta_context& ctx = *(meta_context*)pctx;
        ctx.add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
        register_class<
            control,
            container,
            multicontainer,
            window,
            menu_bar>(ctx);
        REGISTER_ENUM(xaml, halignment_t);
        REGISTER_ENUM(xaml, valignment_t);
    }

    const char* const s_headers[] = {
        "xaml/ui/application.hpp",
        "xaml/ui/container.hpp",
        "xaml/ui/window.hpp",
        nullptr
    };
    XAML_UI_META_API const char* const* include_headers() noexcept { return s_headers; }
}
