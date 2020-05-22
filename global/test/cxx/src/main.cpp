#include <boost/nowide/args.hpp>
#include <boost/nowide/iostream.hpp>
#include <cassert>
#include <iostream>
#include <sf/format.hpp>
#include <xaml/delegate.h>
#include <xaml/enumerable.h>
#include <xaml/map.h>
#include <xaml/observable_vector.h>
#include <xaml/ptr.hpp>
#include <xaml/result.h>
#include <xaml/string.h>
#include <xaml/vector.h>

std::ostream& cout = boost::nowide::cout;

int main()
{
    xaml_ptr<xaml_string> str;
    XAML_THROW_IF_FAILED(xaml_string_new_view(U("Hello world!"), &str));
    sf::println(cout, to_string_view(str));

    xaml_ptr<xaml_observable_vector> vec;
    XAML_THROW_IF_FAILED(xaml_observable_vector_new(&vec));
    xaml_ptr<xaml_delegate> callback;
    XAML_THROW_IF_FAILED((xaml_delegate_new<void, xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args>>(
        [](xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args> args) {
            xaml_vector_changed_action action;
            XAML_THROW_IF_FAILED(args->get_action(&action));
            switch (action)
            {
            case xaml_vector_changed_add:
            {
                int32_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                sf::print(cout, U("Add item at {}: "), index);
                xaml_ptr<xaml_vector_view> new_items;
                XAML_THROW_IF_FAILED(args->get_new_items(&new_items));
                for (auto obj : new_items)
                {
                    sf::print(cout, U("{} "), xaml_unbox_value<int>(obj));
                }
                sf::println(cout);
                break;
            }
            case xaml_vector_changed_erase:
            {
                int32_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                sf::print(cout, U("Erase item at {}: "), index);
                xaml_ptr<xaml_vector_view> old_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                for (auto obj : old_items)
                {
                    sf::print(cout, U("{} "), xaml_unbox_value<int>(obj));
                }
                sf::println(cout);
                break;
            }
            case xaml_vector_changed_move:
            {
                xaml_ptr<xaml_vector_view> old_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                xaml_ptr<xaml_object> obj;
                XAML_THROW_IF_FAILED(old_items->get_at(0, &obj));
                int32_t old_index, new_index;
                XAML_THROW_IF_FAILED(args->get_old_index(&old_index));
                XAML_THROW_IF_FAILED(args->get_new_index(&new_index));
                sf::println(cout, U("Move item {} at {} to {}"), xaml_unbox_value<int>(obj), old_index, new_index);
                break;
            }
            case xaml_vector_changed_replace:
            {
                int32_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                xaml_ptr<xaml_vector_view> old_items, new_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                XAML_THROW_IF_FAILED(args->get_new_items(&new_items));
                xaml_ptr<xaml_object> old_item, new_item;
                XAML_THROW_IF_FAILED(old_items->get_at(0, &old_item));
                XAML_THROW_IF_FAILED(new_items->get_at(0, &new_item));
                sf::println(cout, U("Replace item at {} from {} to {}"), index, xaml_unbox_value<int>(old_item), xaml_unbox_value<int>(new_item));
                break;
            }
            case xaml_vector_changed_reset:
            {
                xaml_ptr<xaml_vector_view> old_items, new_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                XAML_THROW_IF_FAILED(args->get_new_items(&new_items));
                int32_t old_size, new_size;
                XAML_THROW_IF_FAILED(old_items->get_size(&old_size));
                XAML_THROW_IF_FAILED(new_items->get_size(&new_size));
                sf::println(cout, U("Reset. Old count: {}; new count: {}"), old_size, new_size);
                break;
            }
            }
        },
        &callback)));
    int32_t token;
    XAML_THROW_IF_FAILED(vec->add_vector_changed(callback, &token));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(1)));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(2)));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(3)));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(4)));
    XAML_THROW_IF_FAILED(vec->remove_at(0));
    XAML_THROW_IF_FAILED(vec->set_at(2, xaml_box_value(100)));
    XAML_THROW_IF_FAILED(vec->clear());

    xaml_ptr<xaml_hasher> hasher;
    XAML_THROW_IF_FAILED(xaml_hasher_new<int>(&hasher));
    xaml_ptr<xaml_map> map;
    XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(hasher, &map));
    bool replaced;
    XAML_THROW_IF_FAILED(map->insert(xaml_box_value(1), str, &replaced));
    XAML_THROW_IF_FAILED(map->insert(xaml_box_value(2), vec, &replaced));
    xaml_ptr<xaml_object> obj1;
    XAML_THROW_IF_FAILED(map->lookup(xaml_box_value(1), &obj1));
    sf::println(cout, xaml_unbox_value<xaml_ptr<xaml_string>>(obj1));
}
