#ifndef XAML_CMDLINE_DESERIALIZER_H
#define XAML_CMDLINE_DESERIALIZER_H

#include <xaml/cmdline/parser.h>

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_deserialize(xaml_type_info*, xaml_cmdline_options*, xaml_object**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
xaml_result XAML_CALL xaml_cmdline_deserialize(xaml_meta_context* ctx, xaml_vector_view* args, T** ptr) noexcept
{
    xaml_ptr<xaml_reflection_info> info;
    XAML_RETURN_IF_FAILED(ctx->get_type<T>(&info));
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    xaml_ptr<xaml_cmdline_options> opts;
    XAML_RETURN_IF_FAILED(xaml_cmdline_parse(t.get(), args, &opts));
    xaml_ptr<xaml_object> obj;
    XAML_RETURN_IF_FAILED(xaml_cmdline_deserialize(t.get(), opts.get(), &obj));
    return obj->query(ptr);
}

template <typename T>
xaml_result XAML_CALL xaml_cmdline_deserialize(xaml_meta_context* ctx, int argc, xaml_char_t** argv, T** ptr) noexcept
{
    xaml_ptr<xaml_reflection_info> info;
    XAML_RETURN_IF_FAILED(ctx->get_type<T>(&info));
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    xaml_ptr<xaml_cmdline_options> opts;
    XAML_RETURN_IF_FAILED(xaml_cmdline_parse_argv(t.get(), argc, argv, &opts));
    xaml_ptr<xaml_object> obj;
    XAML_RETURN_IF_FAILED(xaml_cmdline_deserialize(t.get(), opts.get(), &obj));
    return obj->query(ptr);
}
#endif // __cplusplus

#endif // !XAML_CMDLINE_DESERIALIZER_H
