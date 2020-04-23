#include <cassert>
#include <iostream>
#include <xaml/delegate.h>
#include <xaml/enumerable.h>
#include <xaml/map.h>
#include <xaml/observable_vector.h>
#include <xaml/ptr.hpp>
#include <xaml/result.h>
#include <xaml/string.h>
#include <xaml/vector.h>

#ifdef UNICODE
#ifndef _tcout
#define _tcout wcout
#endif // !_tcout
#else
#ifndef _tcout
#define _tcout cout
#endif // !_tcout
#endif // UNICODE

using namespace std;

int main()
{
    xaml_ptr<xaml_string> str;
    XAML_THROW_IF_FAILED(xaml_string_new(U("Hello world!"), &str));
    _tcout << to_string_view_t(str) << endl;

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
                size_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                _tcout << U("Add item at ") << index << ": ";
                xaml_ptr<xaml_vector_view> new_items;
                XAML_THROW_IF_FAILED(args->get_new_items(&new_items));
                for (auto obj : new_items)
                {
                    int value = xaml_unbox_value<int>(obj);
                    _tcout << value << ' ';
                }
                _tcout << endl;
                break;
            }
            case xaml_vector_changed_erase:
            {
                size_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                _tcout << U("Erase item at ") << index << ": ";
                xaml_ptr<xaml_vector_view> old_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                for (auto obj : old_items)
                {
                    int value = xaml_unbox_value<int>(obj);
                    _tcout << value << ' ';
                }
                _tcout << endl;
                break;
            }
            case xaml_vector_changed_move:
            {
                xaml_ptr<xaml_vector_view> old_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                xaml_ptr<xaml_object> obj;
                XAML_THROW_IF_FAILED(old_items->get_at(0, &obj));
                size_t old_index, new_index;
                XAML_THROW_IF_FAILED(args->get_old_index(&old_index));
                XAML_THROW_IF_FAILED(args->get_new_index(&new_index));
                _tcout << U("Move item ") << xaml_unbox_value<int>(obj) << U(" at ") << old_index << U(" to ") << new_index << endl;
                break;
            }
            case xaml_vector_changed_replace:
            {
                size_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                xaml_ptr<xaml_vector_view> old_items, new_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                XAML_THROW_IF_FAILED(args->get_new_items(&new_items));
                xaml_ptr<xaml_object> old_item, new_item;
                XAML_THROW_IF_FAILED(old_items->get_at(0, &old_item));
                XAML_THROW_IF_FAILED(new_items->get_at(0, &new_item));
                _tcout << U("Replace item at ") << index << U(" from ") << xaml_unbox_value<int>(old_item) << U(" to ") << xaml_unbox_value<int>(new_item) << endl;
                break;
            }
            case xaml_vector_changed_reset:
            {
                xaml_ptr<xaml_vector_view> old_items, new_items;
                XAML_THROW_IF_FAILED(args->get_old_items(&old_items));
                XAML_THROW_IF_FAILED(args->get_new_items(&new_items));
                size_t old_size, new_size;
                XAML_THROW_IF_FAILED(old_items->get_size(&old_size));
                XAML_THROW_IF_FAILED(new_items->get_size(&new_size));
                _tcout << U("Reset. Old count: ") << old_size << U("; new count: ") << new_size << endl;
                break;
            }
            }
        },
        &callback)));
    size_t token;
    XAML_THROW_IF_FAILED(vec->add_vector_changed(callback.get(), &token));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(1).get()));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(2).get()));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(3).get()));
    XAML_THROW_IF_FAILED(vec->append(xaml_box_value(4).get()));
    XAML_THROW_IF_FAILED(vec->remove_at(0));
    XAML_THROW_IF_FAILED(vec->set_at(2, xaml_box_value(100).get()));
    XAML_THROW_IF_FAILED(vec->clear());

    xaml_ptr<xaml_hasher> hasher;
    XAML_THROW_IF_FAILED(xaml_hasher_new<int>(&hasher));
    xaml_ptr<xaml_map> map;
    XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(hasher.get(), &map));
    bool replaced;
    XAML_THROW_IF_FAILED(map->insert(xaml_box_value(1).get(), str.get(), &replaced));
    XAML_THROW_IF_FAILED(map->insert(xaml_box_value(2).get(), vec.get(), &replaced));
    xaml_ptr<xaml_object> obj1;
    XAML_THROW_IF_FAILED(map->lookup(xaml_box_value(1).get(), &obj1));
    _tcout << to_string_view_t(xaml_unbox_value<xaml_ptr<xaml_string>>(obj1)) << endl;
}
