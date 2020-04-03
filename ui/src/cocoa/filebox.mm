#include <xaml/ui/filebox.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_filebox.hpp>
#include <xaml/ui/native_window.hpp>

using namespace std;

namespace xaml
{
    bool filebox::show(shared_ptr<window> owner)
    {
        if (m_handle)
        {
            m_handle->handle.canCreateDirectories = YES;
            m_handle->handle.showsHiddenFiles = YES;
            m_handle->handle.extensionHidden = NO;
            m_handle->handle.canSelectHiddenExtension = NO;
            m_handle->handle.treatsFilePackagesAsDirectories = YES;
            if (owner && owner->get_window()) m_handle->handle.parentWindow = owner->get_window()->window;
            m_handle->handle.title = [NSString stringWithUTF8String:m_title.c_str()];
            m_handle->handle.nameFieldStringValue = [NSString stringWithUTF8String:m_filename.c_str()];
            NSMutableArray<NSString*>* filters = [[NSMutableArray alloc] init];
            for (auto& f : m_filters)
            {
                [filters addObject:[NSString stringWithUTF8String:f.pattern.c_str()]];
            }
            m_handle->handle.allowedFileTypes = filters;
            if (m_multiple) ((NSOpenPanel*)m_handle->handle).allowsMultipleSelection = YES;
            auto res = [m_handle->handle runModal];
            if (res == NSModalResponseOK)
            {
                if (m_multiple)
                {
                    m_results.clear();
                    for (NSURL* url in [(NSOpenPanel*)m_handle->handle URLs])
                    {
                        m_results.push_back([url.path UTF8String]);
                    }
                }
                else
                {
                    m_results = { [m_handle->handle.URL.path UTF8String] };
                }
                return true;
            }
        }
        return false;
    }

    bool open_filebox::show(shared_ptr<window> owner)
    {
        NSOpenPanel* panel = [NSOpenPanel openPanel];
        panel.canChooseFiles = YES;
        panel.canChooseDirectories = NO;
        panel.resolvesAliases = NO;
        auto h = make_shared<native_filebox>();
        h->handle = panel;
        set_handle(h);
        return filebox::show(owner);
    }

    bool save_filebox::show(shared_ptr<window> owner)
    {
        NSSavePanel* panel = [NSSavePanel savePanel];
        auto h = make_shared<native_filebox>();
        h->handle = panel;
        set_handle(h);
        return filebox::show(owner);
    }
}
