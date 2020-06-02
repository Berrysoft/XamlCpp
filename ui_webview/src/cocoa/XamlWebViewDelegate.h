#import <WebKit/WebKit.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlWebViewDelegate : XamlDelegate<WKNavigationDelegate>
- (void)webView:(WKWebView*)webView didFinishNavigation:(WKNavigation*)navigation;
@end
