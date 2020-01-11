#include "pch.h"
#include <xaml/ui/application.hpp>

using namespace winrt;

namespace xaml
{
    XAML_API void application::init_components()
    {
        init_apartment();
    }
} // namespace xaml
