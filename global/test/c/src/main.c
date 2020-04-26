#include <assert.h>
#include <stdio.h>
#include <xaml/observable_vector.h>
#include <xaml/string.h>
#include <xaml/vector.h>

#ifdef UNICODE
#ifndef _tprintf
#define _tprintf wprintf
#endif // !_tprintf
#else
#ifndef _tprintf
#define _tprintf printf
#endif // !_tprintf
#endif // UNICODE

void print_string(xaml_string* str)
{
    xaml_char_t const* data;
    XAML_ASSERT_SUCCESS(str->vtbl->get_data(str, &data));
    int32_t length;
    XAML_ASSERT_SUCCESS(str->vtbl->get_length(str, &length));
    _tprintf(U("%*s "), (int)length, data);
}

xaml_result observable_vector_changed_callback(xaml_vector_view* args, xaml_object** ptr)
{
    xaml_vector_changed_args* e;
    XAML_RETURN_IF_FAILED(args->vtbl->get_at(args, 1, (xaml_object**)&e));
    xaml_vector_changed_action action;
    XAML_RETURN_IF_FAILED(e->vtbl->get_action(e, &action));
    switch (action)
    {
    case xaml_vector_changed_add:
    {
        int32_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Add item at %zu: "), index);
        xaml_vector_view* new_items;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_items(e, &new_items));
        int32_t size;
        XAML_RETURN_IF_FAILED(new_items->vtbl->get_size(new_items, &size));
        for (int32_t i = 0; i < size; i++)
        {
            xaml_object* obj;
            XAML_RETURN_IF_FAILED(new_items->vtbl->get_at(new_items, i, &obj));
            xaml_string* str;
            XAML_RETURN_IF_FAILED(obj->vtbl->query(obj, &xaml_guid_xaml_string, (void**)&str));
            print_string(str);
            str->vtbl->release(str);
            obj->vtbl->release(obj);
        }
        new_items->vtbl->release(new_items);
        _tprintf(U("\n"));
        break;
    }
    case xaml_vector_changed_erase:
    {
        int32_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Erase item at %zu: "), index);
        xaml_vector_view* old_items;
        XAML_RETURN_IF_FAILED(e->vtbl->get_old_items(e, &old_items));
        int32_t size;
        XAML_RETURN_IF_FAILED(old_items->vtbl->get_size(old_items, &size));
        for (int32_t i = 0; i < size; i++)
        {
            xaml_object* obj;
            XAML_RETURN_IF_FAILED(old_items->vtbl->get_at(old_items, i, &obj));
            xaml_string* str;
            XAML_RETURN_IF_FAILED(obj->vtbl->query(obj, &xaml_guid_xaml_string, (void**)&str));
            print_string(str);
            str->vtbl->release(str);
            obj->vtbl->release(obj);
        }
        old_items->vtbl->release(old_items);
        _tprintf(U("\n"));
        break;
    }
    case xaml_vector_changed_move:
    {
        int32_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Move item at %zu\n"), index);
        break;
    }
    case xaml_vector_changed_replace:
    {
        int32_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Replace item at %zu\n"), index);
        break;
    }
    case xaml_vector_changed_reset:
        _tprintf(U("Reset.\n"));
        break;
    }
    e->vtbl->release(e);
    *ptr = NULL;
    return XAML_S_OK;
}

int main()
{
    xaml_string* str;
    XAML_ASSERT_SUCCESS(xaml_string_new(U("Hello world!"), &str));
    xaml_observable_vector* vec;
    XAML_ASSERT_SUCCESS(xaml_observable_vector_new(&vec));
    xaml_delegate* callback;
    XAML_ASSERT_SUCCESS(xaml_delegate_new(observable_vector_changed_callback, &callback));
    int32_t token;
    XAML_ASSERT_SUCCESS(vec->vtbl->add_vector_changed(vec, callback, &token));
    callback->vtbl->release(callback);
    XAML_ASSERT_SUCCESS(vec->vtbl->append(vec, (xaml_object*)str));
    XAML_ASSERT_SUCCESS(vec->vtbl->remove_at(vec, 0));
    vec->vtbl->release(vec);
    str->vtbl->release(str);
}
