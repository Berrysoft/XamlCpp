#ifndef XAML_MARKUP_EXTENSION_HPP
#define XAML_MARKUP_EXTENSION_HPP

#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    struct markup_context
    {
        virtual std::weak_ptr<meta_class> current_element() const = 0;
        virtual std::string_view current_property() const = 0;
        virtual std::weak_ptr<meta_class> find_element(std::string_view name) const = 0;
        virtual ~markup_context() {}
    };

    class markup_extension : public meta_class
    {
    public:
        virtual ~markup_extension() {}

        virtual void provide(meta_context& ctx, markup_context& context) = 0;

#define ADD_MARKUP_EXTENSION_MEMBERS()

    public:
        REGISTER_CLASS_DECL_NOFILE(xaml, markup_extension)
        {
            ADD_MARKUP_EXTENSION_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_MARKUP_EXTENSION_HPP
