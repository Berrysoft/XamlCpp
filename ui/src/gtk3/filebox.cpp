#include <gtk3/resources.hpp>
#include <xaml/ui/filebox.hpp>

using namespace std;

namespace xaml
{
    string_t filebox::get_result() const
    {
        g_free_unique_ptr<gchar> name{ gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(m_handle)) };
        string_t res = name.get();
        return res;
    }

    vector<string_t> open_filebox::get_results() const
    {
        vector<string_t> res;
        g_slist_free_unique_ptr<GSList> list{ gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(m_handle)) };
        while (list)
        {
            g_free_unique_ptr<gchar> name{ (gchar*)list->data };
            res.push_back(name.get());
            list.reset(list->next);
        }
        return res;
    }

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
            auto res = gtk_dialog_run(GTK_DIALOG(m_handle));
            return res == GTK_RESPONSE_ACCEPT;
        }
        return false;
    }

    open_filebox::open_filebox() : filebox()
    {
    }

    open_filebox::~open_filebox() { gtk_widget_destroy(m_handle); }

    bool open_filebox::show(shared_ptr<window> owner)
    {
        set_handle(gtk_file_chooser_dialog_new(get_title().data(), owner ? GTK_WINDOW(owner->get_handle()) : NULL, GTK_FILE_CHOOSER_ACTION_OPEN, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Open"), GTK_RESPONSE_ACCEPT, NULL));
        return filebox::show(owner);
    }

    save_filebox::save_filebox() : filebox()
    {
    }

    save_filebox::~save_filebox() { gtk_widget_destroy(m_handle); }

    bool save_filebox::show(shared_ptr<window> owner)
    {
        set_handle(gtk_file_chooser_dialog_new(get_title().data(), owner ? GTK_WINDOW(owner->get_handle()) : NULL, GTK_FILE_CHOOSER_ACTION_SAVE, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Save"), GTK_RESPONSE_ACCEPT, NULL));
        return filebox::show(owner);
    }
} // namespace xaml
