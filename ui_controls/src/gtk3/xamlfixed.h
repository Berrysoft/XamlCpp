#ifndef XAML_UI_NATIVE_FIXED_H
#define XAML_UI_NATIVE_FIXED_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define XAML_TYPE_FIXED (xaml_fixed_get_type())
#define XAML_FIXED(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), XAML_TYPE_FIXED, XamlFixed))
#define XAML_FIXED_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), XAML_TYPE_FIXED, XamlFixedClass))
#define XAML_IS_FIXED(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), XAML_TYPE_FIXED))
#define XAML_IS_FIXED_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), XAML_TYPE_FIXED))
#define XAML_FIXED_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), XAML_TYPE_FIXED, XamlFixedClass))

typedef struct _XamlFixed
{
    GtkContainer parent_instance;
} XamlFixed;

typedef struct _XamlFixedClass
{
    GtkContainerClass parent_class;
} XamlFixedClass;

GType xaml_fixed_get_type(void) G_GNUC_CONST;
GtkWidget* xaml_fixed_new(void);
void xaml_fixed_child_size_allocate(XamlFixed* self, GtkWidget* child, GtkAllocation* alloc);

G_END_DECLS

#endif // !XAML_UI_NATIVE_FIXED_H
