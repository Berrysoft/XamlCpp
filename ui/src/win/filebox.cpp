#include <Windows.h>
#include <shared/filebox.hpp>
#include <wil/com.h>
#include <xaml/ui/filebox.h>
#include <xaml/ui/win/control.h>

#include <ShObjIdl.h>

using namespace std;

template <typename I>
xaml_result xaml_filebox_impl<I>::show(xaml_window* parent) noexcept
{
    auto __init = wil::CoInitializeEx(COINIT_APARTMENTTHREADED);

    wil::com_ptr_nothrow<IFileDialog> handle;
    if constexpr (std::is_same_v<I, xaml_open_filebox>)
    {
        XAML_RETURN_IF_FAILED(CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&handle)));
    }
    else
    {
        XAML_RETURN_IF_FAILED(CoCreateInstance(__uuidof(FileSaveDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&handle)));
    }

    xaml_codecvt_pool pool;

    if (m_title)
    {
        wstring_view data;
        XAML_RETURN_IF_FAILED(pool(m_title, &data));
        XAML_RETURN_IF_FAILED(handle->SetTitle(data.data()));
    }
    if (m_filename)
    {
        wstring_view data;
        XAML_RETURN_IF_FAILED(pool(m_filename, &data));
        XAML_RETURN_IF_FAILED(handle->SetFileName(data.data()));
    }
    vector<COMDLG_FILTERSPEC> types;
    if (m_filters)
    {
        XAML_FOREACH_START(xaml_filebox_filter, filter, m_filters);
        {
            wstring_view name_data;
            XAML_RETURN_IF_FAILED(pool(filter.name, &name_data));
            wstring_view pattern_data;
            XAML_RETURN_IF_FAILED(pool(filter.pattern, &pattern_data));
            types.push_back({ name_data.data(), pattern_data.data() });
        }
        XAML_FOREACH_END();
    }
    XAML_RETURN_IF_FAILED(handle->SetFileTypes((UINT)types.size(), types.data()));
    if (m_multiple)
    {
        FILEOPENDIALOGOPTIONS opt;
        XAML_RETURN_IF_FAILED(handle->GetOptions(&opt));
        opt |= FOS_ALLOWMULTISELECT;
        XAML_RETURN_IF_FAILED(handle->SetOptions(opt));
    }
    HWND owner = nullptr;
    if (parent)
    {
        xaml_ptr<xaml_win32_control> native_control;
        if (XAML_SUCCEEDED(parent->query(&native_control)))
        {
            XAML_RETURN_IF_FAILED(native_control->get_handle(&owner));
        }
    }
    XAML_RETURN_IF_FAILED(handle->Show(owner));
    m_results = nullptr;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_results));
    if (m_multiple)
    {
        wil::com_ptr_nothrow<IFileOpenDialog> open_dlg = handle.try_query<IFileOpenDialog>();
        wil::com_ptr_nothrow<IShellItemArray> items;
        XAML_RETURN_IF_FAILED(open_dlg->GetResults(&items));
        DWORD count;
        XAML_RETURN_IF_FAILED(items->GetCount(&count));
        for (DWORD i = 0; i < count; i++)
        {
            wil::com_ptr_nothrow<IShellItem> item;
            XAML_RETURN_IF_FAILED(items->GetItemAt(i, &item));
            wil::unique_cotaskmem_string name;
            XAML_RETURN_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
            xaml_ptr<xaml_string> xaml_name;
            XAML_RETURN_IF_FAILED(xaml_string_new(name.get(), &xaml_name));
            XAML_RETURN_IF_FAILED(m_results->append(xaml_name));
        }
    }
    else
    {
        wil::com_ptr_nothrow<IShellItem> item;
        XAML_RETURN_IF_FAILED(handle->GetResult(&item));
        wil::unique_cotaskmem_string name;
        XAML_RETURN_IF_FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &name));
        xaml_ptr<xaml_string> xaml_name;
        XAML_RETURN_IF_FAILED(xaml_string_new(name.get(), &xaml_name));
        XAML_RETURN_IF_FAILED(m_results->append(xaml_name));
    }
    return XAML_S_OK;
}

template struct xaml_filebox_impl<xaml_open_filebox>;
template struct xaml_filebox_impl<xaml_save_filebox>;
