#ifndef XAMLD_OPTIONS_H
#define XAMLD_OPTIONS_H

#include <xaml/cmdline/option.h>

XAML_CLASS(xaml_detector_options, { 0xd1e430b3, 0xb91f, 0x4cba, { 0x87, 0x81, 0xda, 0x1d, 0x48, 0x59, 0xef, 0x7e } })

#define XAML_DETECTOR_OPTIONS_VTBL(type) XAML_VTBL_INHERIT(XAML_CMDLINE_OPTION_VTBL(type))

#endif // !XAMLD_OPTIONS_H
