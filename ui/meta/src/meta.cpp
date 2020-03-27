#include <tuple>
#include <xaml/ui/application.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/drawing.hpp>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/window.hpp>
#include <xaml/version.hpp>

using namespace std;

namespace xaml
{
    static array t2d_types{
        type_guid_v<meta_box<tuple<double, double>>>,
        type_guid_v<meta_box<point>>, type_guid_v<meta_box<size>>
    };

    static array t4d_types{
        type_guid_v<meta_box<tuple<double, double, double, double>>>,
        type_guid_v<meta_box<rectangle>>, type_guid_v<meta_box<margin>>
    };

    template <size_t N>
    static bool is_of_type(guid const& type, array<guid, N> const& arr)
    {
        return find(arr.begin(), arr.end(), type) != arr.end();
    }
} // namespace xaml

using namespace xaml;

extern "C"
{
    XAML_UI_META_API void xaml_version(version* ver) noexcept { *ver = current_version; }

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
        REGISTER_ENUM(xaml, halignment_t, "xaml/ui/control.hpp");
        REGISTER_ENUM(xaml, valignment_t, "xaml/ui/control.hpp");
    }

    XAML_UI_META_API int can_compile(void const* t) noexcept
    {
        guid const& type = *(guid const*)t;
        return is_of_type(type, t2d_types) || is_of_type(type, t4d_types);
    }

    XAML_UI_META_API void compile(void const* ty, const char* code, size_t code_len, void* res) noexcept
    {
        guid const& type = *(guid const*)ty;
        if (is_of_type(type, t2d_types))
        {
            ostringstream stream;
            auto t = value_converter_traits<tuple<double, double>>::convert(box_value(code));
            stream << "{ " << get<0>(t) << ", " << get<1>(t) << " }";
            *(string*)res = stream.str();
        }
        else if (is_of_type(type, t4d_types))
        {
            ostringstream stream;
            auto t = value_converter_traits<tuple<double, double, double, double>>::convert(box_value(code));
            stream << "{ " << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ", " << get<3>(t) << " }";
            *(string*)res = stream.str();
        }
        else
        {
            *(string*)res = {};
        }
    }
}
