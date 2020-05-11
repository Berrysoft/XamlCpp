#include <gtk3/gstring.hpp>
#include <gtk3/resources.hpp>
#include <shared/filebox.hpp>
#include <xaml/ui/filebox.h>
#include <xaml/ui/gtk3/window.h>

using namespace std;

template <typename I>
xaml_result xaml_filebox_impl<I>::show(xaml_window* owner) noexcept
{
    char const* title = nullptr;
    if (m_title)
    {
        XAML_RETURN_IF_FAILED(m_title->get_data(&title));
    }
    GtkWidget* parent_handle = nullptr;
    if (owner)
    {
        xaml_ptr<xaml_gtk3_window> native_control;
        if (XAML_SUCCEEDED(owner->query(&native_control)))
        {
            XAML_RETURN_IF_FAILED(native_control->get_window(&parent_handle));
        }
    }
    GtkWidget* handle;
    if constexpr (std::is_same_v<I, xaml_open_filebox>)
    {
        handle = gtk_file_chooser_dialog_new(title, GTK_WINDOW(parent_handle), GTK_FILE_CHOOSER_ACTION_OPEN, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Open"), GTK_RESPONSE_ACCEPT, nullptr);
    }
    else
    {
        handle = gtk_file_chooser_dialog_new(title, GTK_WINDOW(parent_handle), GTK_FILE_CHOOSER_ACTION_SAVE, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Save"), GTK_RESPONSE_ACCEPT, nullptr);
    }
    if (m_filename)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_filename->get_data(&data));
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(handle), data);
    }
    auto filter = gtk_file_filter_new();
    XAML_FOREACH_START(f, m_filters);
    {
        xaml_filebox_filter ff;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(f.get(), ff));
        gtk_file_filter_add_pattern(filter, ff.pattern);
    }
    XAML_FOREACH_END();
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(handle), filter);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(handle), m_multiple);
    auto res = gtk_dialog_run(GTK_DIALOG(handle));
    if (res != GTK_RESPONSE_ACCEPT) return XAML_E_FAIL;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_results));
    if (m_multiple)
    {
        g_slist_free_unique_ptr list{ gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(handle)) };
        while (list)
        {
            g_free_unique_ptr<gchar> name{ (gchar*)list->data };
            xaml_ptr<xaml_string> xaml_name;
            XAML_RETURN_IF_FAILED(xaml_string_new_gstring(move(name), &xaml_name));
            XAML_RETURN_IF_FAILED(m_results->append(xaml_name.get()));
            list.reset(list->next);
        }
    }
    else
    {
        g_free_unique_ptr<gchar> name{ gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(handle)) };
        xaml_ptr<xaml_string> xaml_name;
        XAML_RETURN_IF_FAILED(xaml_string_new_gstring(move(name), &xaml_name));
        XAML_RETURN_IF_FAILED(m_results->append(xaml_name.get()));
    }
    gtk_widget_destroy(handle);
    return XAML_S_OK;
}

template struct xaml_filebox_impl<xaml_open_filebox>;
template struct xaml_filebox_impl<xaml_save_filebox>;
