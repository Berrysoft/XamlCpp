#include <nowide/iostream.hpp>
#include <sf/format.hpp>
#include <thread>
#include <xaml/async/action.h>

auto operator co_await(std::chrono::system_clock::duration duration)
{
    struct awaitable
    {
        std::chrono::system_clock::duration duration;

        explicit awaitable(std::chrono::system_clock::duration d)
            : duration(d)
        {
        }

        bool await_ready() const
        {
            return duration.count() <= 0;
        }

        void await_suspend(std::coroutine_handle<> h)
        {
            std::thread t{
                [h, this] {
                    std::this_thread::sleep_for(duration);
                    h.resume();
                }
            };
            t.detach();
        }

        void await_resume() {}
    };
    return awaitable{ duration };
}

using namespace sf;
using nowide::cout;
using namespace std::chrono_literals;

xaml_ptr<xaml_async_action> foo()
{
    co_await 1s;
    println(cout, "Here is in foo.");
}

xaml_ptr<xaml_async_action> bar()
{
    println(cout, "Here is in bar.");
    auto task = foo();
    println(cout, "Here is also in bar.");
    std::this_thread::sleep_for(1.5s);
    co_await task;
    println(cout, "Now back to bar once again.");
    throw std::bad_alloc{};
}

int main()
{
    auto action = bar();
    try
    {
        XAML_THROW_IF_FAILED(xaml_async_action_wait(action));
    }
    catch (xaml_result_error const& e)
    {
        println(cout, xaml_result_get_message(e.get_result()));
    }
    println(cout, "Back to main.");
}
