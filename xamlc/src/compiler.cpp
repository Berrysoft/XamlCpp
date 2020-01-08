#include <algorithm>
#include <array>
#include <iterator>
#include <ostream>
#include <xaml/compiler.hpp>

using namespace std;

namespace xaml
{
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
        if (is_of_type(type, int_types) || is_of_type(type, float_types))
        {
            return (string)code;
        }
        else if (is_of_type(type, string_types))
        {
            ostringstream stream;
            stream << "U(\"" << code << "\")";
            return stream.str();
        }
        else
        {
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
        return write_indent(stream) << "void " << name << "::initialize_component()" << endl;
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
        return write_type(write_indent(stream) << "auto " << name << " = ::xaml::construct<", type) << ">();" << endl;
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

    ostream& compiler::write_add_event(ostream& stream, string_view name, xaml_event& ev)
    {
        return write_call(stream, name, "add_", ev.info.name(), { ev.value });
    }

    constexpr string_view this_name{ "this" };

    static string_view get_node_name(xaml_node& node, bool is_this)
    {
        return is_this ? this_name : node.name;
    }

    ostream& compiler::compile_impl(ostream& stream, xaml_node& node, bool is_this)
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
                compile_impl(stream, n, false);
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
                compile_impl(stream, n, false);
                write_add_property(stream, node.type, prop.second.host_type, prop.second.info.type(), get_node_name(node, is_this), prop.second.info.name(), n.name);
            }
        }
        for (auto& ev : node.events)
        {
            write_add_event(stream, get_node_name(node, is_this), ev);
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
                write_construct(stream, n.name, n.type);
                for (auto& p : n.properties)
                {
                    auto& value = p.value;
                    switch (value.index())
                    {
                    case 0: // std::string
                        write_set_property(stream, n.type, p.host_type, p.info.type(), n.name, p.info.name(), get<string>(value));
                        break;
                    }
                }
                write_call(stream, n.name, {}, "provide", {});
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

    ostream& compiler::compile(ostream& stream)
    {
        if (stream)
        {
            xaml_node root_node = reader.parse();
            if (root_node.map_class)
            {
                auto [ns, name] = *root_node.map_class;
                write_namespace(stream, ns);
                write_begin_block(stream);
                write_init_decl(stream, name);
                write_begin_block(stream);
                compile_impl(stream, root_node, true);
                compile_extensions(stream, root_node, true);
                write_end_block(stream);
                write_end_block(stream);
            }
            else
            {
                write_construct(stream, root_node.name, root_node.type);
                compile_impl(stream, root_node, false);
                compile_extensions(stream, root_node, false);
            }
        }
        return stream;
    }
} // namespace xaml
