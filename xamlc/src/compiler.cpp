#include <algorithm>
#include <array>
#include <iterator>
#include <map>
#include <ostream>
#include <xaml/compiler.hpp>
#include <xaml/deserializer.hpp>
#include <xaml/markup/binding.hpp>
#include <xaml/ui/drawing.hpp>

using namespace std;

namespace xaml
{
    bool compiler_module::can_compile(type_index type)
    {
        int (*pcc)(void*) noexcept = (int (*)(void*) noexcept)get_method("can_compile");
        return pcc ? pcc(&type) : false;
    }

    string compiler_module::compile(type_index type, string_view code)
    {
        void (*pc)(void*, const char*, void*) noexcept = (void (*)(void*, const char*, void*) noexcept)get_method("compile");
        if (pc)
        {
            string result;
            pc(&type, code.data(), &result);
            return result;
        }
        return {};
    }

    static map<string_view, shared_ptr<compiler_module>> m_cmodules;

    void add_compiler_module(string_view path)
    {
        auto m = make_shared<compiler_module>(path);
        m->register_meta();
        m_cmodules.emplace(path, m);
    }

    static array int_types{
        type_index(typeid(int8_t)), type_index(typeid(int16_t)), type_index(typeid(int32_t)), type_index(typeid(int64_t)),
        type_index(typeid(uint8_t)), type_index(typeid(uint16_t)), type_index(typeid(uint32_t)), type_index(typeid(uint64_t))
    };

    static array float_types{
        type_index(typeid(float)), type_index(typeid(double)), type_index(typeid(long double))
    };

    static array string_types{
        type_index(typeid(string)), type_index(typeid(string_view)),
        type_index(typeid(wstring)), type_index(typeid(wstring_view))
    };

    static array t2d_types{
        type_index(typeid(tuple<double, double>)), type_index(typeid(pair<double, double>)), type_index(typeid(array<double, 2>)),
        type_index(typeid(point)), type_index(typeid(size))
    };

    static array t4d_types{
        type_index(typeid(tuple<double, double, double, double>)), type_index(typeid(array<double, 4>)),
        type_index(typeid(rectangle)), type_index(typeid(margin))
    };

    template <size_t N>
    static bool is_of_type(type_index type, array<type_index, N> const& arr)
    {
        for (auto& t : arr)
        {
            if (t == type) return true;
        }
        return false;
    }

    static string xaml_cpp_compile(type_index type, string_view code)
    {
        if (type == type_index(typeid(bool)))
        {
            bool b = value_converter_traits<bool>::convert(code);
            return b ? "true" : "false";
        }
        else if (is_of_type(type, int_types) || is_of_type(type, float_types))
        {
            return (string)code;
        }
        else if (is_of_type(type, string_types))
        {
            ostringstream stream;
            stream << "U(\"" << code << "\")";
            return stream.str();
        }
        else if (is_of_type(type, t2d_types))
        {
            ostringstream stream;
            auto t = value_converter_traits<tuple<double, double>>::convert(code);
            stream << "{ " << get<0>(t) << ", " << get<1>(t) << " }";
            return stream.str();
        }
        else if (is_of_type(type, t4d_types))
        {
            ostringstream stream;
            auto t = value_converter_traits<tuple<double, double, double, double>>::convert(code);
            stream << "{ " << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ", " << get<3>(t) << " }";
            return stream.str();
        }
        else if (is_registered_enum(type))
        {
            auto [ns, name] = *get_type_name(type);
            std::ostringstream stream;
            stream << "::" << ns << "::" << name << "::" << code;
            return stream.str();
        }
        else
        {
            for (auto& p : m_cmodules)
            {
                if (p.second->can_compile(type))
                {
                    return p.second->compile(type, code);
                }
            }
            return (string)code;
        }
    }

    ostream& compiler::write_indent(ostream& stream)
    {
        fill_n(ostream_iterator<char>(stream), indent_count * 4, ' ');
        return stream;
    }

    ostream& compiler::write_begin_block(ostream& stream)
    {
        write_indent(stream) << '{' << endl;
        indent_count++;
        return stream;
    }

    ostream& compiler::write_end_block(ostream& stream)
    {
        indent_count--;
        return write_indent(stream) << '}' << endl;
    }

    ostream& compiler::write_namespace(ostream& stream, string_view ns)
    {
        return write_indent(stream) << "namespace " << ns << endl;
    }

    ostream& compiler::write_init_decl(ostream& stream, string_view name)
    {
        return write_indent(stream) << "void " << name << "::init_components()" << endl;
    }

    ostream& compiler::write_type(ostream& stream, type_index type)
    {
        auto t = *get_type_name(type);
        return stream << "::" << get<0>(t) << "::" << get<1>(t);
    }

    ostream& compiler::write_args(ostream& stream, initializer_list<string_view> args)
    {
        auto bit = args.begin();
        auto eit = args.end();
        if (bit != eit)
        {
            stream << *bit;
            for (++bit; bit != eit; ++bit)
            {
                stream << ", " << *bit;
            }
        }
        return stream;
    }

    ostream& compiler::write_construct(ostream& stream, string_view name, type_index type)
    {
        return write_type(write_indent(stream) << "auto " << name << " = ::std::make_shared<", type) << ">();" << endl;
    }

    ostream& compiler::write_call(ostream& stream, string_view name, string_view prefix, string_view method, initializer_list<string_view> args)
    {
        return write_args(write_indent(stream) << name << "->" << prefix << method << '(', args) << ");" << endl;
    }

    ostream& compiler::write_static_call(ostream& stream, type_index type, string_view prefix, string_view method, initializer_list<string_view> args)
    {
        return write_args(write_type(write_indent(stream), type) << "::" << prefix << method << '(', args) << ");" << endl;
    }

    ostream& compiler::write_set_property(ostream& stream, string_view name, string_view prop, string_view value)
    {
        return write_call(stream, name, "set_", prop, { value });
    }

    ostream& compiler::write_set_property(ostream& stream, type_index type, string_view name, string_view prop, string_view value)
    {
        return write_static_call(stream, type, "set_", prop, { name, value });
    }

    ostream& compiler::write_set_property(ostream& stream, type_index node_type, type_index host_type, type_index prop_type, string_view name, string_view prop, string_view value)
    {
        if (node_type == host_type)
        {
            return write_set_property(stream, name, prop, xaml_cpp_compile(prop_type, value));
        }
        else
        {
            return write_set_property(stream, host_type, name, prop, xaml_cpp_compile(prop_type, value));
        }
    }

    ostream& compiler::write_add_property(ostream& stream, string_view name, string_view prop, string_view value)
    {
        return write_call(stream, name, "add_", prop, { value });
    }

    ostream& compiler::write_add_property(ostream& stream, type_index type, string_view name, string_view prop, string_view value)
    {
        return write_static_call(stream, type, "add_", prop, { name, value });
    }

    ostream& compiler::write_add_property(ostream& stream, type_index node_type, type_index host_type, type_index prop_type, string_view name, string_view prop, string_view value)
    {
        if (node_type == host_type)
        {
            return write_add_property(stream, name, prop, xaml_cpp_compile(prop_type, value));
        }
        else
        {
            return write_add_property(stream, host_type, name, prop, xaml_cpp_compile(prop_type, value));
        }
    }

    ostream& compiler::write_add_event(ostream& stream, xaml_node& this_node, string_view name, xaml_event& ev)
    {
        ostringstream s;
        auto [ns, n] = *this_node.map_class;
        s << "::xaml::mem_fn_bind(&::" << ns << "::" << n << "::" << ev.value << ", this)";
        return write_call(stream, name, "add_", ev.info.name(), { s.str() });
    }

    ostream& compiler::write_markup(ostream& stream, string_view name, string_view prop, shared_ptr<meta_class> markup)
    {
        if (markup->this_type() == type_index(typeid(binding)))
        {
            auto b = static_pointer_cast<binding>(markup);
            auto mode = b->get_mode();
            if (mode & binding_mode::one_way)
            {
                ostringstream s;
                s << "[&" << name << "](auto&, auto value) { " << name << ".set_" << prop << "(value); }";
                write_call(stream, b->get_element(), "add_", (string)b->get_path() + "_changed", { s.str() });
            }
            if (mode & binding_mode::one_way_to_source)
            {
                ostringstream s;
                s << "[&" << b->get_element() << "](auto&, auto value) { " << b->get_element() << ".set_" << b->get_path() << "(value); }";
                write_call(stream, name, "add_", (string)prop + "_changed", { s.str() });
            }
        }
        return stream;
    }

    constexpr string_view this_name{ "this" };

    static string_view get_node_name(xaml_node& node, bool is_this)
    {
        return is_this ? this_name : node.name;
    }

    ostream& compiler::compile_impl(ostream& stream, xaml_node& node, xaml_node& this_node, bool is_this)
    {
        for (auto& prop : node.properties)
        {
            auto& value = prop.value;
            switch (value.index())
            {
            case 0: // std::string
                write_set_property(stream, node.type, prop.host_type, prop.info.type(), get_node_name(node, is_this), prop.info.name(), get<string>(value));
                break;
            case 2: // xaml_node
            {
                auto& n = get<xaml_node>(value);
                write_construct(stream, n.name, n.type);
                compile_impl(stream, n, this_node, false);
                write_set_property(stream, node.type, prop.host_type, prop.info.type(), get_node_name(node, is_this), prop.info.name(), n.name);
                break;
            }
            }
        }
        for (auto& prop : node.collection_properties)
        {
            for (auto& n : prop.second.values)
            {
                write_construct(stream, n.name, n.type);
                compile_impl(stream, n, this_node, false);
                write_add_property(stream, node.type, prop.second.host_type, prop.second.info.type(), get_node_name(node, is_this), prop.second.info.name(), n.name);
            }
        }
        for (auto& ev : node.events)
        {
            write_add_event(stream, this_node, get_node_name(node, is_this), ev);
        }
        return stream;
    }

    ostream& compiler::compile_extensions(ostream& stream, xaml_node& node, bool is_this)
    {
        for (auto& prop : node.properties)
        {
            auto& value = prop.value;
            switch (value.index())
            {
            case 1: // markup_node
            {
                auto& n = get<markup_node>(value);
                deserializer des{};
                auto ex = des.deserialize(n);
                write_markup(stream, node.name, prop.info.name(), ex);
                break;
            }
            case 2: // xaml_node
                compile_extensions(stream, get<xaml_node>(value), false);
                break;
            }
        }
        for (auto& prop : node.collection_properties)
        {
            for (auto& n : prop.second.values)
            {
                compile_extensions(stream, n, false);
            }
        }
        return stream;
    }

    ostream& compiler::compile(xaml_node& node, ostream& stream)
    {
        if (stream)
        {
            if (node.map_class)
            {
                auto [ns, name] = *node.map_class;
                write_namespace(stream, ns);
                write_begin_block(stream);
                write_init_decl(stream, name);
                write_begin_block(stream);
                compile_impl(stream, node, node, true);
                compile_extensions(stream, node, true);
                write_end_block(stream);
                write_end_block(stream);
            }
            else
            {
                write_construct(stream, node.name, node.type);
                compile_impl(stream, node, node, false);
                compile_extensions(stream, node, false);
            }
        }
        return stream;
    }
} // namespace xaml
