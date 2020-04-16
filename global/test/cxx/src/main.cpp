#include <cassert>
#include <iostream>
#include <xaml/callback.hpp>
#include <xaml/enumerable.hpp>
#include <xaml/observable_vector.h>
#include <xaml/result.hpp>
#include <xaml/string.hpp>
#include <xaml/vector.hpp>
#include <xaml/xaml_ptr.hpp>

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

void print_string(xaml_ptr<xaml_string> const& str)
{
    xaml_char_t const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    size_t length;
    XAML_THROW_IF_FAILED(str->get_length(&length));
    _tcout << xaml_std_string_view_t(data, length) << ' ';
}

int main()
{
    xaml_ptr<xaml_string> str;
    XAML_THROW_IF_FAILED(xaml_string_new(U("Hello world!"), &str));
    xaml_ptr<xaml_observable_vector> vec;
    XAML_THROW_IF_FAILED(xaml_observable_vector_new(&vec));
    xaml_ptr<xaml_callback> callback;
    XAML_THROW_IF_FAILED(xaml_callback_new<void(xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args>)>(
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
                for (auto& obj : new_items)
                {
                    xaml_ptr<xaml_string> str;
                    XAML_THROW_IF_FAILED(obj->query(&str));
                    print_string(str);
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
                for (auto& obj : old_items)
                {
                    xaml_ptr<xaml_string> str;
                    XAML_THROW_IF_FAILED(obj->query(&str));
                    print_string(str);
                }
                _tcout << endl;
                break;
            }
            case xaml_vector_changed_move:
            {
                size_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                _tcout << U("Move item at ") << index << endl;
                break;
            }
            case xaml_vector_changed_replace:
            {
                size_t index;
                XAML_THROW_IF_FAILED(args->get_new_index(&index));
                _tcout << U("Replace item at ") << index << endl;
                break;
            }
            case xaml_vector_changed_reset:
                _tcout << U("Reset") << endl;
                break;
            }
        },
        &callback));
    size_t token;
    XAML_THROW_IF_FAILED(vec->add_vector_changed(callback.get(), &token));
    XAML_THROW_IF_FAILED(vec->append(str.get()));
    XAML_THROW_IF_FAILED(vec->remove_at(0));
}
