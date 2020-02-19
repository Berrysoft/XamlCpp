#include <xaml/ui/webview/webview.hpp>
#import <WebKit/WebKit.h>

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
            [webView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
            set_handle(webview);
            draw_uri();
        }
        __set_size_noevent({ real.width, real.height });
        draw_size();
    }

    void webview::draw_uri()
    {
        WKWebView* webview = (WKWebView*)get_handle();
        NSString* nsurlstring = [NSString stringWithUTF8String:m_uri.c_str()];
        NSURL* nsurl= [NSURL URLWithString:nsurlstring];
        NSURLRequest* nsrequest= [NSURLRequest requestWithURL:nsurl];
        [webview loadRequest:nsrequest];
    }

    void webview::draw_size()
    {
        WKWebView* webview = (WKWebView*)get_handle();
        NSRect frame = webview.frame;
        frame.size = to_native<NSSize>(get_size());
        webview.frame = frame;
    }
}
