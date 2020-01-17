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
                string_t p = f.pattern;
                if (!p.empty() && p.front() == L'*') p = p.substr(1);
                filter.Append(p);
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
                string_t p = f.pattern;
                if (!p.empty() && p.front() == L'*') p = p.substr(1);
                filter.Insert(f.name, single_threaded_vector(vector<hstring>{ (hstring)p }));
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

    fire_and_forget filebox::show_async_impl(std::shared_ptr<window> owner, std::function<void(bool)> callback)
    {
        m_handle->set_filename(m_filename);
        m_handle->set_filters(m_filters);
        bool succeed = false;
        if (m_multiple)
        {
            auto res = co_await m_handle->show_multiple_async();
            if (res)
            {
                m_results = *res;
                succeed = true;
            }
        }
        else
        {
            auto res = co_await m_handle->show_async();
            if (res)
            {
                m_results = { *res };
                succeed = true;
            }
        }
        if (callback) callback(succeed);
    }

    bool filebox::show(shared_ptr<window> owner)
    {
        show_async(owner, {});
        return false;
    }

    void filebox::show_async(shared_ptr<window> owner, std::function<void(bool)> callback)
    {
        show_async_impl(owner, callback);
    }

    bool open_filebox::show(shared_ptr<window> owner) { return filebox::show(owner); }
    void open_filebox::show_async(shared_ptr<window> owner, std::function<void(bool)> callback)
    {
        set_handle(make_shared<__file_open_picker_wrapper>());
        return filebox::show_async(owner, callback);
    }

    bool save_filebox::show(shared_ptr<window> owner) { return filebox::show(owner); }
    void save_filebox::show_async(shared_ptr<window> owner, std::function<void(bool)> callback)
    {
        set_handle(make_shared<__file_save_picker_wrapper>());
        return filebox::show_async(owner, callback);
    }
} // namespace xaml
