#include "game/game.hpp"

// void test_threading();

int main(void) {
    Game_t game {};
    game.loop();
    // game.loop_multithread();
    // test_threading();
    return 0;
}

/*
#ifdef __MINGW32__
    #include <mingw-std-threads/mingw.thread.h>
    #include <mingw-std-threads/mingw.mutex.h>
#else
    #include <thread>
    #include <mutex>
#endif

#include <iostream>

void test_threading() {
    using namespace std::chrono_literals;
    using TimerClock_t = std::chrono::high_resolution_clock;
    // constexpr float kTargetDelta = 1.0f / 5.0f;
    constexpr std::chrono::milliseconds kMilisFrameCap = 1000ms / 60;

    auto start = TimerClock_t::now();

    for (size_t i = 0; i < 2500; i++) {
        std::cout << i << "\n";
    }
    
    auto end = TimerClock_t::now();

    // std::chrono::duration<float> duration = end - start;

    std::chrono::milliseconds milisElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << milisElapsed.count() << "\n";

    if(milisElapsed < kMilisFrameCap) {
        std::chrono::milliseconds diff = kMilisFrameCap - milisElapsed;
        std::cout << "Function took { " << diff.count() << "ms } less than frame window.\n";
        std::this_thread::sleep_for(kMilisFrameCap - milisElapsed);
    } else {
        std::chrono::milliseconds diff = milisElapsed - kMilisFrameCap;
        std::cout << "Function took { " << diff.count() << "ms } more than frame window.\n";
    }

    // std::cout << duration.count() << "\n" << milisElapsed.count() << "\n" ;
    std::cout << "Finished!\n";

    std::cin.get();

    // std::thread worker([](){
    //     std::chrono::duration_cast<float>()
    //     std::cout << ""
    // });

}

void test_threading2() {
    int count = 0;

    using namespace std::chrono_literals;

    auto lambda1 = [&count](){
        for(; count < 10; ++count) {
            std::this_thread::sleep_for(1s);
            std::cout << "Hi!\n";
        }
    };

    std::thread worker(lambda1);

    worker.join();

    auto lambda2 = [](){
        std::this_thread::sleep_for(500ms);
        std::cout << "Hi at 0.5s!\n";
    };

    std::thread worker2(lambda2);

    worker2.join();

    std::cout << "Finished count! " << count << "\n";

    std::cin.get();
}
*/