#ifndef XAML_PARSER_NODE_H
#define XAML_PARSER_NODE_H

#include <xaml/meta/event_info.h>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/meta/type_info.h>

XAML_CLASS(xaml_node_base, { 0x9a601cde, 0xec14, 0x4d03, { 0x90, 0xab, 0x80, 0x79, 0x1a, 0x6b, 0x20, 0xb4 } })

#define XAML_NODE_BASE_VTBL(_type)                             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(_type));                \
    XAML_PROP(type, _type, xaml_type_info**, xaml_type_info*); \
    XAML_PROP(name, _type, xaml_string**, xaml_string*);       \
    XAML_PROP(key, _type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_node_base, xaml_object)
{
    XAML_DECL_VTBL(xaml_node_base, XAML_NODE_BASE_VTBL);
};

XAML_CLASS(xaml_string_node, { 0xf8003714, 0xdd89, 0x4089, { 0x9c, 0xd0, 0xd4, 0xe4, 0xd7, 0xf6, 0x3f, 0x42 } })

#define XAML_STRING_NODE_VTBL(type)               \
    XAML_VTBL_INHERIT(XAML_NODE_BASE_VTBL(type)); \
    XAML_PROP(value, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_string_node, xaml_node_base)
{
    XAML_DECL_VTBL(xaml_string_node, XAML_STRING_NODE_VTBL);
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_string_node_new(xaml_string_node**) XAML_NOEXCEPT;

typedef struct xaml_attribute_property xaml_attribute_property;

#ifndef xaml_enumerator_1__xaml_attribute_property_defined
    #define xaml_enumerator_1__xaml_attribute_property_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_attribute_property))
#endif // !xaml_enumerator_1__xaml_attribute_property_defined

#ifndef xaml_vector_1__xaml_attribute_property_defined
    #define xaml_vector_1__xaml_attribute_property_defined
XAML_VECTOR_1_TYPE(XAML_T_O(xaml_attribute_property))
#endif // !xaml_vector_1__xaml_attribute_property_defined

XAML_CLASS(xaml_markup_node, { 0xa45401da, 0xa28d, 0x48bd, { 0x90, 0x4f, 0x7c, 0x53, 0x68, 0x04, 0x0f, 0xec } })

#define XAML_MARKUP_NODE_VTBL(type)               \
    XAML_VTBL_INHERIT(XAML_NODE_BASE_VTBL(type)); \
    XAML_PROP(properties, type, XAML_VECTOR_1_NAME(xaml_attribute_property)**, XAML_VECTOR_1_NAME(xaml_attribute_property)*)

XAML_DECL_INTERFACE_(xaml_markup_node, xaml_node_base)
{
    XAML_DECL_VTBL(xaml_markup_node, XAML_MARKUP_NODE_VTBL);
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_markup_node_new(xaml_markup_node**) XAML_NOEXCEPT;

XAML_CLASS(xaml_node, { 0x85ad34c8, 0x3cba, 0x44eb, { 0x98, 0x1c, 0x67, 0xf7, 0x13, 0xd3, 0xe5, 0xeb } })

#ifndef xaml_key_value_pair_2__xaml_string__xaml_node_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_node_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_node))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_node_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_node_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_node_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_node))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_node_defined

#ifndef xaml_map_2__xaml_string__xaml_node_defined
    #define xaml_map_2__xaml_string__xaml_node_defined
XAML_MAP_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_node))
#endif // !xaml_map_2__xaml_string__xaml_node_defined

typedef struct xaml_attribute_property xaml_attribute_property;

#ifndef xaml_enumerator_1__xaml_attribute_property_defined
    #define xaml_enumerator_1__xaml_attribute_property_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_attribute_property))
#endif // !xaml_enumerator_1__xaml_attribute_property_defined

#ifndef xaml_vector_1__xaml_attribute_property_defined
    #define xaml_vector_1__xaml_attribute_property_defined
XAML_VECTOR_1_TYPE(XAML_T_O(xaml_attribute_property))
#endif // !xaml_vector_1__xaml_attribute_property_defined

typedef struct xaml_attribute_collection_property xaml_attribute_collection_property;

#ifndef xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_attribute_collection_property))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property_define

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property_defined
XAML_ENUMERABLE_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_attribute_collection_property_defined

#ifndef xaml_map_2__xaml_string__xaml_attribute_collection_property_defined
    #define xaml_map_2__xaml_string__xaml_attribute_collection_property_defined
XAML_MAP_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_attribute_collection_property))
#endif // !xaml_map_2__xaml_string__xaml_attribute_collection_property_defined

typedef struct xaml_attribute_event xaml_attribute_event;

#ifndef xaml_enumerator_1__xaml_attribute_event_defined
    #define xaml_enumerator_1__xaml_attribute_event_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_attribute_event))
#endif // !xaml_enumerator_1__xaml_attribute_event_defined

#ifndef xaml_vector_1__xaml_attribute_event_defined
    #define xaml_vector_1__xaml_attribute_event_defined
XAML_VECTOR_1_TYPE(XAML_T_O(xaml_attribute_event))
#endif // !xaml_vector_1__xaml_attribute_event_defined

#define XAML_NODE_VTBL(type)                                                                                                                                                       \
    XAML_VTBL_INHERIT(XAML_NODE_BASE_VTBL(type));                                                                                                                                  \
    XAML_PROP(resources, type, XAML_MAP_2_NAME(xaml_string, xaml_node)**, XAML_MAP_2_NAME(xaml_string, xaml_node)*);                                                               \
    XAML_PROP(properties, type, XAML_VECTOR_1_NAME(xaml_attribute_property)**, XAML_VECTOR_1_NAME(xaml_attribute_property)*);                                                      \
    XAML_PROP(collection_properties, type, XAML_MAP_2_NAME(xaml_string, xaml_attribute_collection_property)**, XAML_MAP_2_NAME(xaml_string, xaml_attribute_collection_property)*); \
    XAML_PROP(events, type, XAML_VECTOR_1_NAME(xaml_attribute_event)**, XAML_VECTOR_1_NAME(xaml_attribute_event)*)

XAML_DECL_INTERFACE_(xaml_node, xaml_node_base)
{
    XAML_DECL_VTBL(xaml_node, XAML_NODE_VTBL);
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_node_new(xaml_node**) XAML_NOEXCEPT;

XAML_CLASS(xaml_attribute_event, { 0x79d5dc01, 0xf5e7, 0x40b5, { 0x9b, 0xd6, 0x1b, 0x76, 0x60, 0x68, 0x77, 0x96 } })

#define XAML_ATTRIBUTE_EVENT_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));      \
    XAML_METHOD(get_info, type, xaml_event_info**); \
    XAML_METHOD(get_value, type, xaml_string**)

XAML_DECL_INTERFACE_(xaml_attribute_event, xaml_object)
{
    XAML_DECL_VTBL(xaml_attribute_event, XAML_ATTRIBUTE_EVENT_VTBL);
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_attribute_event_new(xaml_event_info*, xaml_string*, xaml_attribute_event**) XAML_NOEXCEPT;

XAML_CLASS(xaml_attribute_property, { 0x82c20c0f, 0xc1cd, 0x418a, { 0x84, 0x76, 0x73, 0xd2, 0x6e, 0xf3, 0x15, 0x0a } })

#define XAML_ATTRIBUTE_PROPERTY_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));         \
    XAML_METHOD(get_type, type, xaml_type_info**);     \
    XAML_METHOD(get_info, type, xaml_property_info**); \
    XAML_METHOD(get_value, type, xaml_node_base**)

XAML_DECL_INTERFACE_(xaml_attribute_property, xaml_object)
{
    XAML_DECL_VTBL(xaml_attribute_property, XAML_ATTRIBUTE_PROPERTY_VTBL);
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_attribute_property_new(xaml_type_info*, xaml_property_info*, xaml_node_base*, xaml_attribute_property**) XAML_NOEXCEPT;

#ifndef xaml_enumerator_1__xaml_node_defined
    #define xaml_enumerator_1__xaml_node_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_node))
#endif // !xaml_enumerator_1__xaml_node_defined

#ifndef xaml_vector_1__xaml_node_defined
    #define xaml_vector_1__xaml_node_defined
XAML_VECTOR_1_TYPE(XAML_T_O(xaml_node))
#endif // !xaml_vector_1__xaml_node_defined

XAML_CLASS(xaml_attribute_collection_property, { 0x9e34786c, 0x06f8, 0x493c, { 0xa5, 0xb4, 0x39, 0x8f, 0x28, 0x70, 0x04, 0x79 } })

#define XAML_ATTRIBUTE_COLLECTION_PROPERTY_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                    \
    XAML_METHOD(get_type, type, xaml_type_info**);                \
    XAML_METHOD(get_info, type, xaml_collection_property_info**); \
    XAML_METHOD(get_values, type, XAML_VECTOR_1_NAME(xaml_node)**)

XAML_DECL_INTERFACE_(xaml_attribute_collection_property, xaml_object)
{
    XAML_DECL_VTBL(xaml_attribute_collection_property, XAML_ATTRIBUTE_COLLECTION_PROPERTY_VTBL);
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_attribute_collection_property_new(xaml_type_info*, xaml_collection_property_info*, XAML_VECTOR_1_NAME(xaml_node) *, xaml_attribute_collection_property**) XAML_NOEXCEPT;

#endif // !XAML_PARSER_NODE_H
