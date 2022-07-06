#pragma once

// #define USE_GAME_LOGGING_SYSTEM 1

#ifdef USE_GAME_LOGGING_SYSTEM
    // namespace spdlog { class logger; }
    #include <memory>

    #ifdef __MINGW32__
        // MinGW needs some headers to support C++11 threads (really?)
        // These 3 are just for mingw, when compiling for linux, create another #ifdef Linux to avoid include
        #include <mingw-std-threads/mingw.mutex.h>
        #include <mingw-std-threads/mingw.thread.h>
        #include <mingw-std-threads/mingw.condition_variable.h>
    #endif

    #include <spdlog/spdlog.h>
    #include <glm/gtx/string_cast.hpp>
    // #include "helpers/vector3.hpp"

    struct Logger_t {
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()      { return st_CoreLogger;     }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()    { return st_ClientLogger;   }
    private:
        inline static std::shared_ptr<spdlog::logger> st_CoreLogger;
        inline static std::shared_ptr<spdlog::logger> st_ClientLogger;
    };

    #define LOG_INIT()              Logger_t::Init()
    // Core log macros (for core components, model loading, etc)
    #define LOG_CORE_TRACE(...)     Logger_t::GetCoreLogger()->trace(__VA_ARGS__)
    #define LOG_CORE_INFO(...)      Logger_t::GetCoreLogger()->info(__VA_ARGS__)
    #define LOG_CORE_WARN(...)      Logger_t::GetCoreLogger()->warn(__VA_ARGS__)
    #define LOG_CORE_ERROR(...)     Logger_t::GetCoreLogger()->error(__VA_ARGS__)
    #define LOG_CORE_CRITICAL(...)  Logger_t::GetCoreLogger()->critical(__VA_ARGS__)

    // Client log macros
    #define LOG_TRACE(...)          Logger_t::GetClientLogger()->trace(__VA_ARGS__)
    #define LOG_INFO(...)           Logger_t::GetClientLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)           Logger_t::GetClientLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)          Logger_t::GetClientLogger()->error(__VA_ARGS__)
    #define LOG_CRITICAL(...)       Logger_t::GetClientLogger()->critical(__VA_ARGS__)

#else
    #define LOG_INIT()
    // Core log macros (for core components, model loading, etc)
    #define LOG_CORE_TRACE(...)    
    #define LOG_CORE_INFO(...)     
    #define LOG_CORE_WARN(...)     
    #define LOG_CORE_ERROR(...)    
    #define LOG_CORE_CRITICAL(...) 
    // Client log macros
    #define LOG_TRACE(...)         
    #define LOG_INFO(...)          
    #define LOG_WARN(...)          
    #define LOG_ERROR(...)         
    #define LOG_CRITICAL(...)      
#endif
