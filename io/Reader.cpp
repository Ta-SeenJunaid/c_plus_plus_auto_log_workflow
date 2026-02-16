#include "Reader.hpp"
#include "../logger/Logger.hpp"
#include <thread>
#include <chrono>

void Reader::read()
{
    WORKFLOW("Reader::read");

    slog("Opening file...");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    slog("Parsing content...");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    slog("Finished reading.");
}
