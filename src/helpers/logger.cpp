#include "logger.hpp"

#ifdef USE_GAME_LOGGING_SYSTEM
    #include <spdlog/sinks/stdout_color_sinks.h>
    #include <spdlog/sinks/basic_file_sink.h>

    // std::shared_ptr<spdlog::logger> Logger_t::st_CoreLogger;
    // std::shared_ptr<spdlog::logger> Logger_t::st_ClientLogger;

    void Logger_t::Init() {
        static bool configured = false;

        if(configured) return;

        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Raylib-game.log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        st_CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        spdlog::register_logger(st_CoreLogger);
        st_CoreLogger->set_level(spdlog::level::trace);
        st_CoreLogger->flush_on(spdlog::level::trace);

        st_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(st_ClientLogger);
        st_ClientLogger->set_level(spdlog::level::trace);
        st_ClientLogger->flush_on(spdlog::level::trace);

        configured = true;
    }
#endif

// LOGGING EXAMPLE

// spdlog::info("Welcome to spdlog!");
// spdlog::error("Some error message with arg: {}", 1);

// spdlog::warn("Easy padding in numbers like {:08d}", 12);
// spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
// spdlog::info("Support for floats {:03.2f}", 1.23456);
// spdlog::info("Positional args are {1} {0}..", "too", "supported");
// spdlog::info("{:<30}", "left aligned");

// spdlog::set_level(spdlog::level::debug); // Set global log level to debug
// spdlog::debug("This message should be displayed..");    

// // change log pattern
// spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

// // Compile time log levels
// // define SPDLOG_ACTIVE_LEVEL to desired level
// SPDLOG_TRACE("Some trace message with param {}", 42);
// SPDLOG_DEBUG("Some debug message");
