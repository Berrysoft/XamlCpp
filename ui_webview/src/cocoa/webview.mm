#import <cocoa/XamlWebViewDelegate.h>
#include <cocoa/nsstring.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/webview.hpp>
#include <xaml/ui/controls/webview.h>
#include <xaml/ui/native_drawing.hpp>

@implementation XamlWebViewDelegate
- (void)webView:(WKWebView*)webView didFinishNavigation:(WKNavigation*)navigation;
{
    xaml_webview_internal* ptr = (xaml_webview_internal*)self.classPointer;
    ptr->on_navigated();
}
@end

using namespace std;

xaml_result xaml_webview_internal::draw(const xaml_rectangle& region) noexcept
{
    if (!m_handle)
    {
        xaml_rectangle real = region - m_margin;
        WKWebViewConfiguration* config = [WKWebViewConfiguration new];
        WKWebView* webview = [[WKWebView alloc] initWithFrame:xaml_to_native<NSRect>(real)
                                                configuration:config];
        [webview setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        XamlWebViewDelegate* delegate = [[XamlWebViewDelegate alloc] initWithClassPointer:this];
        webview.navigationDelegate = delegate;
        m_handle = webview;
        m_delegate = delegate;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_uri());
    }
    return set_rect(region);
}

xaml_result xaml_webview_internal::draw_uri() noexcept
{
    WKWebView* webview = (WKWebView*)m_handle;
    NSString* nsurlstring;
    XAML_RETURN_IF_FAILED(get_NSString(m_uri, &nsurlstring));
    NSURL* nsurl = [NSURL URLWithString:nsurlstring];
    NSURLRequest* nsrequest = [NSURLRequest requestWithURL:nsurl];
    [webview loadRequest:nsrequest];
    return XAML_S_OK;
}

void xaml_webview_internal::on_navigated() noexcept
{
    WKWebView* webview = (WKWebView*)m_handle;
    xaml_atomic_guard guard(m_navigating);
    if (!guard.test_and_set())
    {
        xaml_ptr<xaml_string> uri;
        XAML_ASSERT_SUCCEEDED(xaml_string_new(webview.URL.absoluteString.UTF8String, &uri));
        XAML_ASSERT_SUCCEEDED(set_uri(uri));
    }
}

xaml_result xaml_webview_internal::go_forward() noexcept
{
    WKWebView* webview = (WKWebView*)m_handle;
    [webview goForward];
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::go_back() noexcept
{
    WKWebView* webview = (WKWebView*)m_handle;
    [webview goBack];
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_forward(bool* pvalue) noexcept
{
    WKWebView* webview = (WKWebView*)m_handle;
    *pvalue = webview.canGoForward;
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_back(bool* pvalue) noexcept
{
    WKWebView* webview = (WKWebView*)m_handle;
    *pvalue = webview.canGoBack;
    return XAML_S_OK;
}
