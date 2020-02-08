#include <xaml/ui/container.hpp>
#include <xaml/ui/controls/radio_box.hpp>

using namespace std;

namespace xaml
{
    radio_box::radio_box() : button()
    {
        add_is_checked_changed([this](radio_box const&, bool checked) {
            if (get_handle())
            {
                draw_checked();
                if (checked && get_parent())
                {
                    if (auto multic = dynamic_pointer_cast<multicontainer>(get_parent()))
                    {
                        for (auto& c : multic->get_children())
                        {
                            if (auto rc = dynamic_pointer_cast<radio_box>(c))
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
        });
    }

    radio_box::~radio_box() {}
} // namespace xaml
