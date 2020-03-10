#include <algorithm>
#include <array>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <xaml/compiler.hpp>
#include <xaml/deserializer.hpp>
#include <xaml/markup/binding.hpp>

using namespace std;

namespace xaml
{
    static bool can_compile(module const& m, type_index type) noexcept
    {
        using can_compile_t = int (*)(void*) noexcept;
        auto pcc = (can_compile_t)m.get_method("can_compile");
        return pcc ? pcc(&type) : false;
    }

    static string module_compile(module const& m, type_index type, string_view code) noexcept
    {
        using compile_t = void (*)(void*, const char*, size_t, void*) noexcept;
        auto pc = (compile_t)m.get_method("compile");
        if (pc)
        {
            string result;
            pc(&type, code.data(), code.length(), &result);
            return result;
        }
        return {};
    }

    static set<string_view> include_headers(module const& m) noexcept
    {
        using include_headers_t = const char* const* (*)() noexcept;
        auto pih = (include_headers_t)m.get_method("include_headers");
        if (pih)
        {
            const char* const* headers = pih();
            set<string_view> result;
            for (; *headers; headers++)
            {
                result.emplace(*headers);
            }
            return result;
        }
        return {};
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

    template <size_t N>
    static bool is_of_type(type_index const& type, array<type_index, N> const& arr)
    {
        return find(arr.begin(), arr.end(), type) != arr.end();
    }

    string compiler::xaml_cpp_compile(type_index type, string_view code)
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
            stream << "U(" << quoted(code) << ')';
            return stream.str();
        }
        else if (m_ctx->is_registered_enum(type))
        {
            std::ostringstream stream;
            auto t = m_ctx->get_enum_type(type);
            write_type(stream, t) << "::" << code;
            return stream.str();
        }
        else
        {
            for (auto& p : m_ctx->get_modules())
            {
                if (can_compile(*p.second, type))
                {
                    return module_compile(*p.second, type, code);
                }
            }
            return (string)code;
        }
    }

    ostream& compiler::write_indent(ostream& stream)
    {
        fill_n(ostream_iterator<char>(stream), m_indent_count * 4, ' ');
        return stream;
    }

    ostream& compiler::write_include(ostream& stream, string_view header)
    {
        if (!header.empty())
            return write_indent(stream) << "#include <" << header << '>' << endl;
        else
            return stream;
    }

    ostream& compiler::write_begin_block(ostream& stream)
    {
        write_indent(stream) << '{' << endl;
        m_indent_count++;
        return stream;
    }

    ostream& compiler::write_end_block(ostream& stream)
    {
        m_indent_count--;
        return write_indent(stream) << '}' << endl;
    }

    ostream& compiler::write_init_decl(ostream& stream, string_view ns, string_view name)
    {
        return write_type(write_indent(stream) << "void ", ns, name) << "::init_components()" << endl;
    }

    ostream& compiler::write_init_decl_with_meta(ostream& stream, string_view ns, string_view name)
    {
        return write_type(write_indent(stream) << "void ", ns, name) << "::init_components(::xaml::meta_context& ctx)" << endl;
    }

    ostream& compiler::write_type(ostream& stream, reflection_info const* type)
    {
        auto& [ns, name] = type->get_type_name();
        return write_type(stream, ns, name);
    }

    ostream& compiler::write_type(ostream& stream, enum_reflection_info const* type)
    {
        auto& [ns, name] = type->get_type_name();
        return write_type(stream, ns, name);
    }

    ostream& compiler::write_type(ostream& stream, string_view ns, string_view name)
    {
        return stream << "::" << ns << "::" << name;
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

    ostream& compiler::write_construct(ostream& stream, string_view name, reflection_info const* type)
    {
        return write_type(write_indent(stream) << "auto " << name << " = ::std::make_shared<", type) << ">();" << endl;
    }

    ostream& compiler::write_call(ostream& stream, string_view name, string_view prefix, string_view method, initializer_list<string_view> args)
    {
        return write_args(write_indent(stream) << name << "->" << prefix << method << '(', args) << ");" << endl;
    }

    ostream& compiler::write_static_call(ostream& stream, reflection_info const* type, string_view prefix, string_view method, initializer_list<string_view> args)
    {
        return write_args(write_type(write_indent(stream), type) << "::" << prefix << method << '(', args) << ");" << endl;
    }

    ostream& compiler::write_set_property(ostream& stream, string_view name, string_view prop, string_view value)
    {
        return write_call(stream, name, "set_", prop, { value });
    }

    ostream& compiler::write_set_property(ostream& stream, reflection_info const* type, string_view name, string_view prop, string_view value)
    {
        string deref_name = "*" + (string)name;
        return write_static_call(stream, type, "set_", prop, { deref_name, value });
    }

    ostream& compiler::write_set_property(ostream& stream, reflection_info const* node_type, reflection_info const* host_type, type_index prop_type, string_view name, string_view prop, string_view value)
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

    ostream& compiler::write_add_property(ostream& stream, reflection_info const* type, string_view name, string_view prop, string_view value)
    {
        return write_static_call(stream, type, "add_", prop, { name, value });
    }

    ostream& compiler::write_add_property(ostream& stream, reflection_info const* node_type, reflection_info const* host_type, type_index prop_type, string_view name, string_view prop, string_view value)
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
        string arg = s.str();
        return write_call(stream, name, "add_", ev.info->name(), { arg });
    }

    ostream& compiler::write_bind(std::ostream& stream, std::string_view target_name, std::string_view target_prop, std::string_view source_name, std::string_view source_prop)
    {
        {
            ostringstream s;
            s << source_name << "->get_" << source_prop << "()";
            string arg = s.str();
            write_set_property(stream, target_name, target_prop, { (string_view)arg });
        }
        {
            ostringstream s;
            s << "[" << target_name << "](auto&, auto value) { " << target_name << "->set_" << target_prop << "(value); }";
            string arg = s.str();
            write_call(stream, source_name, "add_", (std::string)source_prop + "_changed", { arg });
        }
        return stream;
    }

    ostream& compiler::write_markup(ostream& stream, string_view name, string_view prop, shared_ptr<meta_class> markup)
    {
        if (markup->this_type() == type_index(typeid(binding)))
        {
            auto b = static_pointer_cast<binding>(markup);
            auto mode = b->get_mode();
            if (mode & binding_mode::one_way)
            {
                write_bind(stream, name, prop, b->get_element(), b->get_path());
            }
            if (mode & binding_mode::one_way_to_source)
            {
                write_bind(stream, b->get_element(), b->get_path(), name, prop);
            }
        }
        return stream;
    }

    ostream& compiler::write_deserialize(ostream& stream, string_view path)
    {
        write_indent(stream) << "::xaml::parser __p(ctx, \"" << path << "\");" << endl;
        write_indent(stream) << "::xaml::deserializer __des{ ctx };" << endl;
        write_indent(stream) << "::xaml::xaml_node __node = __p.parse();" << endl;
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
                write_set_property(stream, node.type, prop.host_type, prop.info->type(), get_node_name(node, is_this), prop.info->name(), get<string>(value));
                break;
            case 2: // xaml_node
            {
                auto& n = get<xaml_node>(value);
                write_construct(stream, n.name, n.type);
                compile_impl(stream, n, this_node, false);
                write_set_property(stream, node.type, prop.host_type, prop.info->type(), get_node_name(node, is_this), prop.info->name(), n.name);
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
                write_add_property(stream, node.type, prop.second.host_type, prop.second.info->type(), get_node_name(node, is_this), prop.second.info->name(), n.name);
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
                deserializer des{ *m_ctx };
                auto ex = des.deserialize(n);
                write_markup(stream, node.name, prop.info->name(), ex);
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

    ostream& compiler::compile(ostream& stream, xaml_node& node, set<string> const& headers)
    {
        if (stream)
        {
            if (node.map_class)
            {
                auto all_headers = headers;
                for (auto& m : m_ctx->get_modules())
                {
                    set<string_view> hs = include_headers(*m.second);
                    all_headers.insert(hs.begin(), hs.end());
                }
                write_includes(stream, all_headers);
                auto [ns, name] = *node.map_class;
                write_init_decl(stream, ns, name);
                write_begin_block(stream);
                compile_impl(stream, node, node, true);
                compile_extensions(stream, node, true);
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

    static array<string_view, 2> fake_headers = {
        "xaml/deserializer.hpp",
        "xaml/parser.hpp"
    };

    ostream& compiler::compile_fake(ostream& stream, xaml_node& node, string_view path)
    {
        if (stream)
        {
            if (node.map_class)
            {
                write_includes(stream, fake_headers);
                auto [ns, name] = *node.map_class;
                write_init_decl_with_meta(stream, ns, name);
                write_begin_block(stream);
                write_deserialize(stream, path);
                write_type(write_indent(stream) << "__des.deserialize(__node, ::std::static_pointer_cast<", ns, name) << ">(shared_from_this()));" << endl;
                write_end_block(stream);
            }
            else
            {
                write_construct(stream, node.name, node.type);
                write_deserialize(stream, path);
                write_indent(stream) << "auto wnd = __des.deserialize(__node);" << endl;
            }
        }
        return stream;
    }
} // namespace xaml
