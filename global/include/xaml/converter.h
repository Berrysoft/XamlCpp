#ifndef XAML_CONVERTER_H
#define XAML_CONVERTER_H

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_converter, { 0x157799af, 0x4ffe, 0x488a, { 0x8d, 0xa1, 0xc8, 0x22, 0xfa, 0x6f, 0xad, 0x20 } })

#define XAML_CONVERTER_VTBL(type)                                                                                  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                                                     \
    XAML_METHOD(convert, type, xaml_object*, xaml_guid XAML_CONST_REF, xaml_object*, xaml_string*, xaml_object**); \
    XAML_METHOD(convert_back, type, xaml_object*, xaml_guid XAML_CONST_REF, xaml_object*, xaml_string*, xaml_object**)

XAML_DECL_INTERFACE_(xaml_converter, xaml_object)
{
    XAML_DECL_VTBL(xaml_converter, XAML_CONVERTER_VTBL);
};

#endif // !XAML_CONVERTER_H
