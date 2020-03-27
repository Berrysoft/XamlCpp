#include <xaml/ui/container.hpp>
#include <xaml/ui/controls/radio_box.hpp>

using namespace std;

namespace xaml
{
    radio_box::radio_box() : button()
    {
        add_is_checked_changed([this](shared_ptr<radio_box>, bool) {
            if (get_handle())
            {
                draw_checked();
                draw_group();
            }
        });
#ifdef XAML_UI_COCOA
        add_click([this](shared_ptr<button>) { if (get_handle()) __on_state_changed(); });
#endif // XAML_UI_COCOA
    }

    radio_box::~radio_box() {}

#ifndef XAML_UI_GTK3
    void radio_box::draw_group()
    {
        if (auto sparent = get_parent().lock())
        {
            if (get_is_checked())
            {
                if (auto multic = sparent->query<multicontainer>())
                {
                    for (auto& c : multic->get_children())
                    {
                        if (auto rc = c->query<radio_box>())
                        {
                            if (c != shared_from_this() && rc->get_group() == get_group())
                            {
                                rc->set_is_checked(false);
                            }
                        }
                    }
                }
            }
        }
    }
#endif // !XAML_UI_GTK3
} // namespace xaml
