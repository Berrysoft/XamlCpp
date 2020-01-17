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
    }

    string_t filebox::get_result() const
    {
        return gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(m_handle));
    }
} // namespace xaml
