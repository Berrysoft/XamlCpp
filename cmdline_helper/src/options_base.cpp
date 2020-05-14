#include <filesystem>
#include <iostream>
#include <options_base.hpp>
#include <xaml/cmdline/deserializer.h>
#include <xaml/cmdline/option.h>
#include <xaml/cmdline/options_base.h>
#include <xaml/result_handler.h>

#ifdef UNICODE
#define _tcout ::std::wcout
#define _tcerr ::std::wcerr
#else
#define _tcout ::std::cout
#define _tcerr ::std::cerr
#endif // UNICODE

using namespace std;
using namespace std::filesystem;

xaml_result XAML_CALL xaml_cmdline_options_base_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_cmdline_options_base;
    XAML_TYPE_INFO_ADD_PROP(help, bool);
    XAML_TYPE_INFO_ADD_PROP(verbose, bool);
    XAML_TYPE_INFO_ADD_PROP(debug, bool);
    XAML_TYPE_INFO_ADD_PROP(no_logo, bool);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_cmdline_options_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_cmdline_options_base, "xaml/cmdline/options_base.h");
    XAML_RETURN_IF_FAILED(xaml_cmdline_options_base_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result XAML_CALL xaml_cmdline_parse_and_print(xaml_meta_context* ctx, xaml_guid const& id, int argc, xaml_char_t** argv, void** ptr) noexcept
{
    xaml_ptr<xaml_reflection_info> info;
    XAML_RETURN_IF_FAILED(ctx->get_type(id, &info));
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    xaml_ptr<xaml_cmdline_options> opts;
    XAML_RETURN_IF_FAILED(xaml_cmdline_parse_argv(t.get(), argc, argv, &opts));
    xaml_ptr<xaml_object> obj;
    XAML_RETURN_IF_FAILED(xaml_cmdline_deserialize(t.get(), opts.get(), &obj));
    xaml_ptr<xaml_cmdline_options_base> options;
    XAML_RETURN_IF_FAILED(obj->query(&options));

    bool verbose;
    XAML_RETURN_IF_FAILED(options->get_verbose(&verbose));

    if (verbose)
    {
        int32_t token;
        XAML_RETURN_IF_FAILED(xaml_result_handler_add(
            [](xaml_result hr, xaml_char_t const* msg) {
                _tcerr << U("0x") << hex << hr << U(": ") << msg << endl;
            },
            &token));
    }

    path exe{ argv[0] };
    bool no_logo;
    XAML_RETURN_IF_FAILED(options->get_no_logo(&no_logo));
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
    XAML_RETURN_IF_FAILED(options->get_help(&help));
    if (help || argc <= 1)
    {
        xaml_ptr<xaml_cmdline_option> opt;
        XAML_RETURN_IF_FAILED(t->get_attribute(&opt));
        XAML_RETURN_IF_FAILED(xaml_cmdline_option_print(_tcout, opt.get()));
        exit(1);
    }

    return options->query(id, ptr);
}
