#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <xaml/compiler.hpp>
#include <xaml/deserializer.hpp>
#include <xaml/markup/binding.hpp>

using namespace std;
using std::filesystem::path;

namespace xaml
{
    static bool can_compile(module const& m, guid const& type) noexcept
    {
        using can_compile_t = int (*)(void const*) noexcept;
        auto pcc = (can_compile_t)m.get_method("can_compile");
        return pcc ? pcc(&type) : false;
    }

    static string module_compile(module const& m, guid const& type, string_view code) noexcept
    {
        using compile_t = void (*)(void const*, const char*, size_t, void*) noexcept;
        auto pc = (compile_t)m.get_method("compile");
        if (pc)
        {
            string result;
            pc(&type, code.data(), code.length(), &result);
            return result;
        }
        return {};
    }

    static void include_headers(module const& m, set<string>& h)
    {
        using include_headers_t = const char* const* (*)() noexcept;
        auto pih = (include_headers_t)m.get_method("include_headers");
        if (pih)
        {
            const char* const* headers = pih();
            for (; *headers; headers++)
            {
                h.emplace(*headers);
            }
        }
    }

    static array int_types{
        type_guid_v<meta_box<int8_t>>, type_guid_v<meta_box<int16_t>>, type_guid_v<meta_box<int32_t>>, type_guid_v<meta_box<int64_t>>,
        type_guid_v<meta_box<uint8_t>>, type_guid_v<meta_box<uint16_t>>, type_guid_v<meta_box<uint32_t>>, type_guid_v<meta_box<uint64_t>>
    };

    static array float_types{
        type_guid_v<meta_box<float>>, type_guid_v<meta_box<double>>, type_guid_v<meta_box<long double>>
    };

    static array string_types{
        type_guid_v<meta_box<string>>, type_guid_v<meta_box<wstring>>
    };

    template <size_t N>
    static bool is_of_type(guid const& type, array<guid, N> const& arr)
    {
        return find(arr.begin(), arr.end(), type) != arr.end();
    }

    string compiler::xaml_cpp_compile(guid const& type, string_view code)
    {
        if (type == type_guid_v<meta_box<bool>>)
        {
            bool b = value_converter_traits<bool>::convert(box_value(code));
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
            ostringstream stream;
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

    ostream& compiler::write_static_file_content(ostream& stream, path const& p)
    {
        write_indent(stream) << "static constexpr ::std::string_view __view_file = R\"<<<<<<<<<<<<<<<<(";
        ifstream file{ p };
        char buffer[2048];
        while (file)
        {
            file.read(buffer, sizeof(buffer));
            auto count = file.gcount();
            if (!count) break;
            stream.write(buffer, count);
        }
        return stream << ")<<<<<<<<<<<<<<<<\";" << endl;
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

    ostream& compiler::write_set_property(ostream& stream, reflection_info const* node_type, reflection_info const* host_type, guid const& prop_type, string_view name, string_view prop, string_view value)
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

    ostream& compiler::write_add_property(ostream& stream, reflection_info const* node_type, reflection_info const* host_type, guid const& prop_type, string_view name, string_view prop, string_view value)
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
        auto& [ns, n] = *this_node.map_class;
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
            s << "[" << target_name << "](auto, auto value) { " << target_name << "->set_" << target_prop << "(value); }";
            string arg = s.str();
            write_call(stream, source_name, "add_", (std::string)source_prop + "_changed", { arg });
        }
        return stream;
    }

    ostream& compiler::write_markup(ostream& stream, string_view name, string_view prop, shared_ptr<meta_class> markup)
    {
        if (markup->get_type() == type_guid_v<binding>)
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

    ostream& compiler::write_deserialize(ostream& stream, filesystem::path const& path)
    {
        write_indent(stream) << "auto [__opened, __node, __headers] = ::xaml::parse_string(ctx, __view_file);" << endl;
        write_indent(stream) << "::xaml::deserializer __des{ ctx };" << endl;
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

    static inline string get_path_associated_header_path(filesystem::path const& p)
    {
        return p.filename().string() + ".hpp";
    }

    ostream& compiler::compile(ostream& stream, xaml_node& node, filesystem::path const& path, set<string> const& headers)
    {
        if (stream)
        {
            if (node.map_class)
            {
                auto all_headers = headers;
                for (auto& m : m_ctx->get_modules())
                {
                    include_headers(*m.second, all_headers);
                }
                all_headers.emplace(get_path_associated_header_path(path));
                write_includes(stream, all_headers);
                auto& [ns, name] = *node.map_class;
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

    ostream& compiler::compile_fake(ostream& stream, xaml_node& node, path const& p)
    {
        if (stream)
        {
            if (node.map_class)
            {
                write_includes(stream, fake_headers);
                write_include(stream, get_path_associated_header_path(p));
                write_static_file_content(stream, p);
                auto& [ns, name] = *node.map_class;
                write_init_decl_with_meta(stream, ns, name);
                write_begin_block(stream);
                write_deserialize(stream, p);
                write_type(write_indent(stream) << "__des.deserialize(__node, shared_from_this<", ns, name) << ">());" << endl;
                write_end_block(stream);
            }
            else
            {
                write_static_file_content(stream, p);
                write_deserialize(stream, p);
                write_indent(stream) << "auto wnd = __des.deserialize(__node);" << endl;
            }
        }
        return stream;
    }
} // namespace xaml
