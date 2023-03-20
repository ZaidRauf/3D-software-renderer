#pragma once

#include <cstdbool>
#include <chrono>
#include <thread>

class GameState{
    private:
        std::chrono::time_point<std::chrono::steady_clock> previous_frame_time = std::chrono::steady_clock::now();
        float _delta_time = 0;
    
    public:
        GameState();
        ~GameState();
        void WaitForFrame();

        bool running = true;
        bool backface_culling_enabled = true;
        int FRAMES_PER_SECOND = 120;
        int FRAME_LENGTH_MS = 1000/FRAMES_PER_SECOND;
        int frame_count = 0;
        float &delta_time = _delta_time;

};
