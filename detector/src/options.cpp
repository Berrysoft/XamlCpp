#include <options.h>
#include <options_base.hpp>
#include <xaml/cmdline/option.h>

struct xaml_detector_options_impl : xaml_cmdline_options_base_implement<xaml_detector_options_impl, xaml_detector_options>
{
    XAML_PROP_IMPL(recursive, bool, bool*, bool)
    XAML_PROP_PTR_IMPL(path, xaml_string)
};

xaml_result XAML_CALL xaml_detector_options_new(xaml_detector_options** ptr) noexcept
{
    return xaml_object_new<xaml_detector_options_impl>(ptr);
}

xaml_result XAML_CALL xaml_detector_options_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_detector_options, "options.h");
    XAML_TYPE_INFO_ADD_CTOR(xaml_detector_options_new);
    XAML_RETURN_IF_FAILED(xaml_cmdline_options_base_members(__info.get()));
    XAML_TYPE_INFO_ADD_PROP(recursive, bool);
    XAML_TYPE_INFO_ADD_PROP(path, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(path);
    xaml_ptr<xaml_cmdline_option> opt;
    XAML_RETURN_IF_FAILED(xaml_cmdline_option_new(&opt));
    XAML_RETURN_IF_FAILED(opt->add_arg(U('h'), U("help"), U("help"), U("Print help message")));
    XAML_RETURN_IF_FAILED(opt->add_arg(0, U("version"), U("version"), U("Print version info")));
#ifndef NDEBUG
    XAML_RETURN_IF_FAILED(opt->add_arg(U('d'), U("debug"), U("debug"), U("Show debug output")));
#endif // !NDEBUG
    XAML_RETURN_IF_FAILED(opt->add_arg(U('r'), U("recursive"), U("recursive"), U("Load modules recursively")));
    XAML_RETURN_IF_FAILED(opt->add_arg(0, U("no-logo"), U("no_logo"), U("Cancellation to show copyright infomation")));
    XAML_RETURN_IF_FAILED(opt->add_arg(0, {}, U("path"), U("Library path")));
    XAML_RETURN_IF_FAILED(__info->add_attribute(opt.get()));
    return ctx->add_type(__info.get());
}
