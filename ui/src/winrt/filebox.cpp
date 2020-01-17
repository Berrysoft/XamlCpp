#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Storage.Pickers.h"
#include <pplawait.h>
#include <xaml/ui/filebox.hpp>

using namespace std;
using namespace winrt;
using namespace Windows::Storage::Pickers;
using namespace concurrency;

namespace xaml
{
    class __file_open_picker_wrapper : public __file_picker_wrapper
    {
    private:
        FileOpenPicker m_picker{};

    public:
        __file_open_picker_wrapper() : __file_picker_wrapper() {}
        ~__file_open_picker_wrapper() override {}

        void set_filename(string_view_t value) override {}
        void set_filters(vector<filebox_filter> const& value)
        {
            auto filter = m_picker.FileTypeFilter();
            filter.Clear();
            for (auto& f : value)
            {
                filter.Append(f.pattern);
            }
        }

        task<std::optional<string_t>> show_async() override
        {
            auto file = co_await m_picker.PickSingleFileAsync();
            if (file)
                co_return string_t{ file.Path() };
            else
                co_return nullopt;
        }

        task<std::optional<vector<string_t>>> show_multiple_async() override
        {
            vector<string_t> res;
            auto files = co_await m_picker.PickMultipleFilesAsync();
            if (files.Size() > 0)
            {
                for (auto f : files)
                {
                    res.push_back(string_t{ f.Path() });
                }
                co_return res;
            }
            else
            {
                co_return nullopt;
            }
        }
    };

    class __file_save_picker_wrapper : public __file_picker_wrapper
    {
    private:
        FileSavePicker m_picker{};

    public:
        __file_save_picker_wrapper() : __file_picker_wrapper() {}
        ~__file_save_picker_wrapper() override {}

        void set_filename(string_view_t value) override
        {
            m_picker.SuggestedFileName(value);
        }
        void set_filters(vector<filebox_filter> const& value)
        {
            auto filter = m_picker.FileTypeChoices();
            filter.Clear();
            for (auto& f : value)
            {
                filter.Insert(f.name, single_threaded_vector(vector<hstring>{ (hstring)f.pattern }));
            }
        }

        task<std::optional<string_t>> show_async() override
        {
            auto file = co_await m_picker.PickSaveFileAsync();
            if (file)
                co_return string_t{ file.Path() };
            else
                co_return nullopt;
        }

        task<std::optional<vector<string_t>>> show_multiple_async() override { co_return nullopt; }
    };

    string_t filebox::get_result() const { return m_results[0]; }

    fire_and_forget filebox::show_async_impl(function<void(bool)> callback, shared_ptr<window> owner)
    {
        m_handle->set_filename(m_filename);
        m_handle->set_filters(m_filters);
        if (m_multiple)
        {
            auto res = co_await m_handle->show_multiple_async();
            if (res)
            {
                m_results = *res;
                callback(true);
            }
            else
            {
                callback(false);
            }
        }
        else
        {
            auto res = co_await m_handle->show_async();
            if (res)
            {
                m_results = { *res };
                callback(true);
            }
            else
            {
                callback(false);
            }
        }
    }

    bool filebox::show(shared_ptr<window> owner)
    {
        show_async({}, owner);
        return false;
    }

    void filebox::show_async(function<void(bool)> callback, shared_ptr<window> owner)
    {
        show_async_impl(callback, owner);
    }
} // namespace xaml
