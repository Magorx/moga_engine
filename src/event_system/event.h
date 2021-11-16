#pragma once


#include <vector>
#include <functional>
#include <cassert>

#include "event_types.h"
#include "event_reaction.h"


#include <cstring>

template <typename ES_T, typename EVENT_T>
class EventDispatcher {
    ES_T *es;
    const char  *id;

    typedef std::function<EVENT_T(const EVENT_T &)> EventAffector;

    std::vector<EventReaction<EVENT_T>*> observers_before;
    std::vector<EventReaction<EVENT_T>*> observers_after;

    EventAffector event_affector;

public:

    EventDispatcher(ES_T *es, const char *id = "noname_event") : es(es), id(id), event_affector(nullptr) {
        assert(es && "can't create an EventDispatcher without parental EventSystem");
    }

    virtual ~EventDispatcher() {
        for (size_t i = 0; i < observers_before.size(); ++i) {
            delete observers_before[i];
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
            return clear_sub_result(dispatch(affected_event, sub_es_reverse));
        } else {
            return clear_sub_result(dispatch(event, sub_es_reverse));
        }
    }

    EventAccResult dispatch_to_observers(const EVENT_T &event, bool before_sub_pass = true, const EventAccResult *cur_res = nullptr) {
        EventAccResult sub_res = EventAccResult::none;
        auto &observers = before_sub_pass ? observers_before : observers_after;
    
        for (auto observer : observers) {
            EventAccResult res = (*observer)(event, cur_res);
            process_acc_result(res, sub_res);
            if ((res & EventAccResult::done) || (res & EventAccResult::stop)) { return sub_res; }
        }

        return sub_res;
    }

    EventAccResult dispatch_to_sub_es(const EVENT_T &event, bool sub_es_reverse = false);

    void process_acc_result(EventAccResult &res, EventAccResult &sub_res);
    EventAccResult clear_sub_result(EventAccResult sub_res);

    EventAccResult dispatch(const EVENT_T &event, bool sub_es_reverse) {
        EventAccResult sub_res = EventAccResult::none;

        EventAccResult res = dispatch_to_observers(event);
        process_acc_result(res, sub_res);
        if ((res & EventAccResult::done) || (res & EventAccResult::stop)) {
            return sub_res;
        }
        sub_res = clear_sub_result(sub_res);

        EventAccResult sub_sys_res = dispatch_to_sub_es(event, sub_es_reverse);
        process_acc_result(sub_sys_res, sub_res);
        if (sub_res & EventAccResult::done) {
            return sub_res;
        }
        sub_res = clear_sub_result(sub_res);

        res = dispatch_to_observers(event, false, &sub_sys_res);
        process_acc_result(res, sub_res);
        if ((res & EventAccResult::done) || (res & EventAccResult::stop)) {
            return sub_res;
        }

        return clear_sub_result(sub_res);
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

template <typename SUCC_T>
class AbstractEventSystem {
protected:
    SUCC_T *es_parent;

    std::vector<SUCC_T*> sub_es;

public:
    int index_in_parent;
    bool to_delete;

    AbstractEventSystem() :
    es_parent(nullptr),
    index_in_parent(0),
    to_delete(false)
    {}

    virtual ~AbstractEventSystem() {
        for (size_t i = 0; i < sub_es.size(); ++i) {
            sub_es[i]->set_es_parent(nullptr);
            delete sub_es[i];
        }
        delete_from_parent();
    }

    void set_to_delete(bool to_delete_) { to_delete = to_delete_; }

    void add_es(SUCC_T *sub_system) {
        if (!sub_system) return;

        sub_system->delete_from_parent();
        sub_system->set_es_parent((SUCC_T*) this); //!!!
        sub_system->index_in_parent = sub_es.size();
        sub_es.push_back(sub_system);
    }

    void delete_from_parent() {
        if (!es_parent) return;

        es_parent->delete_es(index_in_parent);
    }

    void delete_es(size_t sub_idx) {
        if (sub_idx >= sub_es.size()) {
            return;
        }

        sub_es.erase(sub_es.begin() + sub_idx);
        for (size_t i = sub_idx; i < sub_es.size(); ++i) {
            sub_es[i]->index_in_parent = i;
        }
    }

    const std::vector<SUCC_T*> &get_sub_es() {
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
        if (es_parent) es_parent->focus(index_in_parent);
    }

    bool is_focused() {
        return (!es_parent || es_parent->is_focused(index_in_parent));
    }

    bool is_focused(int idx) {
        if (idx >= (int) sub_es.size()) return false;
        else return idx == 0;
    }

    inline void set_es_parent(SUCC_T *parent_) {
        es_parent = parent_;
    }
};


class EventSystem : public AbstractEventSystem<EventSystem> {
public:
    EventDispatcher<EventSystem, Event::MousePress>      e_mouse_press;
    EventDispatcher<EventSystem, Event::MouseRelease>    e_mouse_release;
    EventDispatcher<EventSystem, Event::MouseMove>       e_mouse_move;
    EventDispatcher<EventSystem, Event::Scroll>          e_scroll;
    EventDispatcher<EventSystem, Event::MouseDrag>       e_mouse_drag;
    EventDispatcher<EventSystem, Event::Activator>       e_toggle;
    EventDispatcher<EventSystem, Event::ActivityToggle>  e_toggle_activity;
    EventDispatcher<EventSystem, Event::RenderCall>      e_render_call;
    EventDispatcher<EventSystem, Event::Close>           e_close;
    EventDispatcher<EventSystem, Event::Clicked>         e_clicked;
    EventDispatcher<EventSystem, Event::FractionChanged> e_fraction_changed;
    EventDispatcher<EventSystem, Event::VectorFractionChanged> e_vec_fraction_changed;

    EventDispatcher<EventSystem, Event::KeyDown>   e_key_down;
    EventDispatcher<EventSystem, Event::KeyUp>     e_key_up;
    EventDispatcher<EventSystem, Event::TextEnter> e_text_enter;

    EventDispatcher<EventSystem, Event::TextChanged> e_text_changed;

    EventDispatcher<EventSystem, Event::DataPtr> e_data_ptr;

    EventSystem() :
    AbstractEventSystem(),
    e_mouse_press(this, "mouse_press"),
    e_mouse_release(this, "mouse_release"),
    e_mouse_move(this, "mouse_move"),
    e_scroll(this, "scroll"),
    e_mouse_drag(this, "mouse_drag"),
    e_toggle(this, "toggle"),
    e_toggle_activity(this, "toggle_activity"),
    e_render_call(this, "render_call"),
    e_close(this, "close"),
    e_clicked(this, "clicked"),
    e_fraction_changed(this, "fraction_changed"),
    e_vec_fraction_changed(this, "vec_fraction_changed"),
    e_key_down(this, "key down"),
    e_key_up(this, "key up"),
    e_text_enter(this, "text enter"),
    e_text_changed(this, "text changed"),
    e_data_ptr(this, "data ptr")
    {}

    template <typename ES_T, typename T>
    EventDispatcher<ES_T, T> &get_dispatcher();
};

template <>
inline EventDispatcher<EventSystem, Event::MousePress> &EventSystem::get_dispatcher() {
    return e_mouse_press;
}

template <>
inline EventDispatcher<EventSystem, Event::MouseRelease> &EventSystem::get_dispatcher() {
    return e_mouse_release;
}

template <>
inline EventDispatcher<EventSystem, Event::MouseMove> &EventSystem::get_dispatcher() {
    return e_mouse_move;
}

template <>
inline EventDispatcher<EventSystem, Event::MouseDrag> &EventSystem::get_dispatcher() {
    return e_mouse_drag;
}

template <>
inline EventDispatcher<EventSystem, Event::Activator> &EventSystem::get_dispatcher() {
    return e_toggle;
}

template <>
inline EventDispatcher<EventSystem, Event::ActivityToggle> &EventSystem::get_dispatcher() {
    return e_toggle_activity;
}

template <>
inline EventDispatcher<EventSystem, Event::RenderCall> &EventSystem::get_dispatcher() {
    return e_render_call;
}

template <>
inline EventDispatcher<EventSystem, Event::Close> &EventSystem::get_dispatcher() {
    return e_close;
}

template <>
inline EventDispatcher<EventSystem, Event::Clicked> &EventSystem::get_dispatcher() {
    return e_clicked;
}

template <>
inline EventDispatcher<EventSystem, Event::FractionChanged> &EventSystem::get_dispatcher() {
    return e_fraction_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::VectorFractionChanged> &EventSystem::get_dispatcher() {
    return e_vec_fraction_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::DataPtr> &EventSystem::get_dispatcher() {
    return e_data_ptr;
}

template <>
inline EventDispatcher<EventSystem, Event::KeyDown> &EventSystem::get_dispatcher() {
    return e_key_down;
}

template <>
inline EventDispatcher<EventSystem, Event::KeyUp> &EventSystem::get_dispatcher() {
    return e_key_up;
}

template <>
inline EventDispatcher<EventSystem, Event::TextEnter> &EventSystem::get_dispatcher() {
    return e_text_enter;
}

template <>
inline EventDispatcher<EventSystem, Event::TextChanged> &EventSystem::get_dispatcher() {
    return e_text_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::Scroll> &EventSystem::get_dispatcher() {
    return e_scroll;
}

//=====================================================================================================================
template <typename ES_T, typename EVENT_T>
void EventDispatcher<ES_T, EVENT_T>::process_acc_result(EventAccResult &res, EventAccResult &sub_res) {
    if (res & EventAccResult::focus) {
        es->focus();
        sub_res = (EventAccResult) (sub_res | EventAccResult::focus);
    }
    if (res & EventAccResult::cont) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::cont);
    }
    if (res & EventAccResult::done) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::done);
    }
    if (res & EventAccResult::stop) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::stop);
    }
    if (res & EventAccResult::prevent_siblings_dispatch) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::prevent_siblings_dispatch);
    }
    if (res & EventAccResult::to_delete) {
        sub_res = (EventAccResult) (sub_res | EventAccResult::to_delete);
    }
}

template <typename ES_T, typename EVENT_T>
EventAccResult EventDispatcher<ES_T, EVENT_T>::clear_sub_result(EventAccResult sub_res) {
    sub_res = (EventAccResult) (sub_res & ~EventAccResult::stop);

    return sub_res;
}

template <typename ES_T, typename EVENT_T>
EventAccResult EventDispatcher<ES_T, EVENT_T>::dispatch_to_sub_es(const EVENT_T &event, bool sub_es_reverse) {
    EventAccResult sub_res = EventAccResult::none;

    int idx_start = 0;
    int idx_step = 1;
    int idx_stop = es->get_sub_es().size();
    if (sub_es_reverse) {
        idx_start = (int) es->get_sub_es().size() - 1;
        idx_step = -1;
        idx_stop = -1;
    }

    std::vector<ES_T*> deleted_sub_es;

    auto sub_systems = es->get_sub_es();
    for (int i = idx_start; i != idx_stop; i += idx_step) {
        auto sub_es = sub_systems[i];
        if (sub_es->to_delete) {
            deleted_sub_es.push_back(sub_es);
            continue;
        }

        EventAccResult res = sub_es-> template get_dispatcher<ES_T, EVENT_T>().emit(event, sub_es_reverse);
        process_acc_result(res, sub_res);
        sub_res = clear_sub_result(sub_res);
        if ((res & EventAccResult::done) || (res & EventAccResult::prevent_siblings_dispatch)) return sub_res;
    }

    for (auto sub_es : deleted_sub_es) {
        delete sub_es;
    }

    return sub_res;
}
