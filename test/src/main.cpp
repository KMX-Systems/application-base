// application-base
// Copyright (c) 2024 KMX Systems. All rights reserved.
#include <atomic>
#include <iostream>
#include <kmx/application/base.hpp>
#include <thread>

namespace kmx::application
{
    class test: public base<>
    {
    public:
        test();

        void start();
        void stop();

    private:
        void handle_signal(const signal_t signal) noexcept override;

        std::atomic_bool running_ {};
    };

    test::test()
    {
        base<>::register_signals(
            {signal_t::termination, signal_t::interrupt, signal_t::hangup, signal_t::user_defined_1, signal_t::user_defined_2});
    }

    void test::start()
    {
        if (daemonize().second)
        {
            for (running_ = true; running_;)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        else
            std::cerr << "Failed to daemonize" << std::endl;
    }

    void test::stop()
    {
        running_ = false;
    }

    void test::handle_signal(const signal_t signal) noexcept
    {
        switch (signal)
        {
            case signal_t::termination:
            case signal_t::interrupt:
                std::cout << "Received signal " << text_of(signal) << ", terminating daemon..." << std::endl;
                stop();
                break;
            case signal_t::hangup:
                std::cout << "Received SIGHUP signal, reloading configuration..." << std::endl;
                break;
            case signal_t::user_defined_1:
                std::cout << "Received SIGUSR1 signal, performing user-defined action 1..." << std::endl;
                break;
            case signal_t::user_defined_2:
                std::cout << "Received SIGUSR2 signal, performing user-defined action 2..." << std::endl;
                break;
            default:
                std::cout << "Unhandled signal: " << text_of(signal) << std::endl;
                break;
        }
    }

} // namespace kmx::daemon

int main()
{
    kmx::application::test app;
    app.start();
    return 0;
}
