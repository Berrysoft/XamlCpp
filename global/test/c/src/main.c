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

inline void check_result(xaml_result res)
{
    assert(XAML_SUCCESS(res));
}

xaml_result observable_vector_changed_callback(xaml_object* sender, xaml_object* args)
{
    xaml_vector_changed_args* e;
    XAML_RETURN_IF_FAILED(args->vtbl->query(args, &xaml_type_guid_v(xaml_vector_changed_args), (xaml_object**)&e));
    xaml_vector_changed_action action;
    XAML_RETURN_IF_FAILED(e->vtbl->get_action(e, &action));
    switch (action)
    {
    case xaml_vector_changed_add:
    {
        size_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Add item at %zu\n"), index);
        break;
    }
    case xaml_vector_changed_erase:
    {
        size_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Erase item at %zu\n"), index);
        break;
    }
    case xaml_vector_changed_move:
    {
        size_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Move item at %zu\n"), index);
        break;
    }
    case xaml_vector_changed_replace:
    {
        size_t index;
        XAML_RETURN_IF_FAILED(e->vtbl->get_new_index(e, &index));
        _tprintf(U("Replace item at %zu\n"), index);
        break;
    }
    case xaml_vector_changed_reset:
        _tprintf(U("Reset.\n"));
        break;
    }
    e->vtbl->release(e);
    return XAML_S_OK;
}

int main()
{
    xaml_string* str;
    check_result(xaml_string_new(U("Hello world!"), &str));
    xaml_char_t const* data;
    check_result(str->vtbl->get_data(str, &data));
    size_t length;
    check_result(str->vtbl->get_length(str, &length));
    _tprintf(U("%*s\n"), (int)length, data);

    xaml_observable_vector* vec;
    check_result(xaml_observable_vector_new(&vec));
    xaml_callback* callback;
    check_result(xaml_callback_new(observable_vector_changed_callback, &callback));
    size_t token;
    check_result(vec->vtbl->add_vector_changed(vec, callback, &token));
    callback->vtbl->release(callback);
    xaml_object* obj;
    check_result(str->vtbl->query(str, &xaml_type_guid_v(xaml_object), &obj));
    check_result(vec->vtbl->append(vec, obj));
    check_result(vec->vtbl->remove_at(vec, 0));
    vec->vtbl->release(vec);
    str->vtbl->release(str);
}
