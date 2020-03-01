#import <cocoa/XamlWebViewDelegate.h>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/controls/webview.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

@implementation XamlWebViewDelegate
- (void)webView:(WKWebView*)webView didFinishNavigation:(WKNavigation*)navigation;
{
    xaml::webview* ptr = (xaml::webview*)self.classPointer;
    ptr->__on_navigated();
}
@end

using namespace std;

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
            auto h = make_shared<native_control>();
            h->handle = webview;
            h->delegate = delegate;
            set_handle(h);
            draw_uri();
        }
        __set_rect(region);
    }

    void webview::draw_uri()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        NSString* nsurlstring = [NSString stringWithUTF8String:m_uri.c_str()];
        NSURL* nsurl = [NSURL URLWithString:nsurlstring];
        NSURLRequest* nsrequest = [NSURLRequest requestWithURL:nsurl];
        [webview loadRequest:nsrequest];
    }

    void webview::__on_navigated()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        atomic_guard guard(m_navigating);
        if (!guard.exchange(true))
        {
            set_uri(webview.URL.absoluteString.UTF8String);
        }
    }
}
