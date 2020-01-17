#include <gtk3/resources.hpp>
#include <xaml/ui/filebox.hpp>

using namespace std;

namespace xaml
{
    bool filebox::show(shared_ptr<window> owner)
    {
        if (m_handle)
        {
            gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(m_handle), m_filename.c_str());
            auto filter = gtk_file_filter_new();
            for (auto& f : m_filters)
            {
                gtk_file_filter_add_pattern(filter, f.pattern.c_str());
            }
            gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(m_handle), filter);
            gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(m_handle), m_multiple);
            auto res = gtk_dialog_run(GTK_DIALOG(m_handle));
            bool ret = res == GTK_RESPONSE_ACCEPT;
            if (ret)
            {
                if (m_multiple)
                {
                    m_results.clear();
                    g_slist_free_unique_ptr<GSList> list{ gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(m_handle)) };
                    while (list)
                    {
                        g_free_unique_ptr<gchar> name{ (gchar*)list->data };
                        m_results.push_back(name.get());
                        list.reset(list->next);
                    }
                }
                else
                {
                    g_free_unique_ptr<gchar> name{ gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(m_handle)) };
                    m_results = { name.get() };
                }
            }
            gtk_widget_destroy(m_handle);
            m_handle = nullptr;
            return ret;
        }
        return false;
    }

    void filebox::show_async(shared_ptr<window> owner, function<void(bool)> callback)
    {
        auto res = show(owner);
        if (callback) callback(res);
    }

    bool open_filebox::show(shared_ptr<window> owner)
    {
        set_handle(gtk_file_chooser_dialog_new(get_title().data(), owner ? GTK_WINDOW(owner->get_handle()) : NULL, GTK_FILE_CHOOSER_ACTION_OPEN, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Open"), GTK_RESPONSE_ACCEPT, NULL));
        return filebox::show(owner);
    }

    void open_filebox::show_async(shared_ptr<window> owner, function<void(bool)> callback)
    {
        filebox::show_async(owner, callback);
    }

    bool save_filebox::show(shared_ptr<window> owner)
    {
        set_handle(gtk_file_chooser_dialog_new(get_title().data(), owner ? GTK_WINDOW(owner->get_handle()) : NULL, GTK_FILE_CHOOSER_ACTION_SAVE, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Save"), GTK_RESPONSE_ACCEPT, NULL));
        return filebox::show(owner);
    }

    void save_filebox::show_async(shared_ptr<window> owner, function<void(bool)> callback)
    {
        filebox::show_async(owner, callback);
    }
} // namespace xaml
