#ifndef XAML_UI_FILEBOX_HPP
#define XAML_UI_FILEBOX_HPP

#include <functional>
#include <vector>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_WINDOWS
#include <ShObjIdl.h>
#include <wil/com.h>
#elif defined(XAML_UI_WINRT)
#include "winrt/base.h"
#include <memory>
#include <optional>
#include <ppltasks.h>
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

#ifdef XAML_UI_WINRT
    class __file_picker_wrapper
    {
    public:
        __file_picker_wrapper() {}
        virtual ~__file_picker_wrapper() {}

        virtual void set_filename(string_view_t value) = 0;
        virtual void set_filters(std::vector<filebox_filter> const& value) = 0;

        virtual concurrency::task<std::optional<string_t>> show_async() = 0;
        virtual concurrency::task<std::optional<std::vector<string_t>>> show_multiple_async() = 0;
    };
#endif // XAML_UI_WINRT

    class filebox
    {
    private:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = wil::com_ptr<IFileDialog>;
#elif defined(XAML_UI_WINRT)
        using native_handle_type = std::shared_ptr<__file_picker_wrapper>;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkWidget*;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_WINRT
    private:
        std::vector<string_t> m_results{};
#endif // XAML_UI_WINRT

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

        XAML_API string_t get_result() const;

        PROP_CONSTEXPR(multiple, bool)

    public:
        XAML_API std::vector<string_t> get_results() const;

#ifdef XAML_UI_WINRT
    private:
        XAML_API winrt::fire_and_forget show_async_impl(std::shared_ptr<window> owner, std::function<void(bool)> callback);
#endif // XAML_UI_WINRT

    public:
        XAML_API virtual bool show(std::shared_ptr<window> owner = nullptr);
        XAML_API virtual void show_async(std::shared_ptr<window> owner, std::function<void(bool)> callback);
        void show_async(std::function<void(bool)> callback) { show_async(nullptr, callback); }
    };

    class open_filebox : public filebox
    {
    public:
        open_filebox() : filebox() {}
        ~open_filebox() override {}

        XAML_API bool show(std::shared_ptr<window> owner = nullptr) override;
        XAML_API void show_async(std::shared_ptr<window> owner, std::function<void(bool)> callback) override;
    };

    class save_filebox : public filebox
    {
    public:
        save_filebox() : filebox() {}
        ~save_filebox() override {}

        XAML_API bool show(std::shared_ptr<window> owner = nullptr) override;
        XAML_API void show_async(std::shared_ptr<window> owner, std::function<void(bool)> callback) override;
    };
} // namespace xaml

#endif // !XAML_UI_FILEBOX_HPP
