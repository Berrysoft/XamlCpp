#include <QLayout>
#include <shared/control.hpp>

xaml_result xaml_control_internal::set_rect(xaml_rectangle const& region) noexcept
{
    xaml_rectangle real = region - m_margin;
    if (m_parent)
    {
        xaml_ptr<xaml_qt5_control> native_parent;
        if (XAML_SUCCEEDED(m_parent->query(&native_parent)))
        {
            QWidget* parent_handle;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_handle));
            if (!parent_handle->children().contains(m_handle.get()))
            {
                if (auto layout = qobject_cast<QLayout*>(parent_handle))
                {
                    layout->addWidget(m_handle.get());
                }
            }
        }
    }
    m_handle->setFixedSize({ (int)real.width, (int)real.height });
    XAML_RETURN_IF_FAILED(set_size_noevent({ real.width, real.height }));
    XAML_RETURN_IF_FAILED(draw_size());
    return XAML_S_OK;
}

xaml_result xaml_control_internal::size_to_fit() noexcept
{
    auto size = m_handle->sizeHint();
    return set_size_noevent(xaml_from_native(size));
}

xaml_result xaml_control_internal::draw_size() noexcept
{
    m_handle->setFixedSize(xaml_to_native<QSize>(m_size));
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_visible() noexcept
{
    m_handle->setVisible(m_is_visible);
    return XAML_S_OK;
}