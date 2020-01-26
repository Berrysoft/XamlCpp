#ifndef XAML_UI_FILEBOX_HPP
#define XAML_UI_FILEBOX_HPP

#include <functional>
#include <vector>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/objc.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_WINDOWS
#include <ShObjIdl.h>
#include <wil/com.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct filebox_filter
    {
        string_t name;
        string_t pattern;
    };

    class filebox
    {
    private:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = wil::com_ptr<IFileDialog>;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkWidget*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSSavePanel);
#endif // XAML_UI_WINDOWS

    private:
        std::vector<string_t> m_results{};

    protected:
        native_handle_type m_handle{ OBJC_NIL };

        filebox() {}

    public:
        virtual ~filebox() {}

        inline native_handle_type get_handle() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept OBJC_BLOCK({ m_handle = value; });

    public:
        PROP_STRING(title)
        PROP_STRING(filename)

    private:
        std::vector<filebox_filter> m_filters{};

    public:
        std::vector<filebox_filter> const& get_filters() const noexcept { return m_filters; }
        void set_filters(std::vector<filebox_filter> const& value) { m_filters = value; }

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
