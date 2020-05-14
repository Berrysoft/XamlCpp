#include <filesystem>
#include <iostream>
#include <options.h>
#include <xaml/cmdline/deserializer.h>
#include <xaml/cmdline/option.h>
#include <xaml/result_handler.h>

#ifdef UNICODE
#define _tmain wmain
#define _tcout ::std::wcout
#define _tcerr ::std::wcerr
#else
#define _tmain main
#define _tcout ::std::cout
#define _tcerr ::std::cerr
#endif // UNICODE

using namespace std;
using namespace std::filesystem;

int _tmain(int argc, xaml_char_t** argv)
{
    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_rc_options_register(cmdline_ctx.get()));
    xaml_ptr<xaml_rc_options> options;
    XAML_THROW_IF_FAILED(xaml_cmdline_deserialize(cmdline_ctx.get(), argc, argv, &options));

    bool verbose;
    XAML_THROW_IF_FAILED(options->get_verbose(&verbose));

    if (verbose)
    {
        int32_t token;
        XAML_THROW_IF_FAILED(xaml_result_handler_add(
            [](xaml_result hr, xaml_char_t const* msg) {
                _tcerr << U("0x") << hex << hr << U(": ") << msg << endl;
            },
            &token));
    }

    path exe{ argv[0] };
    bool no_logo;
    XAML_THROW_IF_FAILED(options->get_no_logo(&no_logo));
    if (!no_logo)
    {
        _tcout << exe.filename().string<xaml_char_t>() << U(" ") XAML_VERSION;
#ifdef XAML_COMMIT_HASH
        constexpr xaml_std_string_view_t hash{ U("") XAML_COMMIT_HASH };
        _tcout << U('-') << hash.substr(0, 8);
#endif // XAML_COMMIT_HASH
        _tcout << U("\nCopyright (c) 2019-2020 Berrysoft\n") << endl;
    }

    bool help;
    XAML_THROW_IF_FAILED(options->get_help(&help));
    if (help || argc <= 1)
    {
        xaml_ptr<xaml_reflection_info> info;
        XAML_THROW_IF_FAILED(cmdline_ctx->get_type<xaml_rc_options>(&info));
        xaml_ptr<xaml_type_info> t;
        XAML_THROW_IF_FAILED(info->query(&t));
        xaml_ptr<xaml_cmdline_option> opt;
        XAML_THROW_IF_FAILED(t->get_attribute(&opt));
        XAML_THROW_IF_FAILED(xaml_cmdline_option_print(_tcout, opt.get()));
        return 1;
    }

    return 0;
}
