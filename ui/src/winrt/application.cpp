#include "winrt/Windows.UI.Xaml.h"
#include <xaml/ui/application.hpp>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    application::~application()
    {
        cleanup_modules();
    }

    void application::init_components() {}

    int application::run() { return 0; }
} // namespace xaml
