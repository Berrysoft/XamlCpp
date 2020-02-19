#import <cocoa/XamlWebViewDelegate.h>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/webview/webview.hpp>

@implementation XamlWebViewDelegate
- (void)webView:(WKWebView*)webView didFinishNavigation:(WKNavigation*)navigation;
{
    xaml::webview* ptr = (xaml::webview*)self.classPointer;
    ptr->__on_navigated();
}
@end

namespace xaml
{
    void webview::__draw(const rectangle& region)
    {
        rectangle real = region - get_margin();
        if (!get_handle())
        {
            WKWebViewConfiguration* config = [WKWebViewConfiguration new];
            WKWebView* webview = [[WKWebView alloc] initWithFrame:to_native<NSRect>(real)
                                                    configuration:config];
            [webview setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
            XamlWebViewDelegate* delegate = [[XamlWebViewDelegate alloc] initWithClassPointer:this];
            webview.navigationDelegate = delegate;
            set_handle(webview);
            __set_delegate(delegate);
            draw_uri();
        }
        __set_size_noevent({ real.width, real.height });
        draw_size();
    }

    void webview::draw_uri()
    {
        WKWebView* webview = (WKWebView*)get_handle();
        NSString* nsurlstring = [NSString stringWithUTF8String:m_uri.c_str()];
        NSURL* nsurl = [NSURL URLWithString:nsurlstring];
        NSURLRequest* nsrequest = [NSURLRequest requestWithURL:nsurl];
        [webview loadRequest:nsrequest];
    }

    void webview::draw_size()
    {
        WKWebView* webview = (WKWebView*)get_handle();
        NSRect frame = webview.frame;
        frame.size = to_native<NSSize>(get_size());
        webview.frame = frame;
    }

    void webview::__on_navigated()
    {
        WKWebView* webview = (WKWebView*)get_handle();
        atomic_guard guard(m_navigating);
        if (!guard.exchange(true))
        {
            set_uri(webview.URL.absoluteString.UTF8String);
        }
	}
}
