#pragma once


#include <vector>
#include <functional>


#define FRIENDLY_EVENT_ACCEPTOR(AcceptorClass) friend AcceptorClass; AcceptorClass
#define EVENT_ACCEPTOR(AcceptorClass) AcceptorClass


enum class EventAccResult {
    none,
    stop,
    done,
};


template <typename EVENT_T>
class EventReaction {
public:
    virtual EventAccResult operator()(const EVENT_T &event) = 0;
};


template <typename ACCEPTOR_T, typename EVENT_T>
class EventAcceptor : public EventReaction<EVENT_T> {
protected:
    ACCEPTOR_T *acceptor;
public:
    EventAcceptor(ACCEPTOR_T *acceptor) : acceptor(acceptor) {}
};


template <typename EVENT_T>
class EventDispatcher {
    const char *id;

    typedef std::function<EVENT_T(const EVENT_T &)> EventAffector;

    std::vector<EventReaction<EVENT_T>*> observers;
    std::vector<EventDispatcher<EVENT_T>*> chain_events;
    bool dispatch_order;

    EventAffector event_affector;

public:
    EventDispatcher(const char *id = "noname_event") : id(id), dispatch_order(true), event_affector(nullptr) {}
    EventDispatcher &operator=(const EventDispatcher &other) = delete;

    void add(EventReaction<EVENT_T> *observer) {
        observers.push_back(observer);
    }

    void add(EventDispatcher<EVENT_T> *chained_event) {
        chain_events.push_back(chained_event);
    }

    virtual EventAccResult emit(const EVENT_T &event) {
        return dispatch(event);
    }

    EventAccResult dispatch_to_observers(const EVENT_T &event) {
        if (event_affector) {
            EVENT_T affected_event = event_affector(event);
            for (auto observer : observers) {
                EventAccResult res = (*observer)(affected_event);
                if (res != EventAccResult::none) return res;
            }
        } else {
            for (auto observer : observers) {
                EventAccResult res = (*observer)(event);
                if (res != EventAccResult::none) return res;
            }
        }

        return EventAccResult::none;
    }

    EventAccResult dispatch_to_chain_events(const EVENT_T &event) {
        if (event_affector) {
            EVENT_T affected_event = event_affector(event);
            for (auto chained_event : chain_events) {
                if (chained_event->emit(affected_event) == EventAccResult::done) return EventAccResult::done;
            }
        } else {
            for (auto chained_event : chain_events) {
                if (chained_event->emit(event) == EventAccResult::done) return EventAccResult::done;
            }
        }

        return EventAccResult::none;
    }

    EventAccResult dispatch(const EVENT_T &event) {
        if (dispatch_order) {
            EventAccResult res = dispatch_to_observers(event);
            if (res != EventAccResult::none) {
                return res;
            }

            res = dispatch_to_chain_events(event);
            if (res == EventAccResult::done) {
                return res;
            }
        } else {
            EventAccResult res = dispatch_to_chain_events(event);
            if (res == EventAccResult::done) {
                return res;
            }

            res = dispatch_to_observers(event);
            if (res != EventAccResult::none) {
                return res;
            }
        }

        return EventAccResult::none;
    }

    void inverse_dispatch_order() { dispatch_order ^= 1; }

    const char *get_id() { return id; }

    void set_event_affector(EventAffector affector) { event_affector = affector; }
};
