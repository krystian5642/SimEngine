#pragma once

#include <functional>
#include <memory>
#include <utility>

#define DECLARE_SIMPLE_EVENT(EventName, ...) \
    class EventName : public SimEngine::Event<__VA_ARGS__> {};

namespace SimEngine
{
    struct EventId
    {
        EventId() : id(nextId++)
        {
        }

        int GetId() const { return id; }
        bool IsValid() const { return id != -1; }
        void Invalidate() { id = -1; }

        bool operator==(const EventId& other) const { return id == other.id; }

    private:
        static inline int nextId = 0;
        int id = -1;

        friend struct std::hash<EventId>;
    };

    template<class... Args>
    class Event
    {
    private:
        using Callback = std::function<void(Args...)>;

        struct EventHandler
        {
            void* rawPtr = nullptr;
            std::weak_ptr<void> objPtr;
            Callback callback;
        };

    public:
        void Invoke(Args... args)
        {
            for (auto& [eventId, handler] : handlers)
            {
                handler.callback(args...);
            }
        }

        template<class T>
        EventId BindLambda(const std::shared_ptr<T>& owner, Callback callback)
        {
            std::weak_ptr<T> weakOwner = owner;
            return BindInternal({nullptr, weakOwner, [callback, weakOwner](Args... args)
                {
                    if (auto objPtr = weakOwner.lock())
                    {
                        callback(args...);
                    }
                }});
        }

        EventId BindLambda(Callback callback)
        {
            return BindInternal({nullptr, {}, callback});
        }

        template<class T>
        EventId BindSafe(const std::shared_ptr<T>& obj, void(T::* method)(Args...))
        {
            std::weak_ptr<T> weakOwner = obj;
            return BindInternal({nullptr, weakOwner, [weakOwner, method](Args... args)
                {
                    if (auto objPtr = weakOwner.lock())
                    {
                        (objPtr.get()->*method)(args...);
                    }
                }});
        }

        template<class T>
        EventId BindRaw(T* obj, void(T::* method)(Args...))
        {
            return BindInternal({obj, {}, [obj, method](Args... args)
                {
                    (obj->*method)(args...);
                }});
        }

        void Unbind(EventId& id)
        {
            auto it = std::remove_if(handlers.begin(), handlers.end(),
                [&id](const auto& pair) { return pair.first == id; });
            handlers.erase(it, handlers.end());
            id.Invalidate();
        }

        template<class T>
        void UnbindSafe(const std::shared_ptr<T>& obj)
        {
            auto it = std::remove_if(handlers.begin(), handlers.end(),
                [&obj](const auto& pair)
                {
                    auto locked = pair.second.objPtr.lock();
                    return locked && locked == obj;
                });
            handlers.erase(it, handlers.end());
        }

        template<class T>
        void UnbindRaw(T* obj)
        {
            auto it = std::remove_if(handlers.begin(), handlers.end(),
                [obj](const auto& pair) { return pair.second.rawPtr == obj; });
            handlers.erase(it, handlers.end());
        }

        void ResetAll()
        {
            handlers.clear();
        }

    private:
        EventId BindInternal(const EventHandler& handler)
        {
            EventId eventId;
            handlers.emplace_back(eventId, handler);
            return eventId;
        }

        std::vector<std::pair<EventId, EventHandler>> handlers;
    };
}

namespace std
{
    template<>
    struct hash<SimEngine::EventId>
    {
        std::size_t operator()(const SimEngine::EventId& e) const noexcept
        {
            return std::hash<int>{}(e.id);
        }
    };
}


