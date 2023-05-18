#include "gamestate.h"

GameState::GameState() {}

GameState::~GameState() {}

void GameState::WaitForFrame(){
    auto frame_start_time = std::chrono::steady_clock::now();
    auto frame_elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_start_time - previous_frame_time);
    
    int wait_time_ms = FRAME_LENGTH_MS - frame_elapsed_time.count();
    if(wait_time_ms > 0 && wait_time_ms <= FRAME_LENGTH_MS){
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time_ms));
    }

    auto delta_elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - previous_frame_time);
    _delta_time = static_cast<float>(delta_elapsed_time.count()) / 1000.0;
    previous_frame_time = std::chrono::steady_clock::now();
    
    frame_count++;
}
