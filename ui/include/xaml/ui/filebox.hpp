#ifndef XAML_UI_FILEBOX_HPP
#define XAML_UI_FILEBOX_HPP

#include <functional>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/objc.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    struct filebox_filter
    {
        string_t name;
        string_t pattern;
    };

    struct native_filebox;

    class filebox
    {
    private:
        std::vector<string_t> m_results{};

    public:
        using native_handle_type = std::shared_ptr<native_filebox>;

    protected:
        native_handle_type m_handle{ OBJC_NIL };

        filebox() {}

    public:
        virtual ~filebox() {}

        inline native_handle_type get_handle() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept { m_handle = value; }

    public:
        PROP_STRING(title)
        PROP_STRING(filename)

        PROP_VECTOR(filters, filebox_filter)

        string_view_t get_result() const { return m_results[0]; }

        PROP_CONSTEXPR(multiple, bool)

    public:
        std::vector<string_t> const& get_results() const { return m_results; }

    public:
        XAML_UI_API virtual bool show(std::shared_ptr<window> owner = nullptr);
    };

    class open_filebox : public filebox
    {
    public:
        open_filebox() : filebox() {}
        ~open_filebox() override {}

        XAML_UI_API bool show(std::shared_ptr<window> owner = nullptr) override;
    };

    class save_filebox : public filebox
    {
    public:
        save_filebox() : filebox() {}
        ~save_filebox() override {}

        XAML_UI_API bool show(std::shared_ptr<window> owner = nullptr) override;
    };
} // namespace xaml

#endif // !XAML_UI_FILEBOX_HPP
