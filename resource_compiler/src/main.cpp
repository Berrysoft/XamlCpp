#include <fstream>
#include <iomanip>
#include <map>
#include <nowide/args.hpp>
#include <nowide/filesystem.hpp>
#include <nowide/fstream.hpp>
#include <nowide/iostream.hpp>
#include <options.h>
#include <sf/format.hpp>
#include <sstream>
#include <tuple>

using namespace std;
using nowide::filesystem::path;

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

void compile(ostream& stream, xaml_ptr<xaml_vector_view<xaml_string>> const& inputs)
{
    sf::println(stream, "#include <xaml/resource/resource.h>");

    size_t index{ 0 };
    map<path, string> rc_map{};

    for (auto item : xaml_enumerable_wrapper<xaml_string>{ inputs })
    {
        path file = to_string_view(item);
#ifndef XAML_APPLE
        if (!rc_map.contains(file))
#else
        auto it = rc_map.find(file);
        if (it == rc_map.end())
#endif // !XAML_APPLE
        {
            auto it = find(begin(text_extensions), end(text_extensions), file.extension());
            bool text = it != end(text_extensions);
            ios_base::openmode mode = ios_base::in;
            if (!text) mode |= ios_base::binary;
            nowide::ifstream input{ file, mode };
            if (input.is_open())
            {
                string name = get_valid_name(file.string(), index++);
                rc_map.emplace(file.relative_path(), name);
                if (text)
                {
                    sf::println(stream, "inline static constexpr char const {}[] = ", name);
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
                }
                else
                {
                    sf::print(stream, "inline static constexpr ::std::uint8_t const {}[] = {{", name);
                    size_t i = 0;
                    while (input.peek() != char_traits<char>::eof())
                    {
                        i %= 16;
                        if (i++ == 0) sf::println(stream);
                        int b = input.get();
                        sf::print(stream, "{:x,s}", b);
                        if (input.peek() != char_traits<char>::eof()) sf::print(stream, ", ");
                    }
                }
                sf::println(stream, ';');
            }
        }
    }

    sf::println(stream, "xaml_result XAML_CALL xaml_resource_get(xaml_string* path, void const** pdata, std::int32_t* psize) noexcept\n{");

    sf::println(stream, "{}std::string_view file;", tab);
    sf::println(stream, "{}XAML_RETURN_IF_FAILED(to_string_view(path, &file));", tab);

    size_t i = 0;
    for (auto pair : rc_map)
    {
        sf::println(stream, "{}{}if (file == U({}))", tab, i++ ? "else " : "", pair.first);
        sf::println(stream, "{}{{", tab);

        sf::println(stream, "{0}{0}*pdata = reinterpret_cast<void const*>({});", tab, pair.second);
        sf::println(stream, "{0}{0}*psize = static_cast<std::int32_t>(sizeof({}));", tab, pair.second);

        sf::println(stream, "{}}}", tab);
    }

    if (!rc_map.empty()) sf::println(stream, "{}else", tab);
    sf::println(stream, "{}{{", tab);
    sf::println(stream, "{0}{0}return XAML_E_KEYNOTFOUND;", tab);
    sf::println(stream, "{}}}", tab);

    sf::println(stream, "{}return XAML_S_OK;", tab);

    sf::println(stream, '}');
}

int main(int argc, char** argv)
{
    nowide::args _(argc, argv);

    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_rc_options_register(cmdline_ctx));
    xaml_ptr<xaml_rc_options> options;
    XAML_THROW_IF_FAILED(xaml_cmdline_parse_and_print(cmdline_ctx, argc, argv, &options));

    xaml_ptr<xaml_vector_view<xaml_string>> inputs;
    XAML_THROW_IF_FAILED(options->get_inputs(&inputs));

    xaml_ptr<xaml_string> output;
    XAML_THROW_IF_FAILED(options->get_output(&output));

    if (output)
    {
        nowide::ofstream stream{ path(to_string_view(output)) };
        compile(stream, inputs);
    }
    else
    {
        compile(nowide::cout, inputs);
    }

    return 0;
}
