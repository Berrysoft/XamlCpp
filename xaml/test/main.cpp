#include <iostream>
#include <xaml/deserialize.hpp>
#include <xaml/ui/meta.hpp>
#include <xaml/ui/msgbox.hpp>

using namespace std;
using namespace xaml;

class test_window : public window, public meta_class_impl<test_window>
{
public:
    test_window() : window() {}

    ~test_window() override {}

    void init_components()
    {
        deserializer des("test.xaml");
        des.deserialize(static_pointer_cast<test_window>(shared_from_this()));
    }

    void on_button_click(button_base const&)
    {
        msgbox(static_pointer_cast<window>(control::shared_from_this()), U("Hello world!"), U("Hello"));
    }

    static void register_class() noexcept
    {
        REGISTER_TYPE(, test_window);
        ADD_CTOR_DEF();
        ADD_WINDOW_MEMBERS();
        ADD_METHOD(on_button_click);
    }
};

#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
    init_meta();
    register_class<test_window>();
    try
    {
#ifdef XAML_UI_WINDOWS
        auto app = application::init(lpCmdLine);
#else
        auto app = application::init(argc, argv);
#endif // XAML_UI_WINDOWS
        auto wnd = make_shared<test_window>();
        wnd->init_components();
        wnd->show();
        return app->run();
    }
    catch (exception& ex)
    {
        msgbox(__value_converter_traits<string_view_t>::convert(ex.what()), U("XAML Parser Error"), msgbox_style::error);
    }
}
