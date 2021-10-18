#pragma once


#include <vector>
#include <functional>
#include <cassert>


#include "event_types.h"


enum EventAccResult {
    none  = 0,
    stop  = 1 << 1,
    cont  = 1 << 2,
    done  = 1 << 3,
    focus = 1 << 4,
    children_dispatch_only = 1 << 5,
};


template <typename EVENT_T>
class EventReaction {
public:
    virtual ~EventReaction() {}
    virtual EventAccResult operator()(const EVENT_T &event, const EventAccResult *cur_res = nullptr) = 0;
};


template <typename ACCEPTOR_T, typename EVENT_T>
class EventAcceptor : public EventReaction<EVENT_T> {
protected:
    ACCEPTOR_T *acceptor;
public:
    virtual ~EventAcceptor() {}
    EventAcceptor(ACCEPTOR_T *acceptor) : acceptor(acceptor) {}
};


class EventSystem;


template <typename EVENT_T>
class EventDispatcher {
    EventSystem *es;
    const char  *id;

    typedef std::function<EVENT_T(const EVENT_T &)> EventAffector;

    std::vector<EventReaction<EVENT_T>*> observers_before;
    std::vector<EventReaction<EVENT_T>*> observers_after;

    EventAffector event_affector;

public:

    EventDispatcher(EventSystem *es, const char *id = "noname_event") : es(es), id(id), event_affector(nullptr) {
        assert(es && "can't create an EventDispatcher without parental EventSystem");
    }

    ~EventDispatcher() {
        for (auto observer : observers_before) {
            delete observer;
        }
        for (auto observer : observers_after) {
            delete observer;
        }
    }

    EventDispatcher &operator=(const EventDispatcher &other) = delete;

    void add(EventReaction<EVENT_T> *observer, bool before_sub_pass = true) {
        if (before_sub_pass) observers_before.push_back(observer);
        else observers_after.push_back(observer);
    }

    virtual EventAccResult emit(const EVENT_T &event, bool sub_es_reverse = false) {
        if (event_affector) {
            EVENT_T affected_event = event_affector(event);
            return dispatch(affected_event, sub_es_reverse);
        } else {
            return dispatch(event, sub_es_reverse);
        }
    }

    EventAccResult dispatch_to_observers(const EVENT_T &event, bool before_sub_pass = true, const EventAccResult *cur_res = nullptr) {
        EventAccResult sub_res = EventAccResult::none;
        auto &observers = before_sub_pass ? observers_before : observers_after;
    
        for (auto observer : observers) {
            EventAccResult res = (*observer)(event, cur_res);
            if (res & EventAccResult::cont) sub_res = EventAccResult::cont;
            if ((res & EventAccResult::done) || (res & EventAccResult::stop)) { return res; }
        }

        return sub_res;
    }

    EventAccResult dispatch_to_sub_es(const EVENT_T &event, bool sub_es_reverse = false);

    void process_acc_result(EventAccResult &res, const EVENT_T &event);

    EventAccResult dispatch(const EVENT_T &event, bool sub_es_reverse) {
        EventAccResult sub_res = EventAccResult::none;

        EventAccResult res = dispatch_to_observers(event);
        if (res & EventAccResult::cont) sub_res = EventAccResult::cont;
        process_acc_result(res, event);
        if ((res & EventAccResult::done) || (res & EventAccResult::stop)) {
            return res;
        }

        res = dispatch_to_sub_es(event, sub_es_reverse);
        if (res & EventAccResult::cont) sub_res = EventAccResult::cont;
        process_acc_result(res, event);
        if (res & EventAccResult::done) {
            return res;
        }

        res = dispatch_to_observers(event, false, &res);
        if (res & EventAccResult::cont) sub_res = EventAccResult::cont;
        process_acc_result(res, event);
        if ((res & EventAccResult::done) || (res & EventAccResult::stop)) {
            return res;
        }

        return sub_res;
    }

    const char *get_id() { return id; }

    void set_event_affector(EventAffector affector) { event_affector = affector; }

    inline void pop_observer(bool before_sub_pass = true) {
        auto &observers = before_sub_pass ? observers_before : observers_after;

        if (!observers.size()) return;

        delete observers[observers.size() - 1];
        observers.pop_back();
    }
};


//=====================================================================================================================


class EventSystem {
    EventSystem *parent;

    std::vector<EventSystem*> sub_es;
public:
    int index_in_parent;
    EventDispatcher<Event::MousePress>      e_mouse_press;
    EventDispatcher<Event::MouseRelease>    e_mouse_release;
    EventDispatcher<Event::MouseMove>       e_mouse_move;
    EventDispatcher<Event::MouseDrag>       e_mouse_drag;
    EventDispatcher<Event::Activator>       e_toggle;
    EventDispatcher<Event::ActivityToggle>  e_toggle_activity;
    EventDispatcher<Event::RenderCall>      e_render_call;
    EventDispatcher<Event::Close>           e_close;

    EventSystem() :
    parent(nullptr),
    index_in_parent(0),
    e_mouse_press(this, "mouse_press"),
    e_mouse_release(this, "mouse_release"),
    e_mouse_move(this, "mouse_move"),
    e_mouse_drag(this, "mouse_drag"),
    e_toggle(this, "toggle"),
    e_toggle_activity(this, "toggle_activity"),
    e_render_call(this, "render_call"),
    e_close(this, "close")
    {}

    void add_es(EventSystem *sub_system) {
        if (!sub_system) return;

        sub_system->delete_from_parent();
        sub_system->set_es_parent(this);
        sub_system->index_in_parent = sub_es.size();
        sub_es.push_back(sub_system);
    }

    void delete_from_parent() {
        if (!parent) return;

        parent->delete_es(index_in_parent);
    }

    void delete_es(size_t sub_idx) {
        if (sub_idx >= sub_es.size()) {
            return;
        }

        sub_es.erase(sub_es.begin() + sub_idx);
    }

    const std::vector<EventSystem*> &get_sub_es() {
        return sub_es;
    }

    void focus(size_t sub_idx) {
        if (sub_idx >= sub_es.size()) {
            return;
        }

        auto es = sub_es[sub_idx];
        for (int i = sub_idx; i > 0; --i) {
            sub_es[i] = sub_es[i - 1];
            sub_es[i]->index_in_parent = i;
        }
        sub_es[0] = es;
        es->index_in_parent = 0;
    }

    void focus() {
        if (parent) parent->focus(index_in_parent);
    }

    template <typename T>
    EventDispatcher<T> &get_dispatcher();

    inline void set_es_parent(EventSystem *parent_) {
        parent = parent_;
    }
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
inline EventDispatcher<Event::MouseDrag> &EventSystem::get_dispatcher() {
    return e_mouse_drag;
}

template <>
inline EventDispatcher<Event::Activator> &EventSystem::get_dispatcher() {
    return e_toggle;
}

template <>
inline EventDispatcher<Event::ActivityToggle> &EventSystem::get_dispatcher() {
    return e_toggle_activity;
}

template <>
inline EventDispatcher<Event::RenderCall> &EventSystem::get_dispatcher() {
    return e_render_call;
}

template <>
inline EventDispatcher<Event::Close> &EventSystem::get_dispatcher() {
    return e_close;
}

//=====================================================================================================================
template <typename EVENT_T>
void EventDispatcher<EVENT_T>::process_acc_result(EventAccResult &res, const EVENT_T &event) {
    if (res & EventAccResult::focus) {
        es->focus();
    }
    if ((res & EventAccResult::children_dispatch_only) && (res & EventAccResult::done) == 0) {
        res = (EventAccResult) (dispatch_to_sub_es(event) | EventAccResult::done);
    }
}

#include <cstring>

template <typename EVENT_T>
EventAccResult EventDispatcher<EVENT_T>::dispatch_to_sub_es(const EVENT_T &event, bool sub_es_reverse) {
    EventAccResult sub_res = EventAccResult::none;

    int idx_start = 0;
    int idx_step = 1;
    int idx_stop = es->get_sub_es().size();
    if (sub_es_reverse) {
        idx_start = (int) es->get_sub_es().size() - 1;
        idx_step = -1;
        idx_stop = -1;
    }

    auto sub_ess = es->get_sub_es();
    for (int i = idx_start; i != idx_stop; i += idx_step) {
        auto sub_es = sub_ess[i];
        EventAccResult res = sub_es->get_dispatcher<EVENT_T>().emit(event, sub_es_reverse);

        if (res & EventAccResult::cont) sub_res = EventAccResult::cont;
        if (res & EventAccResult::done) return res;
    }

    return sub_res;
}
