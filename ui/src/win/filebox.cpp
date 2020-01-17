#include <wil/resource.h>
#include <wil/result.h>
#include <xaml/ui/filebox.hpp>

using namespace std;
using namespace wil;

namespace xaml
{
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
        if (m_handle)
        {
            THROW_IF_FAILED(m_handle->SetTitle(m_title.data()));
            THROW_IF_FAILED(m_handle->SetFileName(m_filename.data()));
            vector<COMDLG_FILTERSPEC> types;
            for (auto& f : m_filters)
            {
                types.push_back({ f.name.c_str(), f.pattern.c_str() });
            }
            THROW_IF_FAILED(m_handle->SetFileTypes((UINT)types.size(), types.data()));
            return SUCCEEDED(m_handle->Show(owner ? owner->get_handle() : nullptr));
        }
        return false;
    }

    open_filebox::open_filebox() : filebox()
    {
    }

    open_filebox::~open_filebox() {}

    bool open_filebox::show(shared_ptr<window> owner)
    {
        set_handle(CoCreateInstance<FileOpenDialog, IFileDialog>(CLSCTX_INPROC_SERVER));
        return filebox::show(owner);
    }

    save_filebox::save_filebox() : filebox()
    {
    }

    save_filebox::~save_filebox() {}

    bool save_filebox::show(shared_ptr<window> owner)
    {
        set_handle(CoCreateInstance<FileSaveDialog, IFileDialog>(CLSCTX_INPROC_SERVER));
        return filebox::show(owner);
    }
} // namespace xaml
