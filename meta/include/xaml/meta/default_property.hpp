#ifndef XAML_META_DEFAULT_PROPERTY_HPP
#define XAML_META_DEFAULT_PROPERTY_HPP

#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    class default_property : public meta_class
    {
    private:
        std::string m_property_name;

    public:
        std::string_view get_property_name() const noexcept { return m_property_name; }
        void set_property_name(std::string_view value) { m_property_name = value; }

    public:
        default_property(std::string_view prop_name = {}) : m_property_name(prop_name) {}
        ~default_property() override {}

        REGISTER_CLASS_DECL()
        {
            REGISTER_TYPE(xaml, default_property);
            ADD_CTOR_DEF();
            ADD_CTOR(std::string_view);
            ADD_PROP(property_name);
        }
    };
} // namespace xaml

#endif // !XAML_META_DEFAULT_PROPERTY_HPP
