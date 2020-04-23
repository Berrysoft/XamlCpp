#include <Windows.h>
#include <shared/filebox.hpp>
#include <wil/com.h>
#include <xaml/ui/filebox.h>

#include <ShObjIdl.h>

using namespace std;

template <typename I>
xaml_result xaml_filebox_impl<I>::show(xaml_window* parent, bool* presult) noexcept
{
    wil::com_ptr_t<IFileDialog, wil::err_returncode_policy> handle;
    if constexpr (std::is_same_v<I, xaml_open_filebox>)
    {
        XAML_RETURN_IF_FAILED(CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&handle)));
    }
    else
    {
        XAML_RETURN_IF_FAILED(CoCreateInstance(__uuidof(FileSaveDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&handle)));
    }
    if (m_title)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_title->get_data(&data));
        XAML_RETURN_IF_FAILED(handle->SetTitle(data));
    }
    if (m_filename)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_filename->get_data(&data));
        XAML_RETURN_IF_FAILED(handle->SetFileName(data));
    }
    vector<COMDLG_FILTERSPEC> types;
    for (auto f : m_filters)
    {
        types.push_back({ f.name.c_str(), f.pattern.c_str() });
    }
    XAML_RETURN_IF_FAILED(handle->SetFileTypes((UINT)types.size(), types.data()));
    if (m_multiple)
    {
        FILEOPENDIALOGOPTIONS opt;
        XAML_RETURN_IF_FAILED(handle->GetOptions(&opt));
        opt |= FOS_ALLOWMULTISELECT;
        XAML_RETURN_IF_FAILED(handle->SetOptions(opt));
    }
    auto ret = SUCCEEDED(handle->Show(owner ? owner->get_handle()->handle : nullptr));
    if (ret)
    {
        if (m_multiple)
        {
            m_results.clear();
            com_ptr<IShellItemArray> items;
            com_ptr<IFileOpenDialog> open_dlg = m_handle->handle.query<IFileOpenDialog>();
            XAML_RETURN_IF_FAILED(open_dlg->GetResults(&items));
            DWORD count;
            XAML_RETURN_IF_FAILED(items->GetCount(&count));
            for (DWORD i = 0; i < count; i++)
            {
                com_ptr<IShellItem> item;
                XAML_RETURN_IF_FAILED(items->GetItemAt(i, &item));
                unique_cotaskmem_string name;
                XAML_RETURN_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
                m_results.push_back(name.get());
            }
        }
        else
        {
            com_ptr<IShellItem> item;
            XAML_RETURN_IF_FAILED(m_handle->handle->GetResult(&item));
            unique_cotaskmem_string name;
            XAML_RETURN_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
            m_results = { name.get() };
        }
    }
    *presult = ret;
    return XAML_S_OK;
}

template struct xaml_filebox_impl<xaml_open_filebox>;
template struct xaml_filebox_impl<xaml_save_filebox>;

//using namespace wil;
//
//namespace xaml
//{
//    bool filebox::show(shared_ptr<window> owner)
//    {
//        if (m_handle)
//        {
//        }
//        return false;
//    }
//
//    bool open_filebox::show(shared_ptr<window> owner)
//    {
//        auto h = make_shared<native_filebox>();
//        h->handle = CoCreateInstance<FileOpenDialog, IFileDialog>(CLSCTX_INPROC_SERVER);
//        set_handle(h);
//        return filebox::show(owner);
//    }
//
//    bool save_filebox::show(shared_ptr<window> owner)
//    {
//        auto h = make_shared<native_filebox>();
//        h->handle = CoCreateInstance<FileSaveDialog, IFileDialog>(CLSCTX_INPROC_SERVER);
//        set_handle(h);
//        return filebox::show(owner);
//    }
//} // namespace xaml
