#ifndef XAML_CMDLINE_HELPER_OPTIONS_BASE_HPP
#define XAML_CMDLINE_HELPER_OPTIONS_BASE_HPP

#include <xaml/cmdline/options_base.h>

template <typename T, typename Base>
struct xaml_cmdline_options_base_implement : xaml_implement<T, Base>
{
    XAML_PROP_IMPL(help, bool, bool*, bool)
    XAML_PROP_IMPL(verbose, bool, bool*, bool)
    XAML_PROP_IMPL(version, bool, bool*, bool)
    XAML_PROP_IMPL(no_logo, bool, bool*, bool)
};

#endif // !XAML_CMDLINE_HELPER_OPTIONS_BASE_HPP
