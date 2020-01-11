#include <winrt/base.h>
#include <xaml/ui/application.hpp>

using namespace winrt;

namespace xaml
{
    XAML_API application::~application() {}

    XAML_API void application::init_components()
    {
        init_apartment();
    }

    XAML_API int application::run()
    {

        return 0;
    }
} // namespace xaml
