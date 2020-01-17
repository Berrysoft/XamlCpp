#include <gtk3/resources.hpp>
#include <xaml/ui/filebox.hpp>

using namespace std;

namespace xaml
{
    void filebox::set_title(string_view_t value)
    {
        gtk_window_set_title(GTK_WINDOW(m_handle), value.data());
    }

    void filebox::set_filename(string_view_t value)
    {
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(m_handle), value.data());
    }

    void filebox::set_filters(vector<filebox_filter> const& value)
    {
        auto filter = gtk_file_filter_new();
        for (auto& f : value)
        {
            gtk_file_filter_add_pattern(filter, f.pattern.c_str());
        }
        gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(m_handle), filter);
    }

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
        if (owner) gtk_widget_set_parent(m_handle, owner->get_handle());
        auto res = gtk_dialog_run(GTK_DIALOG(m_handle));
        return res == GTK_RESPONSE_ACCEPT;
    }

    open_filebox::open_filebox() : filebox()
    {
        set_handle(gtk_file_chooser_dialog_new(U("Open File"), NULL, GTK_FILE_CHOOSER_ACTION_OPEN, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Open"), GTK_RESPONSE_ACCEPT, NULL));
    }

    open_filebox::~open_filebox() { gtk_widget_destroy(m_handle); }

    save_filebox::save_filebox() : filebox()
    {
        set_handle(gtk_file_chooser_dialog_new(U("Save File"), NULL, GTK_FILE_CHOOSER_ACTION_SAVE, U("_Cancel"), GTK_RESPONSE_CANCEL, U("_Save"), GTK_RESPONSE_ACCEPT, NULL));
    }

    save_filebox::~save_filebox() { gtk_widget_destroy(m_handle); }
} // namespace xaml
