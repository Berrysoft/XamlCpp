#pragma once

#include "MainPage.g.h"
#include <memory>
#include <test_window.hpp>

namespace winrt::xaml_test::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

    private:
        std::shared_ptr<xaml::test::test_window> wnd{ nullptr };
    };
} // namespace winrt::xaml_test::implementation

namespace winrt::xaml_test::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
} // namespace winrt::xaml_test::factory_implementation
