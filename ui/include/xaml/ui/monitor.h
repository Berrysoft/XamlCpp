#ifndef XAML_UI_SCREEN_H
#define XAML_UI_SCREEN_H

#include <vector>
#include <xaml/ui/drawing.h>
#include <xaml/utility.h>
#include <xaml/vector.h>

typedef struct xaml_monitor xaml_monitor;

struct xaml_monitor
{
    xaml_rectangle region;
    xaml_rectangle client;
};

XAML_TYPE(xaml_monitor, { 0xc81625a5, 0xb58f, 0x403e, { 0x92, 0xbb, 0x1a, 0xe2, 0xdf, 0x12, 0x4d, 0xca } })

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_monitor_get_all(xaml_vector_view**) XAML_NOEXCEPT;

#endif // !XAML_UI_SCREEN_H
