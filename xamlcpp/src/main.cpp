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

using namespace std;
using namespace std::filesystem;
using namespace xaml;
using namespace boost::program_options;

optional<version> is_later(map<path_string_t, tuple<path_string_t, version>> const& modules, path const& new_module)
{
    try
    {
        module m;
        m.open(new_module.native());
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

int main(int argc, char const* const* argv)
{
    try
    {
        options_description desc{ "Allowed options" };
        desc.add_options()(
            "help,h", "Print help message")(
            "input-file,i", value<string>(), "Input XAML file")(
            "output-file,o", value<string>(), "Output C++ file")(
            "library-path,L", value<vector<string>>(), "Search library path")(
            "fake,f", "Generate deserialize code")(
            "verbose,v", "Show detailed output")(
            "no-logo", "Cancellation to show copyright infomation");
        positional_options_description p;
        p.add("input-file", -1);

        variables_map vm;
        store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        notify(vm);

        path exe{ argv[0] };
        if (!vm.count("no-logo"))
        {
            cout << exe.filename().string() << " " XAML_VERSION << '\n'
                 << "Copyright (c) 2019-2020 Berrysoft" << '\n'
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
            string inf = vm["input-file"].as<string>();
            path ouf_path = vm.count("output-file") ? vm["output-file"].as<string>() : inf + ".g.cpp";
            map<path_string_t, tuple<path_string_t, version>> modules;
            auto lib_dirs = vm.count("library-path") ? vm["library-path"].as<vector<string>>() : vector<string>{ exe.parent_path().string() };
            for (path dir : lib_dirs)
            {
                if (verbose) cout << "Searching " << dir << "..." << endl;
                for (auto& en : directory_iterator{ dir })
                {
                    if (en.is_regular_file())
                    {
                        auto p = en.path();
                        if (verbose) cout << "Determining " << p << "..." << endl;
                        if (p.has_extension() && p.extension().native() == module_extension)
                        {
                            if (auto pver = is_later(modules, p))
                            {
                                if (verbose) cout << "Select " << p.filename() << '(' << *pver << ')' << " at " << p << endl;
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
                cout << "Registered types:" << endl;
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
                ofstream stream{ ouf_path.string() };
                if (vm.count("fake"))
                {
                    if (verbose) cout << "Compiling fake to " << path{ inf } << "..." << endl;
                    c.compile_fake(stream, node, inf);
                }
                else
                {
                    if (verbose) cout << "Compiling to " << path{ inf } << "..." << endl;
                    c.compile(stream, node, inf, p.get_headers());
                }
            }
            else
            {
                cout << "Cannot open " << inf << endl;
                return 1;
            }
        }
        else
        {
            cout << "Input file must be specified" << endl;
            return 1;
        }
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }
}
