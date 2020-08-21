#include <QComboBox>
#include <qt5/qstring.hpp>
#include <shared/combo_box.hpp>

xaml_result xaml_combo_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        auto combo = new QComboBox();
        m_handle.reset(combo);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_items());
        XAML_RETURN_IF_FAILED(draw_sel());
        XAML_RETURN_IF_FAILED(draw_editable());
    }
    return set_rect(region);
}

xaml_result xaml_combo_box_internal::draw_text() noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>(); combo && m_text)
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        combo->setCurrentText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_items() noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>(); combo && m_items)
    {
        xaml_codecvt_pool pool;
        XAML_FOREACH_START(item, m_items);
        {
            XAML_RETURN_IF_FAILED(create_item(item));
            xaml_ptr<xaml_string> s = item.query<xaml_string>();
            if (s)
            {
                QString ss;
                XAML_RETURN_IF_FAILED(to_QString(s, &ss));
                combo->addItem(ss);
            }
        }
        XAML_FOREACH_END();
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_sel() noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>())
    {
        combo->setCurrentIndex(m_sel_id);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_editable() noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>())
    {
        combo->setEditable(m_is_editable);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::insert_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>())
    {
        xaml_ptr<xaml_string> s = value.query<xaml_string>();
        if (s)
        {
            QString ss;
            XAML_RETURN_IF_FAILED(to_QString(s, &ss));
            combo->addItem(ss);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::remove_item(int32_t index) noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>())
    {
        combo->removeItem(index);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::clear_items() noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>())
    {
        combo->clear();
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::replace_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    if (auto combo = m_handle.objectCast<QComboBox>())
    {
        xaml_ptr<xaml_string> s = value.query<xaml_string>();
        if (s)
        {
            QString ss;
            XAML_RETURN_IF_FAILED(to_QString(s, &ss));
            combo->setItemText(index, ss);
        }
    }
    return XAML_S_OK;
}
