#include <wil/resource.h>
#include <wil/result.h>
#include <xaml/ui/filebox.hpp>

using namespace std;
using namespace wil;

namespace xaml
{
    //filebox::~filebox() {}

    //string_t filebox::get_result() const
    //{
    //}

    //vector<string_t> filebox::get_results() const
    //{
    //}

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
            if (m_multiple)
            {
                FILEOPENDIALOGOPTIONS opt;
                THROW_IF_FAILED(m_handle->GetOptions(&opt));
                opt |= FOS_ALLOWMULTISELECT;
                THROW_IF_FAILED(m_handle->SetOptions(opt));
            }
            auto ret = SUCCEEDED(m_handle->Show(owner ? owner->get_handle() : nullptr));
            if (ret)
            {
                if (m_multiple)
                {
                    m_results.clear();
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
                        m_results.push_back(name.get());
                    }
                }
                else
                {
                    com_ptr<IShellItem> item;
                    THROW_IF_FAILED(m_handle->GetResult(&item));
                    unique_cotaskmem_string name;
                    THROW_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
                    m_results = { name.get() };
                }
            }
            return ret;
        }
        return false;
    }

    void filebox::show_async(shared_ptr<window> owner, function<void(bool)> callback)
    {
        auto res = show(owner);
        if (callback) callback(res);
    }

    bool open_filebox::show(shared_ptr<window> owner)
    {
        set_handle(CoCreateInstance<FileOpenDialog, IFileDialog>(CLSCTX_INPROC_SERVER));
        return filebox::show(owner);
    }

    void open_filebox::show_async(shared_ptr<window> owner, function<void(bool)> callback)
    {
        filebox::show_async(owner, callback);
    }

    bool save_filebox::show(shared_ptr<window> owner)
    {
        set_handle(CoCreateInstance<FileSaveDialog, IFileDialog>(CLSCTX_INPROC_SERVER));
        return filebox::show(owner);
    }

    void save_filebox::show_async(shared_ptr<window> owner, function<void(bool)> callback)
    {
        filebox::show_async(owner, callback);
    }
} // namespace xaml
