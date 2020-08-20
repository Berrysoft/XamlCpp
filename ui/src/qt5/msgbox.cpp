#include <QAbstractButton>
#include <QMessageBox>
#include <map>
#include <qt5/qstring.hpp>
#include <xaml/ui/msgbox.h>
#include <xaml/ui/qt5/control.h>

using namespace std;

xaml_result XAML_CALL xaml_msgbox_custom(xaml_window* parent, xaml_string* message, xaml_string* title, xaml_string* instruction, xaml_msgbox_style style, xaml_vector_view* buttons, xaml_msgbox_result* presult) noexcept
{
    QMessageBox box{};
    if (parent)
    {
        xaml_ptr<xaml_qt5_control> native_control;
        if (XAML_SUCCEEDED(parent->query(&native_control)))
        {
            QWidget* handle;
            XAML_RETURN_IF_FAILED(native_control->get_handle(&handle));
            box.setParent(handle);
        }
    }

    QString title_str;
    XAML_RETURN_IF_FAILED(to_QString(title, &title_str));
    box.setWindowTitle(title_str);

    if (instruction)
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(instruction, &text));
        box.setText(text);

        QString infoText;
        XAML_RETURN_IF_FAILED(to_QString(message, &infoText));
        box.setInformativeText(infoText);
    }
    else
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(message, &text));
        box.setText(text);
    }

    map<QAbstractButton*, xaml_msgbox_result> result_map;
    XAML_FOREACH_START(b, buttons);
    {
        xaml_ptr<xaml_box> bbox;
        XAML_RETURN_IF_FAILED(b->query(&bbox));
        xaml_msgbox_custom_button const* button;
        XAML_RETURN_IF_FAILED(bbox->get_value_ptr(&button));
        result_map.emplace(box.addButton(button->text, QMessageBox::AcceptRole), button->result);
    }
    XAML_FOREACH_END();
    switch (style)
    {
    case xaml_msgbox_info:
        box.setIcon(QMessageBox::Information);
        break;
    case xaml_msgbox_question:
        box.setIcon(QMessageBox::Question);
        break;
    case xaml_msgbox_warning:
        box.setIcon(QMessageBox::Warning);
        break;
    case xaml_msgbox_error:
        box.setIcon(QMessageBox::Critical);
        break;
    }
    box.exec();
    *presult = result_map[box.clickedButton()];
    return XAML_S_OK;
}
