#pragma once


#include <vector>
#include <functional>
#include <cassert>


#include "event_types.h"


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


class EventSystem;


template <typename EVENT_T>
class EventDispatcher {
    EventSystem *es;
    const char  *id;

    typedef std::function<EVENT_T(const EVENT_T &)> EventAffector;

    std::vector<EventReaction<EVENT_T>*> observers;
    bool dispatch_order;

    EventAffector event_affector;

public:
    EventDispatcher(EventSystem *es, const char *id = "noname_event") : es(es), id(id), dispatch_order(true), event_affector(nullptr) {
        assert(es && "can't create an EventDispatcher without parental EventSystem");
    }
    EventDispatcher &operator=(const EventDispatcher &other) = delete;

    void add(EventReaction<EVENT_T> *observer) {
        observers.push_back(observer);
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

    EventAccResult dispatch_to_sub_es(const EVENT_T &event);

    EventAccResult dispatch(const EVENT_T &event) {
        if (dispatch_order) {
            EventAccResult res = dispatch_to_observers(event);
            if (res != EventAccResult::none) {
                return res;
            }

            res = dispatch_to_sub_es(event);
            if (res == EventAccResult::done) {
                return res;
            }
        } else {
            EventAccResult res = dispatch_to_sub_es(event);
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


//=====================================================================================================================


class EventSystem {
    std::vector<EventSystem*> sub_es;
public:
    EventDispatcher<Event::MousePress>   e_mouse_press;
    EventDispatcher<Event::MouseRelease> e_mouse_release;
    EventDispatcher<Event::MouseMove>    e_mouse_move;
    EventDispatcher<Event::Toggle>       e_toggle;

    EventSystem() :
    e_mouse_press(this, "mouse_press"),
    e_mouse_release(this, "mouse_release"),
    e_mouse_move(this, "mouse_hover"),
    e_toggle(this, "toggle")
    {}

    void add(EventSystem *sub_system) {
        if (!sub_system) return;

        sub_es.push_back(sub_system);
    }

    const std::vector<EventSystem*> &get_sub_es() {
        return sub_es;
    }

    template <typename T>
    EventDispatcher<T> &get_dispatcher();

};

template <>
inline EventDispatcher<Event::MousePress> &EventSystem::get_dispatcher() {
    return e_mouse_press;
}

template <>
inline EventDispatcher<Event::MouseRelease> &EventSystem::get_dispatcher() {
    return e_mouse_release;
}

template <>
inline EventDispatcher<Event::MouseMove> &EventSystem::get_dispatcher() {
    return e_mouse_move;
}

template <>
inline EventDispatcher<Event::Toggle> &EventSystem::get_dispatcher() {
    return e_toggle;
}


//=====================================================================================================================


template <typename EVENT_T>
EventAccResult EventDispatcher<EVENT_T>::dispatch_to_sub_es(const EVENT_T &event) {
    if (event_affector) {
        EVENT_T affected_event = event_affector(event);
        for (auto sub_es : es->get_sub_es()) {
            if (sub_es->get_dispatcher<EVENT_T>().emit(affected_event) == EventAccResult::done) return EventAccResult::done;
        }
    } else {
        for (auto sub_es : es->get_sub_es()) {
            if (sub_es->get_dispatcher<EVENT_T>().emit(event) == EventAccResult::done) return EventAccResult::done;
        }
    }

    return EventAccResult::none;
}
