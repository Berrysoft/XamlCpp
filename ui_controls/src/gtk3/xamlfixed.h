#ifndef XAML_UI_NATIVE_FIXED_H
#define XAML_UI_NATIVE_FIXED_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define XAML_FIXED_TYPE (xaml_fixed_get_type())
#define XAML_FIXED(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), XAML_FIXED_TYPE, XamlFixed))
#define XAML_FIXED_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), XAML_FIXED_TYPE, XamlFixedClass))
#define XAML_IS_FIXED(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), XAML_FIXED_TYPE))
#define XAML_IS_FIXED_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), XAML_FIXED_TYPE))

typedef struct _XamlFixed XamlFixed;
typedef struct _XamlFixedClass XamlFixedClass;

struct _XamlFixed
{
    GtkContainer parent_instance;
};

struct _XamlFixedClass
{
    GtkContainerClass parent_class;
};

GType xaml_fixed_get_type(void) G_GNUC_CONST;
GtkWidget* xaml_fixed_new(void);
void xaml_fixed_child_size_allocate(XamlFixed* self, GtkWidget* child, GtkAllocation* alloc);

G_END_DECLS

#endif // !XAML_UI_NATIVE_FIXED_H
