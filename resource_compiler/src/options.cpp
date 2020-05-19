#include <options.h>
#include <options_base.hpp>
#include <xaml/cmdline/option.h>

struct xaml_rc_options_impl : xaml_cmdline_options_base_implement<xaml_rc_options_impl, xaml_rc_options>
{
    xaml_ptr<xaml_vector> m_inputs;

    xaml_result XAML_CALL add_input(xaml_string* value) noexcept override
    {
        return m_inputs->append(value);
    }

    xaml_result XAML_CALL remove_input(xaml_string* value) noexcept override
    {
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL get_inputs(xaml_vector_view** ptr) noexcept override
    {
        return m_inputs->query(ptr);
    }

    XAML_PROP_PTR_IMPL(output, xaml_string)

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_inputs));
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_rc_options_new(xaml_rc_options** ptr) noexcept
{
    return xaml_object_init<xaml_rc_options_impl>(ptr);
}

xaml_result XAML_CALL xaml_rc_options_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_rc_options, "options.h");
    XAML_TYPE_INFO_ADD_CTOR(xaml_rc_options_new);
    XAML_RETURN_IF_FAILED(xaml_cmdline_options_base_members(__info));
    XAML_TYPE_INFO_ADD_CPROP(input, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(output, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(path);
    xaml_ptr<xaml_cmdline_option> opt;
    XAML_RETURN_IF_FAILED(xaml_cmdline_option_new(&opt));
    XAML_RETURN_IF_FAILED(opt->add_arg(U('h'), U("help"), U("help"), U("Print help message")));
    XAML_RETURN_IF_FAILED(opt->add_arg(0, U("version"), U("version"), U("Print version info")));
    XAML_RETURN_IF_FAILED(opt->add_arg(0, U("no-logo"), U("no_logo"), U("Cancellation to show copyright infomation")));
    XAML_RETURN_IF_FAILED(opt->add_arg(U('o'), U("output"), U("output"), U("Output file")));
    XAML_RETURN_IF_FAILED(opt->add_arg(0, {}, U("input"), U("Input files")));
    XAML_RETURN_IF_FAILED(__info->add_attribute(opt.get()));
    return ctx->add_type(__info);
}
