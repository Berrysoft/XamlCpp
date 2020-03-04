#include <xaml/ui/controls/menu_item.hpp>

namespace xaml
{
    menu_item::menu_item() : control() {}

    menu_item::~menu_item() {}

    check_menu_item::check_menu_item() : menu_item() {}

    check_menu_item::~check_menu_item() {}

    radio_menu_item::radio_menu_item() : menu_item() {}

    radio_menu_item::~radio_menu_item() {}

    separator_menu_item::separator_menu_item() : menu_item() {}

    separator_menu_item::~separator_menu_item() {}
}
