#import <Cocoa/Cocoa.h>
#include <cocoa/nsstring.hpp>
#include <shared/filebox.hpp>
#include <xaml/ui/cocoa/window.h>
#include <xaml/ui/filebox.h>

using namespace std;

template <typename I>
xaml_result xaml_filebox_impl<I>::show(xaml_window* owner) noexcept
{
    using native_handle_type = conditional_t<is_same_v<I, xaml_open_filebox>, NSOpenPanel*, NSSavePanel*>;
    native_handle_type handle;
    if constexpr (is_same_v<I, xaml_open_filebox>)
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
    NSString* title;
    XAML_RETURN_IF_FAILED(get_NSString(m_title, &title));
    handle.title = title;
    NSString* filename;
    XAML_RETURN_IF_FAILED(get_NSString(m_filename, &filename));
    handle.nameFieldStringValue = filename;
    NSMutableArray<NSString*>* filters = [[NSMutableArray alloc] init];
    XAML_FOREACH_START(f, m_filters);
    {
        xaml_filebox_filter ff;
        XAML_RETURN_IF_FAILED(xaml_unbox_value<xaml_filebox_filter>(f.get(), &ff));
        NSString* pattern;
        XAML_RETURN_IF_FAILED(get_NSString(ff.pattern, &pattern));
        [filters addObject:pattern];
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
