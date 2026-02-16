#include "logger/Logger.hpp"
#include "io/Reader.hpp"
#include "processing/Processor.hpp"
#include "output/Writer.hpp"

#include <thread>

int main()
{
    WORKFLOW("MainWorkflow");

    Reader reader;
    Processor processor;
    Writer writer;

    std::thread t1([&]()
                   {
        WORKFLOW("Thread-Reader");
        reader.read(); });

    std::thread t2([&]()
                   {
        WORKFLOW("Thread-Processor");
        processor.process(); });

    std::thread t3([&]()
                   {
        WORKFLOW("Thread-Writer");
        writer.write(); });

    t1.join();
    t2.join();
    t3.join();

    logger.exportJson("workflow.json");

    return 0;
}