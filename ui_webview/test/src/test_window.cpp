#include <test_window.h>
#include <xaml/ui/application.h>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/controls/entry.h>
#include <xaml/ui/controls/grid.h>
#include <xaml/ui/controls/webview.h>

using namespace std;

struct xaml_test_window_impl : xaml_implement<xaml_test_window_impl, xaml_test_window, xaml_object>
{
    xaml_ptr<xaml_window> m_window{};

    xaml_result XAML_CALL init() noexcept;
    xaml_result XAML_CALL show() noexcept override
    {
        return m_window->show();
    }
};

xaml_result xaml_test_window_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_window_new(&m_window));
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Test window"), &title));
    XAML_RETURN_IF_FAILED(m_window->set_title(title.get()));
    XAML_RETURN_IF_FAILED(m_window->set_location({ 100, 100 }));
    XAML_RETURN_IF_FAILED(m_window->set_size({ 800, 600 }));

    xaml_ptr<xaml_grid> g;
    XAML_RETURN_IF_FAILED(xaml_grid_new(&g));
    XAML_RETURN_IF_FAILED(g->add_column({ 1, xaml_grid_layout_star }));
    XAML_RETURN_IF_FAILED(g->add_column({ 50, xaml_grid_layout_abs }));
    XAML_RETURN_IF_FAILED(g->add_row({ 0, xaml_grid_layout_auto }));
    XAML_RETURN_IF_FAILED(g->add_row({ 1, xaml_grid_layout_star }));
    XAML_RETURN_IF_FAILED(m_window->set_child(g.get()));

    xaml_ptr<xaml_entry> abar;
    XAML_RETURN_IF_FAILED(xaml_entry_new(&abar));
    XAML_RETURN_IF_FAILED(g->add_child(abar.get()));

    xaml_ptr<xaml_button> abutton;
    XAML_RETURN_IF_FAILED(xaml_button_new(&abutton));
    {
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Go"), &text));
        XAML_RETURN_IF_FAILED(abutton->set_text(text.get()));
    }
    XAML_RETURN_IF_FAILED(g->add_child(abutton.get()));
    XAML_RETURN_IF_FAILED(xaml_grid_set_column(abutton.get(), 1));

    xaml_ptr<xaml_webview> view;
    XAML_RETURN_IF_FAILED(xaml_webview_new(&view));

    xaml_ptr<xaml_string> uri_str;
    {
        xaml_ptr<xaml_application> current_app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
        xaml_ptr<xaml_vector_view> argv;
        XAML_RETURN_IF_FAILED(current_app->get_cmd_lines(&argv));
        int32_t size;
        XAML_RETURN_IF_FAILED(argv->get_size(&size));
        if (size > 1)
        {
            xaml_ptr<xaml_object> item;
            XAML_RETURN_IF_FAILED(argv->get_at(1, &item));
            XAML_RETURN_IF_FAILED(item->query(&uri_str));
        }
        else
        {
            XAML_RETURN_IF_FAILED(xaml_string_new(U("https://www.bing.com/"), &uri_str));
        }
        XAML_RETURN_IF_FAILED(view->set_uri(uri_str.get()));
    }
    XAML_RETURN_IF_FAILED(g->add_child(view.get()));
    XAML_RETURN_IF_FAILED(xaml_grid_set_row(view.get(), 1));
    XAML_RETURN_IF_FAILED(xaml_grid_set_column_span(view.get(), 2));

    XAML_RETURN_IF_FAILED(abar->set_text(uri_str.get()));

    {
        xaml_ptr<xaml_delegate> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void, xaml_ptr<xaml_webview>, xaml_ptr<xaml_string>>(
            [abar](xaml_ptr<xaml_webview>, xaml_ptr<xaml_string> uri) -> xaml_result {
                return abar->set_text(uri.get());
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(view->add_uri_changed(callback.get(), &token));
    }
    {
        xaml_ptr<xaml_delegate> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void, xaml_ptr<xaml_button>>(
            [abar, view](xaml_ptr<xaml_button>) -> xaml_result {
                xaml_ptr<xaml_string> uri;
                XAML_RETURN_IF_FAILED(abar->get_text(&uri));
                return view->set_uri(uri.get());
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(abutton->add_click(callback.get(), &token));
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_test_window_new(xaml_test_window** ptr) noexcept
{
    return xaml_object_init<xaml_test_window_impl>(ptr);
}
