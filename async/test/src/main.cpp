#include <chrono>
#include <nowide/iostream.hpp>
#include <sf/format.hpp>
#include <thread>
#include <xaml/async/action.h>

auto get_awaitable(std::chrono::system_clock::duration duration)
{
    struct awaitable
    {
        std::chrono::system_clock::duration m_duration;
        std::thread m_thread;

        explicit awaitable(std::chrono::system_clock::duration d)
            : m_duration(d),
              m_thread(
                  [d] {
                      if (d.count() > 0) std::this_thread::sleep_for(d);
                  })
        {
        }

        bool await_ready() const
        {
            return m_duration.count() <= 0;
        }

        auto await_suspend(std::coroutine_handle<> h)
        {
            m_thread.join();
            return h;
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
    auto task = get_awaitable(1s);
    println(cout, "Here is in foo.");
    co_await task;
    println(cout, "Here is also in foo.");
}

xaml_ptr<xaml_async_action> bar()
{
    println(cout, "Here is in bar.");
    auto task = foo();
    println(cout, "Here is also in bar.");
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
