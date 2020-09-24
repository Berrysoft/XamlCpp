#include <assert.h>
#include <helper.h>
#include <stdio.h>
#include <xaml/observable_vector.h>
#include <xaml/string.h>
#include <xaml/vector.h>

xaml_result XAML_CALL print_string(xaml_string* str)
{
    char const* data;
    XAML_RETURN_IF_FAILED(str->vtbl->get_data(str, &data));
    int32_t length;
    XAML_RETURN_IF_FAILED(str->vtbl->get_length(str, &length));
    printf(U("%*s "), (int)length, data);
    return XAML_S_OK;
}

xaml_result XAML_CALL observable_vector_changed_callback(xaml_object* sender, xaml_vector_changed_args__xaml_string__* e)
{
    (void)sender;
    xaml_result hr;
    xaml_vector_changed_action action;
    XAML_GOTO_IF_FAILED(e->vtbl->get_action(e, &action), exit);
    switch (action)
    {
    case xaml_vector_changed_add:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Add item at %d: "), index);
        xaml_vector_view__xaml_string__* new_items;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_items(e, &new_items), exit);
        int32_t size;
        XAML_GOTO_IF_FAILED(new_items->vtbl->get_size(new_items, &size), clean_new_items_1);
        for (int32_t i = 0; i < size; i++)
        {
            xaml_string* str;
            XAML_GOTO_IF_FAILED(new_items->vtbl->get_at(new_items, i, &str), clean_new_items_1);
            XAML_GOTO_IF_FAILED(print_string(str), clean_str_1);
            str->vtbl->release(str);
            continue;
        clean_str_1:
            str->vtbl->release(str);
            goto clean_new_items_1;
        }
        printf(U("\n"));
        new_items->vtbl->release(new_items);
        break;
    clean_new_items_1:
        new_items->vtbl->release(new_items);
        goto exit;
    }
    case xaml_vector_changed_erase:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Erase item at %d: "), index);
        xaml_vector_view__xaml_string__* old_items;
        XAML_GOTO_IF_FAILED(e->vtbl->get_old_items(e, &old_items), exit);
        int32_t size;
        XAML_GOTO_IF_FAILED(old_items->vtbl->get_size(old_items, &size), clean_old_items_2);
        for (int32_t i = 0; i < size; i++)
        {
            xaml_string* str;
            XAML_GOTO_IF_FAILED(old_items->vtbl->get_at(old_items, i, &str), clean_old_items_2);
            XAML_GOTO_IF_FAILED(print_string(str), clean_str_2);
            str->vtbl->release(str);
            continue;
        clean_str_2:
            str->vtbl->release(str);
            goto clean_old_items_2;
        }
        printf(U("\n"));
        old_items->vtbl->release(old_items);
        break;
    clean_old_items_2:
        old_items->vtbl->release(old_items);
        goto exit;
    }
    case xaml_vector_changed_move:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Move item at %d\n"), index);
        break;
    }
    case xaml_vector_changed_replace:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), exit);
        printf(U("Replace item at %d\n"), index);
        break;
    }
    case xaml_vector_changed_reset:
        printf(U("Reset.\n"));
        break;
    }
exit:
    return hr;
}

xaml_result main_impl()
{
    xaml_result hr;
    xaml_string* str;
    XAML_GOTO_IF_FAILED(xaml_string_new_view(U("Hello world!"), &str), exit);
    xaml_observable_vector__xaml_string__* vec;
    XAML_GOTO_IF_FAILED(xaml_observable_vector__xaml_string___new(&vec), clean_str);
    xaml_delegate__xaml_object____xaml_vector_changed_args__xaml_string____* callback;
    XAML_GOTO_IF_FAILED(xaml_delegate__xaml_object____xaml_vector_changed_args__xaml_string_____new(observable_vector_changed_callback, &callback), clean_vec);
    int32_t token;
    XAML_GOTO_IF_FAILED(vec->vtbl->add_vector_changed(vec, callback, &token), clean_callback);
    XAML_GOTO_IF_FAILED(vec->vtbl->append(vec, str), clean_callback);
    XAML_GOTO_IF_FAILED(vec->vtbl->remove_at(vec, 0), clean_callback);
clean_callback:
    callback->vtbl->release(callback);
clean_vec:
    vec->vtbl->release(vec);
clean_str:
    str->vtbl->release(str);
exit:
    return hr;
}

int main()
{
    XAML_ASSERT_SUCCEEDED(main_impl());
}
