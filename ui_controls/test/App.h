#pragma once

#include "App.xaml.g.h"
#include <memory>
#include <test_window.hpp>

namespace winrt::ui_test::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);
    };
} // namespace winrt::ui_test::implementation
