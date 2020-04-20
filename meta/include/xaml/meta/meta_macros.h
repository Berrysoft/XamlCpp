#ifndef XAML_META_META_MACROS_H
#define XAML_META_META_MACROS_H

#include <xaml/delegate.h>
#include <xaml/object.h>

#define XAML_PROP(name, type, gtype, stype) \
    XAML_METHOD(get_##name, type, gtype);   \
    XAML_METHOD(set_##name, type, stype)

#define XAML_EVENT(name, type)                                        \
    XAML_METHOD(add_##name, type, xaml_delegate*, XAML_CSTD size_t*); \
    XAML_METHOD(remove_##name, type, XAML_CSTD size_t)

#endif // !XAML_META_META_MACROS_H
