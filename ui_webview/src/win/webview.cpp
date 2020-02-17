#include <xaml/ui/webview/webview.hpp>

#include <winrt/Windows.Web.UI.Interop.h>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::UI::Interop;

namespace xaml
{
    void webview::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        WebViewControlProcess process;
        process.CreateWebViewControlAsync((int64_t)get_handle(), { region.x, region.y, region.width, region.height })
            .Completed([this](IAsyncOperation<WebViewControl> operation, AsyncStatus) {
                WebViewControl view = operation.GetResults();
                view.NavigateToString(get_uri());
            });
    }
} // namespace xaml
