#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <options.h>
#include <sstream>

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

void compile(ostream& stream, xaml_ptr<xaml_vector_view> const& inputs)
{
    stream << "#include <xaml/resource/resource.h>" << endl;
    stream << "#include <xaml/string.h>" << endl;

    size_t index{ 0 };
    map<path, string> rc_map{};

    for (auto item : inputs)
    {
        path file = to_string_view_t(item.query<xaml_string>());
        string name = get_valid_name(file.string(), index++);
        rc_map.emplace(file, name);
        ifstream input{ file };
        stream << "inline constexpr char " << name << "[] = " << endl;
        string line;
        while (getline(input, line))
        {
            stream << quoted(line) << "\"\\n\"" << endl;
        }
    }
    stream << ';' << endl;

    stream << "xaml_result XAML_CALL xaml_resource_get(xaml_string* path, xaml_buffer** ptr) noexcept" << endl;
    stream << '{' << endl;

    stream << tab << "xaml_std_string_view_t file;" << endl;
    stream << tab << "XAML_RETURN_IF_FAILED(to_string_view_t(path, &file));" << endl;
    stream << tab << "xaml_ptr<xaml_buffer> buffer;" << endl;

    size_t i = 0;
    for (auto pair : rc_map)
    {
        stream << tab << (i ? "else if (file == U(" : "if (file == U(") << quoted(pair.first.string()) << "))" << endl;
        stream << tab << '{' << endl;

        stream << tab << tab << "XAML_RETURN_IF_FAILED(xaml_buffer_new_reference(reinterpret_cast<std::uint8_t*>(const_cast<char*>(" << pair.second << ")), (std::int32_t)sizeof(" << pair.second << "), &buffer));";

        stream << tab << '}' << endl;
    }

    stream << tab << "else" << endl;
    stream << tab << '{' << endl;
    stream << tab << tab << "return XAML_E_KEYNOTFOUND;" << endl;
    stream << tab << '}' << endl;

    stream << tab << "return buffer->query(ptr);";

    stream << '}' << endl;
}

int _tmain(int argc, xaml_char_t** argv)
{
    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_rc_options_register(cmdline_ctx.get()));
    xaml_ptr<xaml_rc_options> options;
    XAML_THROW_IF_FAILED(xaml_cmdline_parse_and_print(cmdline_ctx.get(), argc, argv, &options));

    xaml_ptr<xaml_vector_view> inputs;
    XAML_THROW_IF_FAILED(options->get_inputs(&inputs));

    xaml_ptr<xaml_string> output;
    XAML_THROW_IF_FAILED(options->get_output(&output));

    if (output)
    {
        ofstream stream{ to_string_t(output) };
        compile(stream, inputs);
    }
    else
    {
        compile(cout, inputs);
    }

    return 0;
}
