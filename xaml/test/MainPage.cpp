#include "pch.h"

#include "MainPage.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

#include <xaml/parser.hpp>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace xaml;
using namespace xaml::test;

namespace winrt::xaml_test::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
        init_parser();
        register_class<test_window>();
        Loaded([this](IInspectable const&, RoutedEventArgs const&) -> void {
            wnd = make_shared<test_window>();
            wnd->init_components();
            wnd->show(*this);
            UpdateLayout();
        });
    }
} // namespace winrt::xaml_test::implementation
