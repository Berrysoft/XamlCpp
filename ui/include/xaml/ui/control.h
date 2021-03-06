#ifndef XAML_UI_CONTROL_H
#define XAML_UI_CONTROL_H

#include <xaml/markup/element_base.h>
#include <xaml/ui/drawing.h>

typedef enum xaml_halignment
{
    xaml_halignment_stretch,
    xaml_halignment_left,
    xaml_halignment_center,
    xaml_halignment_right
} xaml_halignment;

XAML_TYPE(xaml_halignment, { 0x65614483, 0x6bb3, 0x407d, { 0x89, 0xda, 0x41, 0x2f, 0xfd, 0x99, 0xcf, 0xa8 } })

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_halignment_register(xaml_meta_context*) XAML_NOEXCEPT;

typedef enum xaml_valignment
{
    xaml_valignment_stretch,
    xaml_valignment_top,
    xaml_valignment_center,
    xaml_valignment_bottom
} xaml_valignment;

XAML_TYPE(xaml_valignment, { 0x71173440, 0xcbf1, 0x4fe9, { 0xbd, 0x93, 0x27, 0x0b, 0x73, 0x8b, 0x44, 0x65 } })

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_valignment_register(xaml_meta_context*) XAML_NOEXCEPT;

typedef enum xaml_mouse_button
{
    xaml_mouse_button_left,
    xaml_mouse_button_right,
    xaml_mouse_button_middle,
    xaml_mouse_button_other
} xaml_mouse_button;

XAML_TYPE(xaml_mouse_button, { 0x02160c5f, 0x4507, 0x4347, { 0x91, 0xa9, 0xcd, 0x15, 0xcd, 0x9f, 0xe3, 0x55 } })

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_mouse_button_register(xaml_meta_context*) XAML_NOEXCEPT;

#ifndef xaml_delegate_2__xaml_object__xaml_size_defined
    #define xaml_delegate_2__xaml_object__xaml_size_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_size))
#endif // !xaml_delegate_2__xaml_object__xaml_size_defined

#ifndef xaml_delegate_2__xaml_object__xaml_margin_defined
    #define xaml_delegate_2__xaml_object__xaml_margin_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_margin))
#endif // !xaml_delegate_2__xaml_object__xaml_margin_defined

#ifndef xaml_delegate_2__xaml_object__xaml_halignment_defined
    #define xaml_delegate_2__xaml_object__xaml_halignment_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_halignment))
#endif // !xaml_delegate_2__xaml_object__xaml_halignment_defined

#ifndef xaml_delegate_2__xaml_object__xaml_valignment_defined
    #define xaml_delegate_2__xaml_object__xaml_valignment_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_valignment))
#endif // !xaml_delegate_2__xaml_object__xaml_valignment_defined

#ifndef xaml_delegate_2__xaml_object__xaml_bool_defined
    #define xaml_delegate_2__xaml_object__xaml_bool_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_bool))
#endif // !xaml_delegate_2__xaml_object__xaml_bool_defined

#ifndef xaml_delegate_2__xaml_object__xaml_mouse_button_defined
    #define xaml_delegate_2__xaml_object__xaml_mouse_button_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_mouse_button))
#endif // !xaml_delegate_2__xaml_object__xaml_mouse_button_defined

#ifndef xaml_delegate_2__xaml_object__xaml_point_defined
    #define xaml_delegate_2__xaml_object__xaml_point_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_V(xaml_point))
#endif // !xaml_delegate_2__xaml_object__xaml_point_defined

XAML_CLASS(xaml_control, { 0x389f559a, 0x48bb, 0x49a7, { 0xa0, 0x16, 0x1d, 0xcb, 0x95, 0x72, 0x72, 0xa2 } })

#define XAML_CONTROL_VTBL(type)                                         \
    XAML_VTBL_INHERIT(XAML_ELEMENT_BASE_VTBL(type));                    \
    XAML_PROP(size, type, xaml_size*, xaml_size XAML_CONST_REF);        \
    XAML_PROP(width, type, double*, double);                            \
    XAML_PROP(height, type, double*, double);                           \
    XAML_EVENT(size_changed, type, xaml_object, xaml_size);             \
    XAML_PROP(margin, type, xaml_margin*, xaml_margin XAML_CONST_REF);  \
    XAML_EVENT(margin_changed, type, xaml_object, xaml_margin);         \
    XAML_PROP(halignment, type, xaml_halignment*, xaml_halignment);     \
    XAML_EVENT(halignment_changed, type, xaml_object, xaml_halignment); \
    XAML_PROP(valignment, type, xaml_valignment*, xaml_valignment);     \
    XAML_EVENT(valignment_changed, type, xaml_object, xaml_valignment); \
    XAML_PROP(is_visible, type, bool*, bool);                           \
    XAML_EVENT(is_visible_changed, type, xaml_object, xaml_bool);       \
    XAML_METHOD(get_is_initialized, type, bool*);                       \
    XAML_METHOD(draw, type, xaml_rectangle XAML_CONST_REF);             \
    XAML_EVENT(mouse_down, type, xaml_object, xaml_mouse_button);       \
    XAML_EVENT(mouse_up, type, xaml_object, xaml_mouse_button);         \
    XAML_EVENT(mouse_move, type, xaml_object, xaml_point);              \
    XAML_METHOD(size_to_fit, type);                                     \
    XAML_METHOD(parent_redraw, type);                                   \
    XAML_METHOD(set_size_noevent, type, xaml_size XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_control, xaml_element_base)
{
    XAML_DECL_VTBL(xaml_control, XAML_CONTROL_VTBL);
};

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_control_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_control_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROL_H
