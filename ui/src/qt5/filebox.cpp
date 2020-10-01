#include <QFileDialog>
#include <qt5/qstring.hpp>
#include <sf/sformat.hpp>
#include <shared/filebox.hpp>
#include <xaml/ui/qt5/control.hpp>

using namespace std;

static ostream& operator<<(ostream& stream, xaml_filebox_filter const& filter)
{
    return sf::print(stream, U("{}({});;"), filter.name, filter.pattern);
}

template <typename I>
xaml_result xaml_filebox_impl<I>::show(xaml_window* parent) noexcept
{
    QWidget* owner = nullptr;
    if (parent)
    {
        xaml_ptr<xaml_qt5_control> native_control;
        if (XAML_SUCCEEDED(parent->query(&native_control)))
        {
            XAML_RETURN_IF_FAILED(native_control->get_handle(&owner));
        }
    }

    QFileDialog dialog{ owner };
    dialog.setWindowModality(Qt::WindowModal);
    if constexpr (std::is_same_v<I, xaml_open_filebox>)
    {
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(m_multiple ? QFileDialog::ExistingFiles : QFileDialog::ExistingFile);
    }
    else
    {
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
    }

    QString title;
    XAML_RETURN_IF_FAILED(to_QString(m_title, &title));
    dialog.setWindowTitle(title);

    QString filename;
    XAML_RETURN_IF_FAILED(to_QString(m_filename, &filename));
    dialog.selectFile(filename);

    ostringstream filterstream;
    if (m_filters)
    {
        XAML_FOREACH_START(xaml_filebox_filter, filter, m_filters);
        {
            filterstream << filter;
        }
        XAML_FOREACH_END();
    }
    string filter_str = filterstream.str();
    if (!filter_str.empty())
    {
        dialog.setNameFilter(filter_str.substr(0, filter_str.length() - 2).c_str());
    }

    if (dialog.exec() == QDialog::Rejected) return XAML_E_FAIL;

    m_results = nullptr;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_results));
    for (QString& file : dialog.selectedFiles())
    {
        xaml_ptr<xaml_string> file_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(file, &file_str));
        XAML_RETURN_IF_FAILED(m_results->append(file_str));
    }
    return XAML_S_OK;
}

template struct xaml_filebox_impl<xaml_open_filebox>;
template struct xaml_filebox_impl<xaml_save_filebox>;
