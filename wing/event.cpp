// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.event;

namespace wing {

Event::Event() : nextHandlerId(0)
{
}

void Event::RemoveHandler(int handlerId)
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

} // wing
