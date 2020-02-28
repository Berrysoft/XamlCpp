#include <gtk3/xamlfixed.h>
#include <stdlib.h>

#define XAML_FIXED_PRIVATE(obj) (XamlFixedPrivate*)xaml_fixed_get_instance_private(XAML_FIXED(obj))

typedef struct _XamlFixedPrivate XamlFixedPrivate;

struct _XamlFixedPrivate
{
    GList* children;
};

typedef struct _XamlFixedWidget XamlFixedWidget;

struct _XamlFixedWidget
{
    GtkWidget* widget;
    GtkAllocation alloc;
};

static XamlFixedWidget* xaml_fixed_widget_new(GtkWidget* widget, GtkAllocation* alloc)
{
    XamlFixedWidget* result = malloc(sizeof(XamlFixedWidget));
    if (result)
    {
        result->widget = widget;
        result->alloc = *alloc;
    }
    return result;
}

static void xaml_fixed_widget_destroy(XamlFixedWidget* widget)
{
    free(widget);
}

/* Forward declarations */
static void xaml_fixed_get_preferred_width(GtkWidget* widget, gint* minimal, gint* natural);
static void xaml_fixed_get_preferred_height(GtkWidget* widget, gint* minimal, gint* natural);
static void xaml_fixed_size_allocate(GtkWidget* widget, GtkAllocation* allocation);
static GType xaml_fixed_child_type(GtkContainer* container);
static void xaml_fixed_add(GtkContainer* container, GtkWidget* widget);
static void xaml_fixed_remove(GtkContainer* container, GtkWidget* widget);
static void xaml_fixed_forall(GtkContainer* container, gboolean include_internals, GtkCallback callback, gpointer callback_data);

G_DEFINE_TYPE_WITH_PRIVATE(XamlFixed, xaml_fixed, GTK_TYPE_CONTAINER);

static void xaml_fixed_class_init(XamlFixedClass* klass)
{
    /* Override GtkWidget methods */
    GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
    widget_class->get_preferred_width = xaml_fixed_get_preferred_width;
    widget_class->get_preferred_height = xaml_fixed_get_preferred_height;
    widget_class->size_allocate = xaml_fixed_size_allocate;

    /* Override GtkContainer methods */
    GtkContainerClass* container_class = GTK_CONTAINER_CLASS(klass);
    container_class->child_type = xaml_fixed_child_type;
    container_class->add = xaml_fixed_add;
    container_class->remove = xaml_fixed_remove;
    container_class->forall = xaml_fixed_forall;
}

static void xaml_fixed_init(XamlFixed* fixed)
{
    gtk_widget_set_has_window(GTK_WIDGET(fixed), FALSE);

    /* Initialize private members */
    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(fixed);
    priv->children = NULL;
}

GtkWidget* xaml_fixed_new()
{
    return GTK_WIDGET(g_object_new(xaml_fixed_get_type(), NULL));
}

static void get_size(XamlFixed* self, GtkOrientation direction, gint* minimal, gint* natural)
{
    guint border = gtk_container_get_border_width(GTK_CONTAINER(self));
    *minimal = *natural = border * 2;

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(self);
    GList* iter;
    for (iter = priv->children; iter; iter = g_list_next(iter))
    {
        XamlFixedWidget* w = iter->data;

        if (!gtk_widget_get_visible(w->widget))
            continue;

        if (direction == GTK_ORIENTATION_HORIZONTAL)
        {
            gint ww;
            gtk_widget_get_preferred_width(w->widget, NULL, &ww);
            *natural = MAX(*natural, w->alloc.x + ww);
        }
        else
        {
            gint wh;
            gtk_widget_get_preferred_height(w->widget, NULL, &wh);
            *natural = MAX(*natural, w->alloc.y + wh);
        }
    }
}

/* Get the width of the container */
static void xaml_fixed_get_preferred_width(GtkWidget* widget, gint* minimal, gint* natural)
{
    g_return_if_fail(widget != NULL);
    g_return_if_fail(XAML_IS_FIXED(widget));

    get_size(XAML_FIXED(widget), GTK_ORIENTATION_HORIZONTAL, minimal, natural);
}

/* Get the height of the container */
static void xaml_fixed_get_preferred_height(GtkWidget* widget, gint* minimal, gint* natural)
{
    g_return_if_fail(widget != NULL);
    g_return_if_fail(XAML_IS_FIXED(widget));

    get_size(XAML_FIXED(widget), GTK_ORIENTATION_VERTICAL, minimal, natural);
}

/* Allocate the sizes of the container's children */
static void xaml_fixed_size_allocate(GtkWidget* widget, GtkAllocation* allocation)
{
    g_return_if_fail(widget != NULL || allocation != NULL);
    g_return_if_fail(XAML_IS_FIXED(widget));

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(widget);

    gtk_widget_set_allocation(widget, allocation);

    GList* iter;
    for (iter = priv->children; iter; iter = g_list_next(iter))
    {
        XamlFixedWidget* w = iter->data;

        if (!gtk_widget_get_visible(w->widget))
            continue;

        /* Give the child its allocation */
        GtkAllocation child_allocation = w->alloc;
        child_allocation.x += allocation->x;
        child_allocation.y += allocation->y;
        gtk_widget_size_allocate(w->widget, &child_allocation);
    }
}

static GType xaml_fixed_child_type(GtkContainer* container)
{
    return GTK_TYPE_WIDGET;
}

static void xaml_fixed_add(GtkContainer* container, GtkWidget* widget)
{
    g_return_if_fail(container || XAML_IS_FIXED(container));
    g_return_if_fail(widget || GTK_IS_WIDGET(widget));
    g_return_if_fail(gtk_widget_get_parent(widget) == NULL);

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(container);

    /* Add the child to our list of children. 
     * All the real work is done in gtk_widget_set_parent(). */
    GtkAllocation alloc = { 0, 0, 0, 0 };
    XamlFixedWidget* w = xaml_fixed_widget_new(widget, &alloc);
    priv->children = g_list_append(priv->children, w);
    gtk_widget_set_parent(widget, GTK_WIDGET(container));

    /* Queue redraw */
    if (gtk_widget_get_visible(widget))
        gtk_widget_queue_resize(GTK_WIDGET(container));
}

static GList* g_list_find_xaml_fixed_widget(GList* list, GtkWidget* data)
{
    for (; list; list = g_list_next(list))
    {
        XamlFixedWidget* w = list->data;
        if (w->widget == data)
        {
            return list;
        }
    }
    return NULL;
}

static void xaml_fixed_remove(GtkContainer* container, GtkWidget* widget)
{
    g_return_if_fail(container || XAML_IS_FIXED(container));
    g_return_if_fail(widget || GTK_IS_WIDGET(widget));

    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(container);

    /* Remove the child from our list of children. 
     * Again, all the real work is done in gtk_widget_unparent(). */
    GList* link = g_list_find_xaml_fixed_widget(priv->children, widget);
    if (link)
    {
        gboolean was_visible = gtk_widget_get_visible(widget);
        gtk_widget_unparent(widget);

        xaml_fixed_widget_destroy(link->data);
        priv->children = g_list_delete_link(priv->children, link);

        /* Queue redraw */
        if (was_visible)
            gtk_widget_queue_resize(GTK_WIDGET(container));
    }
}

typedef struct _XamlFixedForallData XamlFixedForallData;

struct _XamlFixedForallData
{
    GFunc callback;
    gpointer user_data;
};

static void xaml_fixed_forall_callback(gpointer data, gpointer user_data)
{
    XamlFixedForallData* d = user_data;
    d->callback(((XamlFixedWidget*)data)->widget, d->user_data);
}

static void xaml_fixed_forall(GtkContainer* container, gboolean include_internals, GtkCallback callback, gpointer callback_data)
{
    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(container);
    XamlFixedForallData data = { (GFunc)callback, callback_data };
    g_list_foreach(priv->children, xaml_fixed_forall_callback, &data);
}

void xaml_fixed_child_size_allocate(XamlFixed* self, GtkWidget* child, GtkAllocation* alloc)
{
    XamlFixedPrivate* priv = XAML_FIXED_PRIVATE(self);

    GList* link = g_list_find_xaml_fixed_widget(priv->children, child);
    if (link)
    {
        XamlFixedWidget* w = link->data;
        gboolean was_visible = gtk_widget_get_visible(w->widget);
        w->alloc = *alloc;

        /* Queue redraw */
        if (was_visible)
            gtk_widget_queue_resize(GTK_WIDGET(self));
    }
}
