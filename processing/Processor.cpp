#include "Processor.hpp"
#include "../logger/Logger.hpp"
#include <thread>
#include <chrono>

void Processor::process()
{
    WORKFLOW("Processor::process");

    slog("Cleaning data...");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    slog("Running heavy algorithm...");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    slog("Processing done.");
}