#include <sstream>
#include <xaml/cmdline/parser.hpp>

using namespace std;

namespace xaml::cmdline
{
    static string get_invalid_option_message(string_view_t opt)
    {
        ostringstream stream;
        stream << "Invalid option: " << quoted(value_converter_traits<string_view>::convert(opt));
        return stream.str();
    }

    invalid_option::invalid_option(string_view_t opt) : logic_error(get_invalid_option_message(opt)) {}

    vector<option> parse(reflection_info const* refl, array_view<string_t> args)
    {
        vector<option> options;
        for (size_t i = 1; i < args.size(); i++)
        {
            string_view_t arg = args[i];
            if (arg.empty())
                continue;
            else if (arg[0] == '-')
            {
                if (arg.size() == 1)
                    throw invalid_option{ arg };
                else if (arg[1] == '-')
                {
                    if (arg.size() == 2) throw invalid_option{ arg };
                    string_view_t long_arg = arg.substr(2);
                    // TODO: long args
                }
                else
                {
                    char_t short_arg = arg[1];
                    string_view_t switches_or_value = arg.substr(2);
                    // TODO: short args
                }
            }
            else
            {
                // TODO: default args
            }
        }
        return options;
    }
} // namespace xaml::cmdline
