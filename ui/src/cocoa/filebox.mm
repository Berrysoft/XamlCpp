#include <shared/filebox.hpp>
#include <xaml/ui/filebox.h>

using namespace std;

template <typename I>
xaml_result xaml_filebox_impl<I>::show(xaml_window* owner) noexcept
{
    using native_handle_type = conditional_t<is_same_v<I, xaml_open_filebox>, NSOpenPanel*, NSSavePanel*>;
    native_handle_type handle;
    if constexpr (is_save_v<I, xaml_open_filebox>)
    {
        handle = [NSOpenPanel openPanel];
        handle.canChooseFiles = YES;
        handle.canChooseDirectories = NO;
        handle.resolvesAliases = NO;
    }
    else
    {
        handle = [NSSavePanel savePanel];
    }
    handle.canCreateDirectories = YES;
    handle.showsHiddenFiles = YES;
    handle.extensionHidden = NO;
    handle.canSelectHiddenExtension = NO;
    handle.treatsFilePackagesAsDirectories = YES;
    if (owner)
    {
        xaml_ptr<xaml_cocoa_window> native_control;
        if (XAML_SUCCEEDED(owner->query(&native_control)))
        {
            NSWindow* parent_handle;
            XAML_RETURN_IF_FAILED(native_control->get_window(&parent_handle));
            handle.parentWindow = parent_handle;
        }
    }
    if (m_title)
    {
        char const* title;
        XAML_RETURN_IF_FAILED(m_title->get_data(&title));
        handle.title = [NSString stringWithUTF8String:title];
    }
    if (m_filename)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_filename->get_data(&data));
        handle.nameFieldStringValue = [NSString stringWithUTF8String:data];
    }
    NSMutableArray<NSString*>* filters = [[NSMutableArray alloc] init];
    XAML_FOREACH_START(f, m_filters);
    {
        xaml_filebox_filter ff;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(f.get(), ff));
        [filters addObject:[NSString stringWithUTF8String:f.pattern]];
    }
    XAML_FOREACH_END();
    handle.allowedFileTypes = filters;
    if (m_multiple) ((NSOpenPanel*)handle).allowsMultipleSelection = YES;
    auto res = [handle runModal];
    if (res != NSModalResponseOK) return XAML_E_FAIL;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_results));
    if (m_multiple)
    {
        for (NSURL* url in [(NSOpenPanel*)handle URLs])
        {
            xaml_ptr<xaml_string> str;
            XAML_RETURN_IF_FAILED(xaml_string_new(url.path.UTF8String, &str));
            XAML_RETURN_IF_FAILED(m_results->append(str.get()));
        }
    }
    else
    {
        xaml_ptr<xaml_string> str;
        XAML_RETURN_IF_FAILED(xaml_string_new(handle.URL.path.UTF8String, &str));
        XAML_RETURN_IF_FAILED(m_results->append(str.get()));
    }
    return XAML_S_OK;
}

template struct xaml_filebox_impl<xaml_open_filebox>;
template struct xaml_filebox_impl<xaml_save_filebox>;
