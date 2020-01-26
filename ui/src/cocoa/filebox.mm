#include <xaml/ui/filebox.hpp>

using namespace std;

namespace xaml
{
    bool filebox::show(shared_ptr<window> owner)
    {
        if (m_handle)
        {
            m_handle.canCreateDirectories = YES;
            m_handle.showsHiddenFiles = YES;
            m_handle.extensionHidden = NO;
            m_handle.canSelectHiddenExtension = NO;
            m_handle.treatsFilePackagesAsDirectories = YES;
            if (owner) m_handle.parentWindow = owner->__get_window();
            m_handle.title = [NSString stringWithUTF8String:m_title.c_str()];
            m_handle.nameFieldStringValue = [NSString stringWithUTF8String:m_filename.c_str()];
            NSMutableArray<NSString*>* filters = [[NSMutableArray alloc] init];
            for (auto& f : m_filters)
            {
                [filters addObject:[NSString stringWithUTF8String:f.pattern.c_str()]];
            }
            m_handle.allowedFileTypes = filters;
            if (m_multiple) ((NSOpenPanel*)m_handle).allowsMultipleSelection = YES;
            auto res = [m_handle runModal];
            if (res == NSModalResponseOK)
            {
                if (m_multiple)
                {
                    m_results.clear();
                    for (NSURL* url in [(NSOpenPanel*)m_handle URLs])
                    {
                        m_results.push_back([url.path UTF8String]);
                    }
                }
                else
                {
                    m_results = { [m_handle.URL.path UTF8String] };
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
        set_handle(panel);
        return filebox::show(owner);
    }

    bool save_filebox::show(shared_ptr<window> owner)
    {
        set_handle([NSSavePanel savePanel]);
        return filebox::show(owner);
    }
}
