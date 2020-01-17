#include <wil/resource.h>
#include <wil/result.h>
#include <xaml/ui/filebox.hpp>

using namespace std;
using namespace wil;

namespace xaml
{
    void filebox::set_title(string_view_t value)
    {
        THROW_IF_FAILED(m_handle->SetTitle(value.data()));
    }

    void filebox::set_filename(string_view_t value)
    {
        THROW_IF_FAILED(m_handle->SetFileName(value.data()));
    }

    void filebox::set_filters(vector<filebox_filter> const& value)
    {
        vector<COMDLG_FILTERSPEC> types;
        for (auto& f : value)
        {
            types.push_back({ f.name.c_str(), f.pattern.c_str() });
        }
        THROW_IF_FAILED(m_handle->SetFileTypes((UINT)types.size(), types.data()));
    }

    string_t filebox::get_result() const
    {
        com_ptr<IShellItem> item;
        THROW_IF_FAILED(m_handle->GetResult(&item));
        unique_cotaskmem_string name;
        THROW_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
        string_t result = name.get();
        return result;
    }

    vector<string_t> open_filebox::get_results() const
    {
        vector<string_t> result;
        com_ptr<IShellItemArray> items;
        com_ptr<IFileOpenDialog> open_dlg = m_handle.query<IFileOpenDialog>();
        THROW_IF_FAILED(open_dlg->GetResults(&items));
        DWORD count;
        THROW_IF_FAILED(items->GetCount(&count));
        for (DWORD i = 0; i < count; i++)
        {
            com_ptr<IShellItem> item;
            THROW_IF_FAILED(items->GetItemAt(i, &item));
            unique_cotaskmem_string name;
            THROW_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
            result.push_back(name.get());
        }
        return result;
    }

    bool filebox::show(shared_ptr<window> owner)
    {
        return SUCCEEDED(m_handle->Show(owner ? owner->get_handle() : nullptr));
    }

    open_filebox::open_filebox() : filebox()
    {
        set_handle(CoCreateInstance<FileOpenDialog, IFileDialog>(CLSCTX_INPROC_SERVER));
    }

    open_filebox::~open_filebox() {}

    save_filebox::save_filebox() : filebox()
    {
        set_handle(CoCreateInstance<FileSaveDialog, IFileDialog>(CLSCTX_INPROC_SERVER));
    }

    save_filebox::~save_filebox() {}
} // namespace xaml
