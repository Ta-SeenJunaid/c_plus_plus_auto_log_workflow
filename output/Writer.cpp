#include "Writer.hpp"
#include "../logger/Logger.hpp"
#include <thread>
#include <chrono>

void Writer::write()
{
    WORKFLOW("Writer::write");

    slog("Preparing output...");
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    slog("Writing to disk...");
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    slog("Finished writing.");
}