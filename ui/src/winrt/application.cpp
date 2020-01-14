#include "winrt/Windows.UI.Xaml.h"
#include <xaml/ui/application.hpp>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    XAML_API application::~application() {}

    XAML_API void application::init_components() {}

    XAML_API int application::run() { return 0; }
} // namespace xaml
