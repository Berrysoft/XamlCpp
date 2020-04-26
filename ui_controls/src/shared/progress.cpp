#include <xaml/ui/controls/progress.hpp>

using namespace std;

namespace xaml
{
    progress::progress() : control()
    {
        add_value_changed([this](shared_ptr<progress>, int32_t) { if (get_handle()) draw_progress(); });
        add_is_indeterminate_changed([this](shared_ptr<progress>, bool) { if (get_handle()) draw_indeterminate(); });
    }

    progress::~progress() {}
} // namespace xaml
