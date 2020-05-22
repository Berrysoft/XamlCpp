#include <assert.h>
#include <stdio.h>
#include <xaml/box.h>
#include <xaml/observable_vector.h>
#include <xaml/string.h>
#include <xaml/vector.h>

xaml_result XAML_CALL print_string(char const* str)
{
    printf(U("%s "), str);
    return XAML_S_OK;
}

xaml_result XAML_CALL observable_vector_changed_callback(xaml_vector_view* args, xaml_object** ptr)
{
    xaml_result hr;
    xaml_object* e_obj;
    XAML_GOTO_IF_FAILED(args->vtbl->get_at(args, 1, &e_obj), exit);
    xaml_vector_changed_args* e;
    XAML_GOTO_IF_FAILED(e_obj->vtbl->query(e_obj, &xaml_guid_xaml_vector_changed_args, (void**)&e), clean_e_obj);
    xaml_vector_changed_action action;
    XAML_GOTO_IF_FAILED(e->vtbl->get_action(e, &action), clean_e);
    switch (action)
    {
    case xaml_vector_changed_add:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), clean_e);
        printf(U("Add item at %d: "), index);
        xaml_vector_view* new_items;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_items(e, &new_items), clean_e);
        int32_t size;
        XAML_GOTO_IF_FAILED(new_items->vtbl->get_size(new_items, &size), clean_new_items_1);
        for (int32_t i = 0; i < size; i++)
        {
            xaml_object* obj;
            XAML_GOTO_IF_FAILED(new_items->vtbl->get_at(new_items, i, &obj), clean_new_items_1);
            xaml_box* str_box;
            XAML_GOTO_IF_FAILED(obj->vtbl->query(obj, &xaml_guid_xaml_box, (void**)&str_box), clean_obj_1);
            char const* str;
            str_box->vtbl->get_data(str_box, &str);
            XAML_GOTO_IF_FAILED(print_string(str), clean_str_1);
            str_box->vtbl->release(str_box);
            obj->vtbl->release(obj);
            continue;
        clean_str_1:
            str_box->vtbl->release(str_box);
        clean_obj_1:
            obj->vtbl->release(obj);
            goto clean_new_items_1;
        }
        printf(U("\n"));
        new_items->vtbl->release(new_items);
        break;
    clean_new_items_1:
        new_items->vtbl->release(new_items);
        goto clean_e;
    }
    case xaml_vector_changed_erase:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), clean_e);
        printf(U("Erase item at %d: "), index);
        xaml_vector_view* old_items;
        XAML_GOTO_IF_FAILED(e->vtbl->get_old_items(e, &old_items), clean_e);
        int32_t size;
        XAML_GOTO_IF_FAILED(old_items->vtbl->get_size(old_items, &size), clean_old_items_2);
        for (int32_t i = 0; i < size; i++)
        {
            xaml_object* obj;
            XAML_GOTO_IF_FAILED(old_items->vtbl->get_at(old_items, i, &obj), clean_old_items_2);
            xaml_box* str_box;
            XAML_GOTO_IF_FAILED(obj->vtbl->query(obj, &xaml_guid_xaml_box, (void**)&str_box), clean_obj_2);
            char const* str;
            str_box->vtbl->get_data(str_box, &str);
            XAML_GOTO_IF_FAILED(print_string(str), clean_str_2);
            str_box->vtbl->release(str_box);
            obj->vtbl->release(obj);
            continue;
        clean_str_2:
            str_box->vtbl->release(str_box);
        clean_obj_2:
            obj->vtbl->release(obj);
            goto clean_old_items_2;
        }
        printf(U("\n"));
        old_items->vtbl->release(old_items);
        break;
    clean_old_items_2:
        old_items->vtbl->release(old_items);
        goto clean_e;
    }
    case xaml_vector_changed_move:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), clean_e);
        printf(U("Move item at %d\n"), index);
        break;
    }
    case xaml_vector_changed_replace:
    {
        int32_t index;
        XAML_GOTO_IF_FAILED(e->vtbl->get_new_index(e, &index), clean_e);
        printf(U("Replace item at %d\n"), index);
        break;
    }
    case xaml_vector_changed_reset:
        printf(U("Reset.\n"));
        break;
    }
    *ptr = NULL;
clean_e:
    e->vtbl->release(e);
clean_e_obj:
    e_obj->vtbl->release(e_obj);
exit:
    return hr;
}

xaml_result main_impl()
{
    xaml_result hr;
    char const str[] = U("Hello world!");
    xaml_box* str_box;
    XAML_GOTO_IF_FAILED(xaml_box_new(&xaml_guid_string, str, sizeof(str), &str_box), exit);
    xaml_observable_vector* vec;
    XAML_GOTO_IF_FAILED(xaml_observable_vector_new(&vec), clean_str);
    xaml_delegate* callback;
    XAML_GOTO_IF_FAILED(xaml_delegate_new(observable_vector_changed_callback, &callback), clean_vec);
    int32_t token;
    XAML_GOTO_IF_FAILED(vec->vtbl->add_vector_changed(vec, callback, &token), clean_callback);
    XAML_GOTO_IF_FAILED(vec->vtbl->append(vec, (xaml_object*)str_box), clean_callback);
    XAML_GOTO_IF_FAILED(vec->vtbl->remove_at(vec, 0), clean_callback);
clean_callback:
    callback->vtbl->release(callback);
clean_vec:
    vec->vtbl->release(vec);
clean_str:
    str_box->vtbl->release(str_box);
exit:
    return hr;
}

int main()
{
    XAML_ASSERT_SUCCEEDED(main_impl());
}
