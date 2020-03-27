#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <xaml/cmdline/deserializer.hpp>
#include <xaml/cmdline/option.hpp>
#include <xaml/compiler.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/parser.hpp>
#include <xaml/version.hpp>

#ifdef UNICODE
#ifndef _tmain
#define _tmain wmain
#endif // !_tmain

#define _tcout wcout
#define _tvalue wvalue
#else
#ifndef _tmain
#define _tmain main
#endif // !_tmain

#define _tcout cout
#define _tvalue value
#endif // UNICODE

using namespace std;
using namespace std::filesystem;
using namespace xaml;

namespace xamlcpp
{
    struct xamlcpp_options;
}

namespace xaml
{
    template <>
    struct type_guid<xamlcpp::xamlcpp_options>
    {
        static constexpr guid value{ 0xd1e430b3, 0xb91f, 0x4cba, 0x87, 0x81, 0xda, 0x1d, 0x48, 0x59, 0xef, 0x7e };
    };
} // namespace xaml

namespace xamlcpp
{
    struct xamlcpp_options : public meta_class
    {
        META_CLASS_IMPL(meta_class)

        xamlcpp_options() : meta_class() {}
        ~xamlcpp_options() override {}

        PROP_CONSTEXPR(help, bool)
        PROP_CONSTEXPR(fake, bool)
        PROP_CONSTEXPR(verbose, bool)
        PROP_CONSTEXPR(no_logo, bool)
        PROP_STRING(input)
        PROP_STRING(output)

    private:
        vector<string_t> m_lib_paths{};

    public:
        void add_lib_path(string_view_t p) { m_lib_paths.emplace_back(p); }
        void remove_lib_path(string_view_t p) {}
        array_view<string_t> get_lib_paths() const noexcept { return m_lib_paths; }

        REGISTER_CLASS_DECL_NOFILE(xamlcpp, xamlcpp_options)
        {
            ADD_CTOR_DEF();

            ADD_PROP(help);
            ADD_PROP(fake);
            ADD_PROP(verbose);
            ADD_PROP(no_logo);
            ADD_PROP(input);
            ADD_PROP(output);
            ADD_COLLECTION_PROP(lib_path, string_view_t);

            auto attr = make_unique<cmdline::option>();
            attr->add_arg(U('h'), U("help"), "help");
            attr->add_arg(U('f'), U("fake"), "fake");
            attr->add_arg(U('v'), U("verbose"), "verbose");
            attr->add_arg(nullopt, U("no-logo"), "no_logo");
            attr->add_arg(U('i'), U("input-file"), "input");
            attr->add_arg(nullopt, nullopt, "input");
            attr->add_arg(U('o'), U("output-file"), "output");
            attr->add_arg(U('L'), U("library-path"), "lib_path");
            ref->set_attribute(move(attr));
        }
        REGISTER_CLASS_END()
    };
} // namespace xamlcpp

optional<version> is_later(map<path, tuple<path, version>> const& modules, path const& new_module)
{
    try
    {
        module m{ new_module.native() };
        auto pxaml_version = (void (*)(version*) noexcept)m.get_method("xaml_version");
        if (pxaml_version)
        {
            auto it = modules.find(new_module.filename());
            version ver;
            pxaml_version(&ver);
            if (it == modules.end())
                return ver;
            else if (ver > get<1>(it->second))
                return ver;
            else
                return nullopt;
        }
        else
        {
            return nullopt;
        }
    }
    catch (system_error const&)
    {
        return nullopt;
    }
}

int _tmain(int argc, char_t const* const* argv)
{
    try
    {
        meta_context cmdline_ctx{};
        cmdline::init_parser(cmdline_ctx);
        register_class<xamlcpp::xamlcpp_options>(cmdline_ctx);
        auto refl = cmdline_ctx.get_type(type_guid_v<xamlcpp::xamlcpp_options>);
        auto nodes = cmdline::parse(refl, argc, argv);
        auto opts = static_pointer_cast<xamlcpp::xamlcpp_options>(cmdline::deserialize(refl, nodes));

        path exe{ argv[0] };
        if (!opts->get_no_logo())
        {
            _tcout << exe.filename().string<char_t>() << U(" ") XAML_VERSION << U('\n')
                   << U("Copyright (c) 2019-2020 Berrysoft") << U('\n')
                   << endl;
        }

        if (opts->get_help())
        {
            return 1;
        }

        bool verbose = opts->get_verbose();

        path inf = opts->get_input();
        if (!inf.empty())
        {
            path ouf_path = opts->get_output();
            if (ouf_path.empty()) ouf_path = inf.string<char_t>() + U(".g.cpp");
            map<path, tuple<path, version>> modules;
            vector<string_t> lib_dirs = opts->get_lib_paths();
            for (path dir : lib_dirs)
            {
                if (verbose) _tcout << U("Searching ") << dir << U("...") << endl;
                for (auto& en : directory_iterator{ dir })
                {
                    if (en.is_regular_file())
                    {
                        auto p = en.path();
                        if (verbose) _tcout << U("Determining ") << p << U("...") << endl;
                        if (p.has_extension() && p.extension().native() == module_extension)
                        {
                            if (auto pver = is_later(modules, p))
                            {
                                if (verbose) _tcout << U("Select ") << p.filename() << U('(') << *pver << U(") at ") << p << endl;
                                modules.emplace(p.filename(), make_tuple(p, *pver));
                            }
                        }
                    }
                }
            }
            meta_context ctx{};
            for (auto& m : modules)
            {
                ctx.add_module(get<0>(m.second));
            }
            init_parser(ctx);
            if (verbose)
            {
                _tcout << U("Registered types:") << endl;
                for (auto& pair : ctx.get_types())
                {
                    auto& ns = pair.first;
                    for (auto& p : pair.second)
                    {
                        auto& name = p.first;
                        cout << ns << "::" << name << endl;
                    }
                }
            }
            auto [opened, node, headers] = parse_file(ctx, inf);
            if (opened)
            {
                compiler c{ ctx };
                ofstream stream{ ouf_path };
                if (opts->get_fake())
                {
                    if (verbose) _tcout << U("Compiling fake to ") << inf << U("...") << endl;
                    c.compile_fake(stream, node, inf);
                }
                else
                {
                    if (verbose) _tcout << U("Compiling to ") << inf << U("...") << endl;
                    c.compile(stream, node, inf, headers);
                }
            }
            else
            {
                _tcout << U("Cannot open ") << inf << endl;
                return 1;
            }
        }
        else
        {
            _tcout << U("Input file must be specified") << endl;
            return 1;
        }
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}
