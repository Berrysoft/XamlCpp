#ifndef XAML_UI_FILEBOX_HPP
#define XAML_UI_FILEBOX_HPP

#include <vector>
#include <xaml/meta/meta_macro.hpp>
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
#endif // XAML_UI_WINDOWS

    protected:
        native_handle_type m_handle{ OBJC_NIL };

        filebox() {}

    public:
        virtual ~filebox() {}

        inline native_handle_type get_handle() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept OBJC_BLOCK({ m_handle = value; });

    public:
        XAML_API void set_title(string_view_t value);

        XAML_API void set_filename(string_view_t value);

        XAML_API void set_filters(std::vector<filebox_filter> const& value);

        XAML_API string_t get_result() const;

        XAML_API bool show(std::shared_ptr<window> owner = nullptr);
    };

    class open_filebox : public filebox
    {
    public:
        XAML_API open_filebox();
        XAML_API ~open_filebox() override;

        XAML_API std::vector<string_t> get_results() const;
    };

    class save_filebox : public filebox
    {
    public:
        XAML_API save_filebox();
        XAML_API ~save_filebox() override;
    };
} // namespace xaml

#endif // !XAML_UI_FILEBOX_HPP
