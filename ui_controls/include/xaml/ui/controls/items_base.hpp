#ifndef XAML_UI_CONTROLS_ITEMS_BASE_HPP
#define XAML_UI_CONTROLS_ITEMS_BASE_HPP

#include <xaml/observable_vector.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    template <>
    struct type_guid<meta_box<observable_vector<string_t>>>
    {
        static constexpr guid value{ 0x0283120a, 0xec35, 0x42c4, 0x92, 0x4f, 0xc4, 0xf7, 0x1d, 0x1f, 0xef, 0x0e };
    };

    template <>
    struct type_guid<meta_box<observable_vector_view<string_t>>>
    {
        static constexpr guid value{ 0x7465007d, 0x3b69, 0x49ec, 0xa5, 0x28, 0x8d, 0xdd, 0xa9, 0xc3, 0x87, 0xa9 };
    };

    template <typename T>
    class items_base;

    template <>
    struct type_guid<items_base<string_t>>
    {
        static constexpr guid value{ 0xfad67a33, 0x4a5f, 0x4915, 0xa1, 0x77, 0xe6, 0x1e, 0x89, 0xc7, 0x5e, 0x26 };
    };

    template <typename T>
    class items_base : public control
    {
    public:
        META_CLASS_IMPL(control)

    public:
        items_base() : control() {}
        ~items_base() override {}

    protected:
        virtual void insert_item(std::size_t index, T const& value) = 0;
        virtual void remove_item(std::size_t index) = 0;
        virtual void clear_items() = 0;
        virtual void replace_item(std::size_t index, T const& value) = 0;

    private:
        std::size_t on_items_changed_token{ 0 };
        void on_items_changed(observable_vector<T>&, vector_changed_args<T>& args)
        {
            switch (args.action)
            {
            case vector_changed_action::reset:
                clear_items();
                [[fallthrough]];
            case vector_changed_action::add:
                for (std::size_t i = 0; i < args.new_items.size(); i++)
                {
                    insert_item(i + args.new_index, args.new_items[i]);
                }
                break;
            case vector_changed_action::erase:
                for (std::size_t i = 0; i < args.old_items.size(); i++)
                {
                    remove_item(i + args.old_index);
                }
                break;
            case vector_changed_action::replace:
                replace_item(args.old_index, args.new_items[0]);
                break;
            case vector_changed_action::move:
                remove_item(args.old_index);
                insert_item(args.new_index, args.new_items[0]);
                break;
            }
            __parent_redraw();
        }

    public:
        EVENT(items_changed, std::shared_ptr<items_base>, observable_vector_view<T>)
        PROP_RD(items, observable_vector_view<T>)
        void set_items(observable_vector_view<T> value)
        {
            if (m_items != value)
            {
                if (m_items && on_items_changed_token) m_items.remove_vector_changed(on_items_changed_token);
                m_items = value;
                on_items_changed_token = m_items.add_vector_changed(mem_fn_bind(&items_base::on_items_changed, this));
                m_items_changed(std::static_pointer_cast<items_base>(shared_from_this()), m_items);
            }
        }

        EVENT(sel_id_changed, std::shared_ptr<items_base>, std::size_t)
        PROP_CONSTEXPR_EVENT(sel_id, std::size_t)

    public:
#define ADD_ITEMS_BASE_MEMBERS() \
    ADD_CONTROL_MEMBERS();       \
    ADD_PROP_EVENT(items);       \
    ADD_PROP_EVENT(sel_id)
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_ITEMS_BASE_HPP
