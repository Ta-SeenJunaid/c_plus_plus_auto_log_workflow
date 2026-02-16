#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <thread>

Logger logger;

// Define thread-local stack
thread_local std::stack<int> Logger::nodeStack;

void Logger::enter(const std::string &name)
{
    int id = nextId.fetch_add(1);

    int parentId = -1;
    if (!nodeStack.empty())
    {
        parentId = nodeStack.top();
    }

    std::string threadIdStr =
        std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

    {
        std::lock_guard<std::mutex> lock(nodesMutex);

        WorkflowNode node;
        node.id = id;
        node.name = name;
        node.parentId = parentId;
        node.threadId = threadIdStr;

        nodes[id] = node;

        if (parentId != -1)
        {
            nodes[parentId].children.push_back(id);
        }
    }

    nodeStack.push(id);

    std::lock_guard<std::mutex> lock(nodesMutex);
    std::cout << "[Thread " << threadIdStr << "] ENTER: "
              << name << std::endl;
}

void Logger::exit()
{
    if (!nodeStack.empty())
    {
        nodeStack.pop();
    }
}

void Logger::log(const std::string &message)
{
    std::string threadIdStr =
        std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

    std::lock_guard<std::mutex> lock(nodesMutex);
    std::cout << "[Thread " << threadIdStr << "] "
              << message << std::endl;
}

/* void Logger::exportJson(const std::string &filename)
{
    std::ofstream file(filename);

    file << "{\n  \"workflow\": [\n";

    bool first = true;

    for (auto &[id, node] : nodes)
    {
        if (!first)
            file << ",\n";
        first = false;

        file << "    {\n";
        file << "      \"id\": " << node.id << ",\n";
        file << "      \"name\": \"" << node.name << "\",\n";
        file << "      \"parentId\": " << node.parentId << ",\n";
        file << "      \"threadId\": \"" << node.threadId << "\",\n";
        file << "      \"children\": [";

        for (size_t i = 0; i < node.children.size(); ++i)
        {
            file << node.children[i];
            if (i < node.children.size() - 1)
                file << ", ";
        }

        file << "]\n    }";
    }

    file << "\n  ]\n}";
} */

void Logger::exportJson(const std::string& filename)
{
    // ---------------------------
    // Step 1: Take snapshot under lock
    // ---------------------------
    std::map<int, WorkflowNode> snapshot;

    {
        std::lock_guard<std::mutex> lock(nodesMutex);
        snapshot = nodes;   // Fast memory copy
    }

    // ---------------------------
    // Step 2: Write file without holding lock
    // ---------------------------
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    file << "{\n  \"workflow\": [\n";

    bool first = true;

    for (const auto& [id, node] : snapshot)
    {
        if (!first)
            file << ",\n";
        first = false;

        file << "    {\n";
        file << "      \"id\": " << node.id << ",\n";
        file << "      \"name\": \"" << node.name << "\",\n";
        file << "      \"parentId\": " << node.parentId << ",\n";
        file << "      \"threadId\": \"" << node.threadId << "\",\n";
        file << "      \"children\": [";

        for (size_t i = 0; i < node.children.size(); ++i)
        {
            file << node.children[i];
            if (i < node.children.size() - 1)
                file << ", ";
        }

        file << "]\n    }";
    }

    file << "\n  ]\n}";
}
