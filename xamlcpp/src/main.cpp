#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <xaml/compiler.hpp>
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
using namespace boost::program_options;

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
        options_description desc{ "Allowed options" };
        desc.add_options()(
            "help,h", "Print help message")(
            "input-file,i", _tvalue<string_t>(), "Input XAML file")(
            "output-file,o", _tvalue<string_t>(), "Output C++ file")(
            "library-path,L", _tvalue<vector<string_t>>(), "Search library path")(
            "fake,f", bool_switch(), "Generate deserialize code")(
            "verbose,v", "Show detailed output")(
            "no-logo", "Cancellation to show copyright infomation");
        positional_options_description p;
        p.add("input-file", -1);

        variables_map vm;
        store(basic_command_line_parser<char_t>(argc, argv).options(desc).positional(p).run(), vm);
        notify(vm);

        path exe{ argv[0] };
        if (!vm.count("no-logo"))
        {
            _tcout << exe.filename().string<char_t>() << U(" ") XAML_VERSION << U('\n')
                   << U("Copyright (c) 2019-2020 Berrysoft") << U('\n')
                   << endl;
        }

        if (vm.empty() || vm.count("help"))
        {
            cout << desc << endl;
            return 1;
        }

        bool verbose = vm.count("verbose");

        if (vm.count("input-file"))
        {
            path inf = vm["input-file"].as<string_t>();
            path ouf_path = vm.count("output-file") ? vm["output-file"].as<string_t>() : inf.string<char_t>() + U(".g.cpp");
            map<path, tuple<path, version>> modules;
            auto lib_dirs = vm.count("library-path") ? vm["library-path"].as<vector<string_t>>() : vector<string_t>{ exe.parent_path().string<char_t>() };
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
            parser p{ ctx, inf };
            if (p.is_open())
            {
                auto node = p.parse();
                compiler c{ ctx };
                ofstream stream{ ouf_path };
                if (vm.count("fake") && vm["fake"].as<bool>())
                {
                    if (verbose) _tcout << U("Compiling fake to ") << inf << U("...") << endl;
                    c.compile_fake(stream, node, inf);
                }
                else
                {
                    if (verbose) _tcout << U("Compiling to ") << inf << U("...") << endl;
                    c.compile(stream, node, inf, p.get_headers());
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
