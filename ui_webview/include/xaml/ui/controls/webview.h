#ifndef XAML_UI_WEBVIEW_WEBVIEW_H
#define XAML_UI_WEBVIEW_WEBVIEW_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_webview, { 0xb39028bb, 0xc65f, 0x4df9, { 0xa0, 0xef, 0xf2, 0x04, 0x30, 0x77, 0x40, 0xda } })

#define XAML_WEBVIEW_VTBL(type)                        \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));        \
    XAML_PROP(uri, type, xaml_string**, xaml_string*); \
    XAML_EVENT(uri_changed, type);                     \
    XAML_METHOD(get_can_go_forward, type, bool*);      \
    XAML_METHOD(get_can_go_back, type, bool*);         \
    XAML_METHOD(go_forward, type);                     \
    XAML_METHOD(go_back, type);                        \
    XAML_EVENT(resource_requested, type)

XAML_DECL_INTERFACE_(xaml_webview, xaml_control)
{
    XAML_DECL_VTBL(xaml_webview, XAML_WEBVIEW_VTBL);
};

typedef struct xaml_webview_web_request
{
    xaml_char_t const* method;
    xaml_char_t const* uri;
    XAML_CSTD uint8_t* data;
    XAML_CSTD int32_t length;
} xaml_webview_web_request;

typedef struct xaml_webview_web_response
{
    xaml_char_t const* content_type;
    XAML_CSTD uint8_t* data;
    XAML_CSTD int32_t length;
} xaml_webview_web_response;

XAML_CLASS(xaml_webview_resource_requested_args, { 0x507c59c1, 0x5775, 0x484e, { 0x9c, 0xe5, 0xbc, 0x34, 0x23, 0x85, 0x5a, 0x0b } })

#define XAML_WEBVIEW_RESOURCE_REQUESTED_ARGS_VTBL(type) \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));          \
    XAML_METHOD(get_request, type);                     \
    XAML_PROP(response, type)

#endif // !XAML_UI_WEBVIEW_WEBVIEW_H
