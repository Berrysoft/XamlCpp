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
        type_index(typeid(tuple<double, double>)), type_index(typeid(pair<double, double>)), type_index(typeid(array<double, 2>)),
        type_index(typeid(point)), type_index(typeid(size))
    };

    static array t4d_types{
        type_index(typeid(tuple<double, double, double, double>)), type_index(typeid(array<double, 4>)),
        type_index(typeid(rectangle)), type_index(typeid(margin))
    };

    template <size_t N>
    static bool is_of_type(type_index const& type, array<type_index, N> const& arr)
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

    XAML_UI_META_API int can_compile(void* t) noexcept
    {
        type_index& type = *(type_index*)t;
        return is_of_type(type, t2d_types) || is_of_type(type, t4d_types);
    }

    XAML_UI_META_API void compile(void* t, const char* code, size_t code_len, void* res) noexcept
    {
        type_index& type = *(type_index*)t;
        if (is_of_type(type, t2d_types))
        {
            ostringstream stream;
            auto t = value_converter_traits<tuple<double, double>>::convert(code);
            stream << "{ " << get<0>(t) << ", " << get<1>(t) << " }";
            *(string*)res = stream.str();
        }
        else if (is_of_type(type, t4d_types))
        {
            ostringstream stream;
            auto t = value_converter_traits<tuple<double, double, double, double>>::convert(code);
            stream << "{ " << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ", " << get<3>(t) << " }";
            *(string*)res = stream.str();
        }
        else
        {
            *(string*)res = {};
        }
    }
}
