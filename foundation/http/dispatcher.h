#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <unordered_map>  // std::unordered_multimap
#include <functional>     // std::function
#include <string>         // std::string

template <typename ParamType>
class Dispatcher 
{
private:
    using EventCallback = std::function<void(ParamType&)>;
    using Container = std::unordered_multimap<std::string, EventCallback>;

public:
    void Register(const char* cmdType, EventCallback cb)
    {
        callbacks_.insert(std::pair<const char*, EventCallback>(cmdType, cb));
    }

    bool Dispatch(const char* cmdType, ParamType& param)
    {
        auto range = callbacks_.equal_range(cmdType);
        for (auto it = range.first; it != range.second; ++it) 
        {
            auto func = it->second;
            func(param);
            return true;
        }
        return false;
    }

private:
    Container callbacks_;
};

#endif
