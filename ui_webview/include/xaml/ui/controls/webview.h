#ifndef XAML_UI_WEBVIEW_WEBVIEW_H
#define XAML_UI_WEBVIEW_WEBVIEW_H

#include <xaml/buffer.h>
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

EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_webview_new(xaml_webview**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_webview_members(xaml_reflection_info*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_webview_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_webview_web_request, { 0xa1c28b95, 0x1a77, 0x4cb4, { 0xa3, 0xbd, 0xd4, 0x3a, 0x71, 0x47, 0x94, 0xc4 } })

#define XAML_WEBVIEW_WEB_REQUEST_VTBL(type)               \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));            \
    XAML_PROP(method, type, xaml_string**, xaml_string*); \
    XAML_PROP(uri, type, xaml_string**, xaml_string*);    \
    XAML_PROP(data, type, xaml_buffer**, xaml_buffer*)

XAML_DECL_INTERFACE_(xaml_webview_web_request, xaml_object)
{
    XAML_DECL_VTBL(xaml_webview_web_request, XAML_WEBVIEW_WEB_REQUEST_VTBL);
};

EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_webview_web_request_new(xaml_webview_web_request**) XAML_NOEXCEPT;

XAML_CLASS(xaml_webview_web_response, { 0x8825e9f8, 0xa8d2, 0x44f1, { 0xa6, 0xa0, 0x76, 0x93, 0xfc, 0xbe, 0xd4, 0x1b } })

#define XAML_WEBVIEW_WEB_RESPONSE_VTBL(type)                    \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                  \
    XAML_PROP(content_type, type, xaml_string**, xaml_string*); \
    XAML_PROP(data, type, xaml_buffer**, xaml_buffer*)

XAML_DECL_INTERFACE_(xaml_webview_web_response, xaml_object)
{
    XAML_DECL_VTBL(xaml_webview_web_response, XAML_WEBVIEW_WEB_RESPONSE_VTBL);
};

EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_webview_web_response_new(xaml_webview_web_response**) XAML_NOEXCEPT;

XAML_CLASS(xaml_webview_resource_requested_args, { 0x507c59c1, 0x5775, 0x484e, { 0x9c, 0xe5, 0xbc, 0x34, 0x23, 0x85, 0x5a, 0x0b } })

#define XAML_WEBVIEW_RESOURCE_REQUESTED_ARGS_VTBL(type)         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                  \
    XAML_METHOD(get_request, type, xaml_webview_web_request**); \
    XAML_PROP(response, type, xaml_webview_web_response**, xaml_webview_web_response*)

XAML_DECL_INTERFACE_(xaml_webview_resource_requested_args, xaml_object)
{
    XAML_DECL_VTBL(xaml_webview_resource_requested_args, XAML_WEBVIEW_RESOURCE_REQUESTED_ARGS_VTBL);
};

EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_webview_resource_requested_args_new(xaml_webview_resource_requested_args**) XAML_NOEXCEPT;

#endif // !XAML_UI_WEBVIEW_WEBVIEW_H
