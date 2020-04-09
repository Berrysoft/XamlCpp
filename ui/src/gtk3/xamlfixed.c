#include <stdlib.h>
#include <xaml/ui/gtk3/xamlfixed.h>

typedef struct _XamlFixedPrivate
{
    GHashTable* children;
} XamlFixedPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(XamlFixed, xaml_fixed, GTK_TYPE_CONTAINER);

#define XAML_FIXED_PRIVATE(obj) (xaml_fixed_get_instance_private(XAML_FIXED(obj)))

static void xaml_fixed_destroy(GtkWidget* widget)
{
    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(widget);
    if (priv->children)
    {
        g_hash_table_destroy(priv->children);
        priv->children = NULL;
    }
}

static void get_size(XamlFixed* self, GtkOrientation direction, gint* minimal, gint* natural)
{
    guint border = gtk_container_get_border_width(GTK_CONTAINER(self));
    *minimal = *natural = border * 2;

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(self);

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, priv->children);
    gint size_natural = 0;
    GtkWidget* key;
    GtkAllocation* value;
    while (g_hash_table_iter_next(&iter, (void**)&key, (void**)&value))
    {
        if (!gtk_widget_get_visible(key)) continue;

        if (direction == GTK_ORIENTATION_HORIZONTAL)
        {
            gint ww;
            gtk_widget_get_preferred_width(key, NULL, &ww);
            size_natural = MAX(size_natural, value->x + ww);
        }
        else
        {
            gint wh;
            gtk_widget_get_preferred_height(key, NULL, &wh);
            size_natural = MAX(size_natural, value->y + wh);
        }
    }
    *natural += size_natural;
}

static void xaml_fixed_get_preferred_width(GtkWidget* widget, gint* minimal, gint* natural)
{
    get_size(XAML_FIXED(widget), GTK_ORIENTATION_HORIZONTAL, minimal, natural);
}

static void xaml_fixed_get_preferred_height(GtkWidget* widget, gint* minimal, gint* natural)
{
    get_size(XAML_FIXED(widget), GTK_ORIENTATION_VERTICAL, minimal, natural);
}

static void xaml_fixed_size_allocate(GtkWidget* widget, GtkAllocation* allocation)
{
    g_return_if_fail(allocation);

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(widget);

    gtk_widget_set_allocation(widget, allocation);

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, priv->children);
    GtkWidget* key;
    GtkAllocation* value;
    while (g_hash_table_iter_next(&iter, (void**)&key, (void**)&value))
    {
        if (!gtk_widget_get_visible(key)) continue;

        GtkAllocation child_allocation = *value;
        child_allocation.x += allocation->x;
        child_allocation.y += allocation->y;
        gtk_widget_size_allocate(key, &child_allocation);
    }
}

static GType xaml_fixed_child_type(GtkContainer* container)
{
    return GTK_TYPE_WIDGET;
}

static void xaml_fixed_add(GtkContainer* container, GtkWidget* widget)
{
    g_return_if_fail(widget);

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(container);

    if (!g_hash_table_contains(priv->children, widget))
    {
        gtk_widget_unparent(widget);
        GtkAllocation* alloc = g_new(GtkAllocation, 1);
        g_hash_table_insert(priv->children, widget, alloc);
        gtk_widget_set_parent(widget, GTK_WIDGET(container));

        if (gtk_widget_get_visible(widget))
            gtk_widget_queue_resize(GTK_WIDGET(container));
    }
}

static void xaml_fixed_remove(GtkContainer* container, GtkWidget* widget)
{
    g_return_if_fail(widget);

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(container);

    g_hash_table_remove(priv->children, widget);
    gtk_widget_unparent(widget);
}

static void xaml_fixed_forall(GtkContainer* container, gboolean include_internals, GtkCallback callback, gpointer callback_data)
{
    g_return_if_fail(callback);

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(container);

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, priv->children);
    GtkWidget* key;
    while (g_hash_table_iter_next(&iter, (void**)&key, NULL))
    {
        callback(key, callback_data);
    }
}

static void xaml_fixed_class_init(XamlFixedClass* klass)
{
    GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
    widget_class->destroy = xaml_fixed_destroy;
    widget_class->get_preferred_width = xaml_fixed_get_preferred_width;
    widget_class->get_preferred_height = xaml_fixed_get_preferred_height;
    widget_class->size_allocate = xaml_fixed_size_allocate;

    GtkContainerClass* container_class = GTK_CONTAINER_CLASS(klass);
    container_class->child_type = xaml_fixed_child_type;
    container_class->add = xaml_fixed_add;
    container_class->remove = xaml_fixed_remove;
    container_class->forall = xaml_fixed_forall;
}

static void xaml_fixed_init(XamlFixed* fixed)
{
    gtk_widget_set_has_window(GTK_WIDGET(fixed), FALSE);

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(fixed);
    priv->children = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
}

GtkWidget* xaml_fixed_new(void)
{
    return GTK_WIDGET(g_object_new(xaml_fixed_get_type(), NULL));
}

void xaml_fixed_child_size_allocate(XamlFixed* self, GtkWidget* child, GtkAllocation* alloc)
{
    g_return_if_fail(self);
    g_return_if_fail(child);
    g_return_if_fail(alloc);

    if (alloc->width < 0) alloc->width = 0;
    if (alloc->height < 1) alloc->height = 1;

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(self);

    GtkAllocation* child_alloc = g_hash_table_lookup(priv->children, child);
    g_return_if_fail(child_alloc);

    *child_alloc = *alloc;

    if (gtk_widget_get_visible(child))
        gtk_widget_queue_resize(GTK_WIDGET(self));
}
