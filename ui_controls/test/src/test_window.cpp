#include <algorithm>
#include <cmath>
#include <test_window.h>
#include <xaml/ui/application.h>
#include <xaml/ui/colors.h>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/controls/combo_box.h>
#include <xaml/ui/controls/entry.h>
#include <xaml/ui/controls/grid.h>
#include <xaml/ui/controls/label.h>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/controls/password_entry.h>
#include <xaml/ui/controls/progress.h>
#include <xaml/ui/controls/stack_panel.h>
#include <xaml/ui/filebox.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/msgbox.h>
#include <xaml/ui/timer.h>

#if __has_include(<numbers>)
#include <numbers>
using std::numbers::pi;
#else
inline constexpr double pi = M_PI;
#endif // __has_include(<numbers>)

using namespace std;

struct xaml_test_window_impl : xaml_implement<xaml_test_window_impl, xaml_test_window, xaml_object>
{
    xaml_ptr<xaml_window> m_window{};
    xaml_ptr<xaml_timer> m_timer{};
    xaml_ptr<xaml_observable_vector> m_combo_source{};
    int count{ 0 };

    xaml_result XAML_CALL init() noexcept;
    xaml_result XAML_CALL show() noexcept override
    {
        return m_window->show();
    }

    xaml_result XAML_CALL on_timer_tick(xaml_ptr<xaml_timer>) noexcept;
    xaml_result XAML_CALL on_button_click(xaml_ptr<xaml_button>) noexcept;
    xaml_result XAML_CALL on_canvas_redraw(xaml_ptr<xaml_canvas>, xaml_ptr<xaml_drawing_context>) noexcept;
    xaml_result XAML_CALL on_window_closing(xaml_ptr<xaml_window>, xaml_ptr<xaml_box>) noexcept;
};

xaml_result xaml_test_window_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_window_new(&m_window));
    XAML_RETURN_IF_FAILED(xaml_timer_new_interval(1s, &m_timer));
    XAML_RETURN_IF_FAILED(xaml_observable_vector_new(&m_combo_source));
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("A"), &item));
        XAML_RETURN_IF_FAILED(m_combo_source->append(item.get()));
    }
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("BBB"), &item));
        XAML_RETURN_IF_FAILED(m_combo_source->append(item.get()));
    }
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("C"), &item));
        XAML_RETURN_IF_FAILED(m_combo_source->append(item.get()));
    }
    {
        xaml_ptr<xaml_delegate> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void, xaml_ptr<xaml_timer>>(xaml_mem_fn(&xaml_test_window_impl::on_timer_tick, this), &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(m_timer->add_tick(callback.get(), &token));
    }
    {
        xaml_ptr<xaml_delegate> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void, xaml_ptr<xaml_window>, xaml_ptr<xaml_box>>(xaml_mem_fn(&xaml_test_window_impl::on_window_closing, this), &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(m_window->add_closing(callback.get(), &token));
    }
    // Set title, location, and size.
    // Cover the string constant with `U()` macro.
    set_title(U("Test Window"));
    set_location({ 100, 100 });
    set_size({ 800, 600 });
    // Construct a grid.
    auto g = make_shared<grid>();
    // Add 3 columns and 3 rows, with same width or height.
    g->add_column({ 1, grid_layout::star });
    g->add_column({ 1, grid_layout::star });
    g->add_column({ 1, grid_layout::star });
    g->add_row({ 1, grid_layout::compact });
    g->add_row({ 1, grid_layout::compact });
    g->add_row({ 1, grid_layout::star });
    g->add_row({ 1, grid_layout::star });
    // Set g as child of the window.
    set_child(g);
    {
        auto prog = make_shared<progress>();
        prog->set_margin({ 10, 10, 10, 10 });
        prog->set_valignment(valignment_t::center);
        prog->set_minimum(0);
        prog->set_maximum(100);
        prog->set_value(30);
        prog->set_is_indeterminate(true);
        g->add_child(prog);
        grid::set_column(*prog, 2);
        grid::set_row(*prog, 2);
        // Construct a button.
        auto btn = make_shared<button>();
        // Set margin and text.
        btn->set_margin({ 10, 10, 10, 10 });
        btn->set_text(U("Hello"));
        // Add a handler for button.
        btn->add_click([prog](shared_ptr<button>) { prog->set_is_indeterminate(false); });
        btn->add_click(mem_fn_bind(&test_window::on_button_click, this));
        // Set the button to be placed in the center vertically.
        btn->set_valignment(valignment_t::center);
        // Add the button to the grid, and set its column and row.
        g->add_child(btn);
        grid::set_column(*btn, 1);
        grid::set_row(*btn, 2);
    }
    {
        // Construct a label.
        auto lb = make_shared<label>();
        // Set the margin, text alignmemt, vertical alignment and text.
        lb->set_margin({ 10, 10, 10, 10 });
        lb->set_text_halignment(halignment_t::right);
        lb->set_valignment(valignment_t::center);
        lb->set_text(U("Username:"));
        // Add the label to the grid, and set its column and row.
        g->add_child(lb);
        grid::set_column(*lb, 0);
        grid::set_row(*lb, 0);
    }
    {
        // Construct an entry (single-line text box).
        auto et = make_shared<entry>();
        et->set_margin({ 0, 5, 0, 5 });
        et->set_text(U("John"));
        et->set_halignment(halignment_t::left);
        et->set_valignment(valignment_t::center);
        g->add_child(et);
        grid::set_column(*et, 1);
        grid::set_row(*et, 0);
    }
    {
        auto lb2 = make_shared<label>();
        lb2->set_margin({ 10, 10, 10, 10 });
        lb2->set_text_halignment(halignment_t::right);
        lb2->set_valignment(valignment_t::center);
        lb2->set_text(U("Password:"));
        g->add_child(lb2);
        grid::set_column(*lb2, 0);
        grid::set_row(*lb2, 1);
    }
    {
        auto pet = make_shared<password_entry>();
        pet->set_margin({ 0, 5, 0, 5 });
        pet->set_text(U("123456"));
        pet->set_valignment(valignment_t::center);
        g->add_child(pet);
        grid::set_column(*pet, 1);
        grid::set_row(*pet, 1);
    }
    {
        // Construct a cnavas.
        auto cv = make_shared<canvas>();
        // Set the margin.
        cv->set_margin({ 10, 10, 10, 10 });
        // Add a handler to the canvas to handle the redraw event.
        // You should always draw in that event.
        cv->add_redraw(mem_fn_bind(&test_window::on_canvas_redraw, this));
        // Add the canvas to the grid, and set its column and row.
        g->add_child(cv);
        grid::set_column(*cv, 0);
        grid::set_row(*cv, 3);
    }
    {
        auto box = make_shared<combo_box>();
        box->set_halignment(halignment_t::center);
        box->set_valignment(valignment_t::top);
        box->set_margin({ 10, 10, 10, 10 });
        box->set_items(combo_source);
        box->set_sel_id(1);
        g->add_child(box);
        grid::set_column(*box, 1);
        grid::set_row(*box, 3);

        auto panel = make_shared<stack_panel>();
        panel->set_orientation(orientation::vertical);
        panel->set_margin({ 5, 5, 5, 5 });
        {
            auto btn = make_shared<button>();
            btn->set_margin({ 5, 5, 5, 5 });
            btn->set_text(U("Push"));
            btn->add_click([this](shared_ptr<button>) { combo_source.push_back(U("DDDD")); });
            panel->add_child(btn);
        }
        {
            auto btn = make_shared<button>();
            btn->set_margin({ 5, 5, 5, 5 });
            btn->set_text(U("Pop"));
            btn->add_click([this](shared_ptr<button>) { combo_source.pop_back(); });
            panel->add_child(btn);
        }
        {
            auto btn = make_shared<button>();
            btn->set_margin({ 5, 5, 5, 5 });
            btn->set_text(U("Show"));
            btn->add_click([this, box](shared_ptr<button>) {
                int32_t sel = box->get_sel_id();
                if (sel >= 0 && sel < combo_source.size())
                    msgbox(shared_from_this<window>(), combo_source[sel].get(), U("Show selected item"));
            });
            panel->add_child(btn);
        }
        g->add_child(panel);
        grid::set_column(*panel, 2);
        grid::set_row(*panel, 3);
    }
    {
        auto mbar = make_shared<menu_bar>();
        {
            auto mfile = make_shared<popup_menu_item>();
            mfile->set_text(U("File"));
            {
                auto mquit = make_shared<menu_item>();
                mquit->set_text(U("Close"));
                mquit->add_click([this](shared_ptr<menu_item>) { close(); });
                mfile->add_submenu(mquit);
            }
            mbar->add_child(mfile);
        }
        set_menu_bar(mbar);
    }
    return XAML_S_OK;
}

xaml_result xaml_test_window_impl::on_timer_tick(xaml_ptr<xaml_timer>) noexcept
{
    xaml_ptr<xaml_string> msg;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &msg));
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello"), &title));
    xaml_msgbox_result res;
    XAML_RETURN_IF_FAILED(xaml_msgbox(m_window.get(), msg.get(), title.get(), nullptr, xaml_msgbox_info, xaml_msgbox_buttons_ok, &res));
    if (++count >= 3)
    {
        XAML_RETURN_IF_FAILED(m_timer->stop());
        xaml_ptr<xaml_open_filebox> openbox;
        XAML_RETURN_IF_FAILED(xaml_open_filebox_new(&openbox));
        xaml_ptr<xaml_string> title;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Open file"), &title));
        XAML_RETURN_IF_FAILED(openbox->set_title(title.get()));
        xaml_ptr<xaml_object> f;
        XAML_RETURN_IF_FAILED(xaml_box_value<xaml_filebox_filter>({ U("XAML file"), U("*.xaml") }, &f));
        xaml_ptr<xaml_vector> filters;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ f }, &filters));
        XAML_RETURN_IF_FAILED(openbox->set_filters(filters.get()));
        if (XAML_SUCCEEDED(openbox->show(m_window.get())))
        {
            xaml_ptr<xaml_string> result;
            XAML_RETURN_IF_FAILED(openbox->get_result(&result));
            xaml_ptr<xaml_string> title;
            XAML_RETURN_IF_FAILED(xaml_string_new(U("Open file"), &title));
            XAML_RETURN_IF_FAILED(xaml_msgbox(m_window.get(), result.get(), title.get(), nullptr, xaml_msgbox_none, xaml_msgbox_buttons_ok, &res));
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_test_window_impl::on_button_click(xaml_ptr<xaml_button> btn) noexcept
{
    xaml_ptr<xaml_string> text;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &text));
    XAML_RETURN_IF_FAILED(btn->set_text(text.get()));
    count = 0;
    return m_timer->start();
}

xaml_result xaml_test_window_impl::on_canvas_redraw(xaml_ptr<xaml_canvas> cv, xaml_ptr<xaml_drawing_context> dc)
{
    xaml_ptr<xaml_application> current_app;
    XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
    xaml_application_theme theme;
    XAML_RETURN_IF_FAILED(current_app->get_theme(&theme));
    bool is_dark = theme == xaml_application_theme_dark;
    xaml_size csize;
    XAML_RETURN_IF_FAILED(cv->get_size(&csize));
    auto cx = csize.width / 2;
    auto cy = csize.height / 2;
    auto r = (min)(cx, cy) - 2;
    XAML_RETURN_IF_FAILED(dc->draw_arc({ is_dark ? white : black }, { cx - r, cy - r, r * 2, r * 2 }, pi, 2 * pi));
    XAML_RETURN_IF_FAILED(dc->draw_line({ is_dark ? white : black }, { cx - r, cy }, { cx + r, cy }));
    XAML_RETURN_IF_FAILED(dc->draw_round_rect({ sky_blue }, { cx - r - 1, cy - r - 1, r * 2 + 2, r * 1.618 + 2 }, { r / 10, r / 10 }));
    xaml_ptr<xaml_string> text;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &text));
    XAML_RETURN_IF_FAILED(dc->draw_string({ pink }, { U("Arial"), r / 5, false, false, xaml_halignment_center, xaml_valignment_bottom }, { cx, cy }, text.get()));
    return XAML_S_OK;
}

xaml_result xaml_test_window_impl::on_window_closing(xaml_ptr<xaml_window> win, xaml_ptr<xaml_box> box)
{
    bool handled;
    XAML_RETURN_IF_FAILED(xaml_unbox_value(box.get(), handled));
    xaml_ptr<xaml_string> msg;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Do you want to close this window?"), &msg));
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Close window"), &title));
    xaml_msgbox_result res;
    XAML_RETURN_IF_FAILED(xaml_msgbox(m_window.get(), msg.get(), title.get(), nullptr, xaml_msgbox_question, xaml_msgbox_buttons_yes_no, &res));
    handled = res != xaml_msgbox_result_yes;
    XAML_RETURN_IF_FAILED(box->set_value(handled));
    return XAML_S_OK;
}
