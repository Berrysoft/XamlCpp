#include <algorithm>
#include <iterator>
#include <ostream>
#include <xaml/compiler.hpp>

using namespace std;

namespace xaml
{
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

    ostream& compiler::write_type(ostream& stream, type_index type)
    {
        auto t = *get_type_name(type);
        return stream << "::" << get<0>(t) << "::" << get<1>(t);
    }

    ostream& compiler::write_construct(ostream& stream, string_view name, type_index type)
    {
        return write_type(write_indent(stream) << "auto " << name << " = ::xaml::construct<", type) << ">();" << endl;
    }

    ostream& compiler::write_call(ostream& stream, string_view name, string_view method, string_view args)
    {
        return write_indent(stream) << name << "->" << method << '(' << args << ");" << endl;
    }

    ostream& compiler::write_set_property(ostream& stream, string_view name, std::string_view prop, std::string_view value)
    {
        return write_call(stream, name, "set_" + (string)prop, value);
    }

    ostream& compiler::write_add_property(ostream& stream, string_view name, std::string_view prop, std::string_view value)
    {
        return write_call(stream, name, "add_" + (string)prop, value);
    }

    ostream& compiler::write_add_event(ostream& stream, string_view name, xaml_event& ev)
    {
        return write_call(stream, name, "add_" + (string)ev.info.name(), ev.value);
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
                write_set_property(stream, get_node_name(node, is_this), prop.info.name(), get<string>(value));
                break;
            case 2: // xaml_node
            {
                auto& n = get<xaml_node>(value);
                write_construct(stream, n.name, n.type);
                compile_impl(stream, n, false);
                write_set_property(stream, get_node_name(node, is_this), prop.info.name(), n.name);
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
                write_add_property(stream, get_node_name(node, is_this), prop.second.info.name(), n.name);
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
                        write_set_property(stream, n.name, p.info.name(), get<string>(value));
                        break;
                    }
                }
                write_call(stream, n.name, "provide", "");
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
            auto [ns, name] = *get_type_name(root_node.type);
            write_indent(stream) << "namespace " << ns << endl;
            write_begin_block(stream);
            write_indent(stream) << name << "::initialize_component()" << endl;
            write_begin_block(stream);
            compile_impl(stream, root_node, true);
            compile_extensions(stream, root_node, true);
            write_end_block(stream);
            write_end_block(stream);
        }
        return stream;
    }
} // namespace xaml
