#include <ostream>
#include <xaml/compiler.hpp>

using namespace std;

namespace xaml
{
    ostream& compiler::write_type(ostream& stream, type_index type)
    {
        auto t = *get_type_name(type);
        return stream << "::" << get<0>(t) << "::" << get<1>(t);
    }

    ostream& compiler::write_construct(ostream& stream, string_view name, type_index type)
    {
        return write_type(stream << "auto " << name << " = ::xaml::construct<", type) << ">();" << endl;
    }

    ostream& compiler::write_call(ostream& stream, string_view name, string_view method, string_view args)
    {
        return stream << name << "->" << method << '(' << args << ");" << endl;
    }

    ostream& compiler::write_set_property(ostream& stream, string_view name, xaml_property& prop)
    {
        return write_call(stream, name, "set_" + (string)prop.info.name(), prop.value);
    }

    ostream& compiler::write_set_property(ostream& stream, string_view name, xaml_construct_property& prop)
    {
        return write_call(stream, name, "set_" + (string)prop.info.name(), prop.value.name);
    }

    ostream& compiler::write_add_child(ostream& stream, string_view parent, string_view child)
    {
        return write_call(stream, parent, "add_child", child);
    }

    ostream& compiler::write_add_event(ostream& stream, string_view name, xaml_event& ev)
    {
        return write_call(stream, name, "add_" + (string)ev.info.name(), ev.value);
    }

    ostream& compiler::compile_impl(ostream& stream, xaml_node& node, type_index root)
    {
        for (auto& prop : node.properties)
        {
            write_set_property(stream, node.name, prop);
        }
        for (auto& ev : node.events)
        {
            write_add_event(stream, node.name, ev);
        }
        for (auto& prop : node.construct_properties)
        {
            write_construct(stream, prop.value.name, prop.value.type);
            compile_impl(stream, prop.value, root);
            write_set_property(stream, node.name, prop);
        }
        bool is_container = invoke_static_method<bool>(node.type, "is_container").value_or(false);
        bool is_multicontainer = invoke_static_method<bool>(node.type, "is_multicontainer").value_or(false);
        if (is_container)
        {
            if (node.children.size() > 1 && !is_multicontainer)
            {
                throw xaml_not_multicontainer(node.type);
            }
            for (auto& cnode : node.children)
            {
                write_construct(stream, cnode.name, cnode.type);
                compile_impl(stream, cnode, root);
                if (is_multicontainer)
                {
                    write_add_child(stream, node.name, cnode.name);
                }
                else
                {
                    auto prop = get_property(node.type, "child");
                    write_set_property(stream, node.name, xaml_property{ prop, cnode.name });
                }
            }
        }
        return stream;
    }

    ostream& compiler::compile_extensions(ostream& stream, xaml_node& node)
    {
        for (auto& prop : node.extension_properties)
        {
            write_construct(stream, prop.value.name, prop.value.type);
            for (auto& p : prop.value.properties)
            {
                write_set_property(stream, node.name, p);
            }
            write_call(stream, prop.value.name, "provide", "");
        }
        for (auto& c : node.children)
        {
            compile_extensions(stream, c);
        }
        return stream;
    }

    ostream& compiler::compile(ostream& stream)
    {
        if (stream)
        {
            xaml_node root_node = reader.parse();
            compile_impl(stream, root_node, root_node.type);
            compile_extensions(stream, root_node);
        }
        return stream;
    }
} // namespace xaml
