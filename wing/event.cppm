// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.event;

import std.core;

export namespace wing {

template<typename T>
std::function<void()> MakeHandler(T* t, void (T::* pm)())
{
    return std::bind(pm, t);
}

class Event
{
public:
    Event();
    template<typename T>
    int AddHandler(T* t, void (T::* pm)())
    {
        int handlerId = nextHandlerId++;
        handlerIdMap[handlerId] = handlers.size();
        handlers.push_back(MakeHandler(t, pm));
        return handlerId;
    }
    void RemoveHandler(int handlerId);
    void Fire()
    {
        for (auto& handler : handlers)
        {
            handler();
        }
    }
    const std::vector<std::function<void()>>& Handlers() const { return handlers; }
private:
    std::vector<std::function<void()>> handlers;
    std::map<int, int> handlerIdMap;
    int nextHandlerId;
};

template<typename T, typename ArgsT>
std::function<void(ArgsT&)> MakeHandler(T* t, void (T::* pm)(ArgsT&))
{
    return std::bind(pm, t, std::placeholders::_1);
}

template <typename ArgsT>
class EventWithArgs
{
public:
    EventWithArgs() : nextHandlerId(0)
    {
    }
    template<typename T>
    int AddHandler(T* t, void (T::* pm)(ArgsT&))
    {
        int handlerId = nextHandlerId++;
        handlerIdMap[handlerId] = handlers.size();
        handlers.push_back(MakeHandler(t, pm));
        return handlerId;
    }
    void RemoveHandler(int handlerId)
    {
        auto it = handlerIdMap.find(handlerId);
        if (it != handlerIdMap.cend())
        {
            int handlerIndex = it->second;
            handlers.erase(handlers.begin() + handlerIndex);
            for (int i = 0; i < nextHandlerId; ++i)
            {
                auto hit = handlerIdMap.find(i);
                if (hit != handlerIdMap.cend())
                {
                    int hi = hit->second;
                    if (hi > handlerIndex)
                    {
                        handlerIdMap[i] = handlerIdMap[i] - 1;
                    }
                }
            }
            handlerIdMap.erase(handlerId);
        }
    }
    void Fire(ArgsT& args)
    {
        for (auto& handler : handlers)
        {
            handler(args);
        }
    }
    const std::vector<std::function<void(ArgsT&)>>& Handlers() const { return handlers; }
private:
    std::vector<std::function<void(ArgsT&)>> handlers;
    std::map<int, int> handlerIdMap;
    int nextHandlerId;
};

} // wing
