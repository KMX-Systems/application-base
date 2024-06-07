// application-base
// Copyright (c) 2024 KMX Systems. All rights reserved.
#include "kmx/application/base.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>

namespace kmx::application
{
    static const std::unordered_map<signal_t, std::string_view> signal_names //
        {{signal_t::abort, "SIGABRT"},
         {signal_t::attempt_to_read, "SIGTTIN"},
         {signal_t::attempt_to_write, "SIGTTOU"},
         {signal_t::broken_pipe, "SIGPIPE"},
         {signal_t::child_stopped, "SIGCHLD"},
         {signal_t::continue_, "SIGCONT"},
         {signal_t::floating_point_exception, "SIGFPE"},
         {signal_t::hangup, "SIGHUP"},
         {signal_t::illegal_instruction, "SIGILL"},
         {signal_t::interrupt, "SIGINT"},
         {signal_t::kill, "SIGKILL"},
         {signal_t::quit, "SIGQUIT"},
         {signal_t::segmentation_fault, "SIGSEGV"},
         {signal_t::stop_key, "SIGTSTP"},
         {signal_t::stop_process, "SIGSTOP"},
         {signal_t::termination, "SIGTERM"},
         {signal_t::timer_alarm, "SIGALRM"},
         {signal_t::user_defined_1, "SIGUSR1"},
         {signal_t::user_defined_2, "SIGUSR2"}};

    const std::string_view& text_of(const signal_t signal_no) noexcept
    {
        const auto pos = signal_names.find(signal_no);
        if (pos != signal_names.end())
            return pos->second;

        static const std::string_view empty {};
        return empty;
    }

    std::pair<pid_t, bool> daemonize(const char* dir) noexcept
    {
        std::pair<pid_t, bool> failure {-1, false};

        pid_t pid = ::fork();
        if (pid < 0)
        {
            return failure;
        }

        if (pid > 0)
        {
            std::exit(0); // Parent exits
        }

        if (setsid() < 0)
        {
            return failure;
        }

        std::signal(SIGHUP, SIG_IGN);

        pid = ::fork();
        if (pid < 0)
        {
            return failure;
        }

        if (pid > 0)
        {
            std::exit(0); // Parent exits
        }

        ::umask(0);

        if (::chdir(dir) < 0)
        {
            return failure;
        }

        for (auto x = ::sysconf(_SC_OPEN_MAX); x >= 0; x--)
        {
            ::close(static_cast<int>(x));
        }

        const auto fd = ::open("/dev/null", O_RDWR);
        if (fd != -1)
        {
            ::dup2(fd, STDIN_FILENO);
            ::dup2(fd, STDOUT_FILENO);
            ::dup2(fd, STDERR_FILENO);
            if (fd > 2)
            {
                close(fd);
            }
        }

        return {pid, true};
    }

    void standard_signal_registrar::on(const signal_handler_t handler, const signal_t signal, const sigset_t& /*mask*/) noexcept
    {
        std::signal(std::to_underlying(signal), handler);
    }

    void advanced_signal_registrar::on(const signal_handler_t handler, const signal_t signal, const sigset_t& mask) noexcept
    {
        const struct sigaction action
        {
            handler, mask, 0, nullptr
        };

        ::sigaction(std::to_underlying(signal), &action, nullptr);
    }
}
