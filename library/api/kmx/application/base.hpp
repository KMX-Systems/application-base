// application-base
// Copyright (c) 2024 KMX Systems. All rights reserved.
#ifndef PCH
    #include <csignal>
    #include <string_view>
    #include <utility>
#endif

namespace kmx::application
{
    enum class signal_t : int
    {
        abort = SIGABRT,
        attempt_to_read = SIGTTIN,
        attempt_to_write = SIGTTOU,
        broken_pipe = SIGPIPE,
        child_stopped = SIGCHLD,
        continue_ = SIGCONT,
        floating_point_exception = SIGFPE,
        hangup = SIGHUP,
        illegal_instruction = SIGILL,
        interrupt = SIGINT,
        kill = SIGKILL,
        quit = SIGQUIT,
        segmentation_fault = SIGSEGV,
        stop_key = SIGTSTP,
        stop_process = SIGSTOP,
        termination = SIGTERM,
        timer_alarm = SIGALRM,
        user_defined_1 = SIGUSR1,
        user_defined_2 = SIGUSR2,
    };

    const std::string_view& text_of(const signal_t signal_no) noexcept;

    using signal_handler_t = void (*)(int signal_no);

    struct standard_signal_registrar
    {
        static void on(const signal_handler_t handler, const signal_t signal, const sigset_t& mask = {}) noexcept;
    };

    struct advanced_signal_registrar
    {
        static void on(const signal_handler_t handler, const signal_t signal, const sigset_t& mask = {}) noexcept;
    };

    template <typename _SignalRegistrar = standard_signal_registrar>
    class base
    {
    public:
        base() noexcept { object_ = this; }
        base(const base&) noexcept = default;
        base(base&&) noexcept = default;

        virtual ~base() = default;

        base& operator=(const base&) noexcept = default;
        base& operator=(base&&) noexcept = default;

    protected:
        virtual void handle_signal(const signal_t signal) noexcept = 0;

        static void register_signal(const signal_t signal, const sigset_t& mask = {}) noexcept
        {
            _SignalRegistrar::on(&internal_signal_handler, signal, mask);
        }

        static void register_signals(const std::initializer_list<signal_t>& signals, const sigset_t& mask = {}) noexcept
        {
            for (auto& signal: signals)
            {
                register_signal(signal, mask);
            }
        }

        static void unregister_signal(const signal_t signal) noexcept { ::signal(std::to_underlying(signal), SIG_DFL); }

        static void internal_signal_handler(const int signal) noexcept { object_->handle_signal(static_cast<signal_t>(signal)); }

        static inline base* object_ {};
    };

    std::pair<pid_t, bool> daemonize(const char* dir = "/") noexcept;
}
