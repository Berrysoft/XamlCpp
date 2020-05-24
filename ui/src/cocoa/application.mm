#import <cocoa/XamlApplicationDelegate.h>
#include <shared/application.hpp>
#include <xaml/ui/application.h>

@implementation XamlApplicationDelegate : XamlDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    [NSApp activateIgnoringOtherApps:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    return YES;
}
@end

using namespace std;

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    for (int i = 0; i < argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new_view(argv[i], &arg));
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(arg));
    }
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    XamlApplicationDelegate* appDelegate = [[XamlApplicationDelegate alloc] initWithClassPointer:this];
    [[NSApplication sharedApplication] setDelegate:appDelegate];
    return XAML_S_OK;
}

xaml_result xaml_application_impl::run(int* pres) noexcept
{
    [NSApp run];
    *pres = m_quit_value;
    return XAML_S_OK;
}

xaml_result xaml_application_impl::quit(int value) noexcept
{
    m_quit_value = value;
    [NSApp terminate:nil];
    return XAML_S_OK;
}

xaml_result xaml_application_impl::get_theme(xaml_application_theme* ptheme) noexcept
{
    NSString* osxMode = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
    if (osxMode && [osxMode isEqualToString:@"Dark"])
    {
        *ptheme = xaml_application_theme_dark;
    }
    else
    {
        *ptheme = xaml_application_theme_light;
    }
    return XAML_S_OK;
}
