#ifdef XAML_USE_BOOST_NOWIDE
#include <ios>

#include <boost/nowide/args.hpp>
#include <boost/nowide/fstream.hpp>
#include <boost/nowide/iostream.hpp>
#else
#include <iostream>
#endif // XAML_USE_BOOST_NOWIDE

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <options.h>
#include <sf/format.hpp>
#include <sstream>
#include <tuple>

#ifdef XAML_USE_BOOST_NOWIDE
using u8ofstream = boost::nowide::ofstream;
using u8ifstream = boost::nowide::ifstream;

#define u8cout ::boost::nowide::cout
#else
using u8ofstream = std::ofstream;
using u8ifstream = std::ifstream;

#define u8cout ::std::cout
#endif // XAML_WIN32

#ifdef UNICODE
#define _tmain wmain
#define _tcout ::std::wcout
#else
#define _tmain main
#define _tcout ::std::cout
#endif // UNICODE

using namespace std;
using namespace std::filesystem;

string get_valid_name(string_view str, size_t index)
{
    ostringstream stream;
    stream << "__";
    for (char c : str)
    {
        if (isdigit(c) || isalpha(c))
            stream << c;
        else
            stream << '_';
    }
    stream << "__" << index;
    return stream.str();
}

constexpr string_view tab = "    ";
constexpr string_view text_extensions[] = { ".txt", ".xml", ".xaml", ".md" };

void compile(ostream& stream, xaml_ptr<xaml_vector_view> const& inputs)
{
    sf::println(stream, "#include <xaml/resource/resource.h>");

    size_t index{ 0 };
    map<path, tuple<string, bool>> rc_map{};

    for (auto item : inputs)
    {
        path file = to_string_view(item.query<xaml_string>());
#ifndef XAML_APPLE
        if (!rc_map.contains(file))
#else
        auto it = rc_map.find(file);
        if (it == rc_map.end())
#endif // !XAML_APPLE
        {
            auto it = find(begin(text_extensions), end(text_extensions), file.extension());
            bool text = it != end(text_extensions);
            if (text)
            {
                u8ifstream input{ file };
                if (input.is_open())
                {
                    string name = get_valid_name(file.string(), index++);
                    rc_map.emplace(file.relative_path(), make_tuple(name, text));
                    sf::println(stream, "inline static constexpr char8_t {}[] = ", name);
                    sf::print(stream, "u8");
                    string line;
                    while (getline(input, line))
                    {
                        sf::print(stream, quoted(line));
                        if (!input.eof())
                        {
                            sf::println(stream, "\"\\n\"");
                        }
                    }
                    sf::println(stream, ';');
                }
            }
            else
            {
                ifstream input{ file, ios_base::binary };
                if (input.is_open())
                {
                    string name = get_valid_name(file.string(), index++);
                    rc_map.emplace(file.relative_path(), make_tuple(name, text));
                    sf::print(stream, "inline static constexpr ::std::uint8_t {}[] = {{", name);
                    size_t i = 0;
                    while (input.peek() != char_traits<char>::eof())
                    {
                        i %= 16;
                        if (i++ == 0) sf::println(stream);
                        int b = input.get();
                        sf::print(stream, "0x{:x}", b);
                        if (input.peek() != char_traits<char>::eof()) sf::print(stream, ", ");
                    }
                    sf::println(stream, "};");
                }
            }
        }
    }

    sf::println(stream, "xaml_result XAML_CALL xaml_resource_get(xaml_string* path, std::uint8_t const** pdata, std::int32_t* psize) noexcept\n{");

    sf::println(stream, "{}std::string_view file;", tab);
    sf::println(stream, "{}XAML_RETURN_IF_FAILED(to_string_view(path, &file));", tab);

    size_t i = 0;
    for (auto pair : rc_map)
    {
        sf::println(stream, "{}{}if (file == U({}))", tab, i++ ? "else " : "", pair.first);
        sf::println(stream, "{}{{", tab);

        auto& [name, text] = pair.second;
        if (text)
        {
            sf::println(stream, "{0}{0}*pdata = reinterpret_cast<std::uint8_t const*>({});", tab, name);
        }
        else
        {
            sf::println(stream, "{0}{0}*pdata = {};", tab, name);
        }
        sf::println(stream, "{0}{0}*psize = static_cast<std::int32_t>(sizeof({}));", tab, name);

        sf::println(stream, "{}}}", tab);
    }

    if (!rc_map.empty()) sf::println(stream, "{}else", tab);
    sf::println(stream, "{}{{", tab);
    sf::println(stream, "{0}{0}return XAML_E_KEYNOTFOUND;", tab);
    sf::println(stream, "{}}}", tab);

    sf::println(stream, "{}return XAML_S_OK;", tab);

    sf::println(stream, '}');
}

int _tmain(int argc, char** argv)
{
#if defined(XAML_USE_BOOST_NOWIDE) && !defined(UNICODE)
    boost::nowide::args _(argc, argv);
#endif // XAML_USE_BOOST_NOWIDE && !UNICODE

    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_rc_options_register(cmdline_ctx));
    xaml_ptr<xaml_rc_options> options;
    XAML_THROW_IF_FAILED(xaml_cmdline_parse_and_print(cmdline_ctx, argc, argv, &options));

    xaml_ptr<xaml_vector_view> inputs;
    XAML_THROW_IF_FAILED(options->get_inputs(&inputs));

    xaml_ptr<xaml_string> output;
    XAML_THROW_IF_FAILED(options->get_output(&output));

    if (output)
    {
        u8ofstream stream{ to_string(output) };
        compile(stream, inputs);
    }
    else
    {
        compile(u8cout, inputs);
    }

    return 0;
}
