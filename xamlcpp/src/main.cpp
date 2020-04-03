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
#ifndef umain
#define umain wmain
#endif // !umain

#ifndef ucout
#define ucout wcout
#endif // !ucout
#else
#ifndef umain
#define umain main
#endif // !umain

#ifndef ucout
#define ucout cout
#endif // !ucout
#endif // UNICODE

using namespace std;
using namespace std::filesystem;
using namespace xaml;

namespace xaml
{
    namespace xamlcpp
    {
        struct xamlcpp_options;
    }

    template <>
    struct type_guid<xamlcpp::xamlcpp_options>
    {
        static constexpr guid value{ 0xd1e430b3, 0xb91f, 0x4cba, { 0x87, 0x81, 0xda, 0x1d, 0x48, 0x59, 0xef, 0x7e } };
    };
} // namespace xaml

namespace xaml::xamlcpp
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

        REGISTER_CLASS_DECL_NOFILE(xaml::xamlcpp, xamlcpp_options)
        {
            ADD_CTOR();

            ADD_PROP(help);
            ADD_PROP(fake);
            ADD_PROP(verbose);
            ADD_PROP(no_logo);
            ADD_PROP(input);
            ADD_PROP(output);
            ADD_COLLECTION_PROP(lib_path, string_view_t);

            ADD_ATTR(
                make_shared<cmdline::option>()
                    ->add_arg(UU('h'), UU("help"), "help", UU("Print help message"))
                    ->add_arg(UU('f'), UU("fake"), "fake", UU("Generate deserialize code"))
                    ->add_arg(UU('v'), UU("verbose"), "verbose", UU("Show detailed output"))
                    ->add_arg(0, UU("no-logo"), "no_logo", UU("Cancellation to show copyright infomation"))
                    ->add_arg(UU('i'), UU("input-file"), "input", UU("Input XAML file"))
                    ->add_arg(0, {}, "input")
                    ->add_arg(UU('o'), UU("output-file"), "output", UU("Output C++ file"))
                    ->add_arg(UU('L'), UU("library-path"), "lib_path", UU("Search library path")));
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml::xamlcpp

optional<version> is_later(map<path, tuple<path, version>> const& modules, path const& new_module)
{
    try
    {
        module m{ new_module };
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

using namespace xaml::xamlcpp;

int umain(int argc, uchar_t const* const* argv)
{
    try
    {
        meta_context cmdline_ctx{};
        cmdline::init_parser(cmdline_ctx);
        register_class<xamlcpp::xamlcpp_options>(cmdline_ctx);
        auto refl = cmdline_ctx.get_type(type_guid_v<xamlcpp_options>);
        auto nodes = cmdline::parse(refl, argc, argv);
        auto opts = cmdline::deserialize(refl, nodes)->query<xamlcpp_options>();

        path exe{ argv[0] };
        if (!opts->get_no_logo())
        {
            ucout << exe.filename().string<uchar_t>() << UU(" ") XAML_VERSION << UU('\n')
                   << UU("Copyright (c) 2019-2020 Berrysoft") << UU('\n')
                   << endl;
        }

        if (opts->get_help() || argc <= 1)
        {
            auto popt = refl->get_attribute<cmdline::option>();
            popt->print_help(ucout);
            return 1;
        }

        bool verbose = opts->get_verbose();

        path inf = opts->get_input();
        if (!inf.empty())
        {
            path ouf_path = opts->get_output();
            if (ouf_path.empty()) ouf_path = inf.string<uchar_t>() + UU(".g.cpp");
            map<path, tuple<path, version>> modules;
            vector<string_t> lib_dirs = opts->get_lib_paths();
            for (path dir : lib_dirs)
            {
                if (verbose) ucout << UU("Searching ") << dir << UU("...") << endl;
                for (auto& en : directory_iterator{ dir })
                {
                    if (en.is_regular_file())
                    {
                        auto& p = en.path();
                        if (verbose) ucout << UU("Determining ") << p << UU("...") << endl;
                        if (p.has_extension() && p.extension() == module_extension)
                        {
                            if (auto pver = is_later(modules, p))
                            {
                                if (verbose) ucout << UU("Select ") << p.filename() << UU('(') << *pver << UU(") at ") << p << endl;
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
                ucout << UU("Registered types:") << endl;
                for (auto& pair : ctx.get_types())
                {
                    auto ns = to_ustring_t(pair.first);
                    for (auto& p : pair.second)
                    {
                        auto name = to_ustring_t(p.first);
                        ucout << ns << UU("::") << name << endl;
                    }
                }
            }
            auto [node, headers] = parse_file(ctx, inf);
            compiler c{ ctx };
            ofstream stream{ ouf_path };
            if (opts->get_fake())
            {
                if (verbose) ucout << UU("Compiling fake to ") << ouf_path << UU("...") << endl;
                c.compile_fake(stream, node, inf);
            }
            else
            {
                if (verbose) ucout << UU("Compiling to ") << ouf_path << UU("...") << endl;
                c.compile(stream, node, inf, headers);
            }
        }
        else
        {
            ucout << UU("Input file must be specified") << endl;
            return 1;
        }
    }
    catch (exception const& e)
    {
        ucout << to_ustring_t(e.what()) << endl;
        return 1;
    }
    return 0;
}
