#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <mutex>
#include <atomic>

struct WorkflowNode
{
    int id;
    std::string name;
    int parentId;
    std::vector<int> children;
    std::string threadId;
};

class Logger
{
private:
    std::map<int, WorkflowNode> nodes;
    std::mutex nodesMutex;

    std::atomic<int> nextId{0};

    // Each thread has its own stack
    static thread_local std::stack<int> nodeStack;

public:
    void enter(const std::string &name);
    void exit();
    void log(const std::string &message);
    void exportJson(const std::string &filename);
};

extern Logger logger;

// ------------------
// RAII Workflow Scope
// ------------------
class WorkflowScope
{
public:
    WorkflowScope(const std::string &name)
    {
        logger.enter(name);
    }
    ~WorkflowScope()
    {
        logger.exit();
    }
};

// Convenient macros
#define WORKFLOW(name) WorkflowScope scope_##__LINE__(name);
#define slog(msg) logger.log(msg);