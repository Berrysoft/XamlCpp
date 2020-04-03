#import <cocoa/XamlWebViewDelegate.h>
#include <cocoa/strings.hpp>
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
            draw_visible();
            draw_uri();
        }
        __set_rect(region);
    }

    void webview::draw_uri()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        NSString* nsurlstring = to_native(m_uri);
        NSURL* nsurl = [NSURL URLWithString:nsurlstring];
        NSURLRequest* nsrequest = [NSURLRequest requestWithURL:nsurl];
        [webview loadRequest:nsrequest];
    }

    void webview::__on_navigated()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        atomic_guard guard(m_navigating);
        if (!guard.test_and_set())
        {
            set_uri(from_native(webview.URL.absoluteString));
        }
    }

    void webview::go_forward()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        [webview goForward];
    }

    void webview::go_back()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        [webview goBack];
    }

    bool webview::get_can_go_forward()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        return webview.canGoForward;
    }

    bool webview::get_can_go_back()
    {
        WKWebView* webview = (WKWebView*)get_handle()->handle;
        return webview.canGoBack;
    }
}
