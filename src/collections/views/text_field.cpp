#include "text_field.h"


v_TextField::v_TextField(const ViewBody &body, TextStyle *style, bool redactable) :
v_Highlighter(body),
line(10),
style(style),
v_cursor(new v_Highlighter({0, {2, body.size.y()}})),
v_selection(new v_Highlighter({0, 0})),
redactable(redactable)
{
    set_focuseable(true);

    Appearence *appr = nullptr;
    Resources.add_appr(appr = new AppearenceColor({100, 100, 100}));
    set_appearence(appr);

    e_key_down.add(new KeyDownTextFieldAcceptor(this));
    e_key_up.add(new KeyUpTextFieldAcceptor(this));
    e_text_enter.add(new TextEnterAcceptor(this));

    display();

    v_cursor->set_appearence(Resources.add_appr(new AppearenceColor({230, 230, 230})));
    v_selection->set_appearence(Resources.add_appr(new AppearenceColor({180, 180, 230, 150})));
}

v_TextField::~v_TextField() {
    delete style;
}

void v_TextField::render(Renderer *renderer) {
    v_Highlighter::render(renderer);

    if (!is_focused()) return;

    renderer->shift(body.position + v_label->get_body().position);

    v_cursor->render(renderer);
    v_selection->render(renderer);

    renderer->shift(-(body.position + v_label->get_body().position));
}

void v_TextField::display() {
    add_label(line.c_str(), style->size, style->foreground, style->background, false);

    Vec2d pos_cursor = Renderer::get_char_position(line.c_str(), line.cursor_pos(), style->size, style->font);
    Vec2d pos_anchor = Renderer::get_char_position(line.c_str(), line.anchor_pos(), style->size, style->font);

    pos_cursor.content[1] += style->size / 5;
    pos_anchor.content[1] += style->size / 5;

    v_cursor->get_body().position = pos_cursor;
    v_cursor->get_body().size.content[1] = style->size;

    v_selection->get_body().position = pos_cursor;
    v_selection->get_body().size = pos_anchor - pos_cursor;
    v_selection->get_body().size.content[1] = style->size;
}

void v_TextField::add_char(char c) {
    if (!redactable) return;

    if (insert) {
        line.put_char(c);
    } else {
        line.insert_char(c);
    }

    line.free_anchors();
    if (!line.cursor_mag()) {
        line.cursor_r();
    }

    if (shifted) {
        line.fix_anchors();
    }
}

void v_TextField::copy_to_clipboard() {
    char *str = (char*) calloc(line.len(), sizeof(char));
    int from = line.selection_l();
    int to = line.selection_r();
    strncpy(str, line.c_str() + from, to - from);
    
    sf::Clipboard::setString(str);
    logger.info("text_field", "copied [%s] to the clipboard", str);

    free(str);
}


char to_ascii(uint32_t unicode) {
    return (char) unicode;
}

void convert(char *dst, const uint32_t *unicode, int len) {
    for (int w = 0, r = 0; r < len; ++w, ++r) {
        if (unicode[r] < 128) {
            dst[w] = unicode[r];
        } else {
            dst[w] = ' ';
        }
    }
    dst[len] = '\0';
}

void v_TextField::paste_from_clipboard() {
    int len = (int) sf::Clipboard::getString().getSize();

    char *data = (char*) calloc(len + 1, sizeof(char));
    convert(data, sf::Clipboard::getString().getData(), len);

    line.put_str(data);
    display();

    logger.info("text_field", "pasted [%s] to the clipboard", data);

    free(data);
}


TextEnterAcceptor::TextEnterAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult TextEnterAcceptor::operator()(const Event::TextEnter &event, const EventAccResult *) {
    if (!event.is_symbolic()) return EventAccResult::done;

    char c = event.ascii();

    if (c == '\r' || c == '\n') {
        // acceptor->add_char('\n');
    } else {
        acceptor->add_char(c);
    }
    acceptor->display();

    return EventAccResult::done;
}


KeyDownTextFieldAcceptor::KeyDownTextFieldAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult KeyDownTextFieldAcceptor::operator()(const Event::KeyDown &event, const EventAccResult *) {
    
    switch (event.code) {

        case Keyboard::Key::backspace :
            acceptor->line.erase_prev();
            acceptor->display();
            break;

        case Keyboard::Key::del :
            acceptor->line.erase_next();
            acceptor->display();
            break;

        case Keyboard::Key::left :
            acceptor->line.cursor_l();
            acceptor->display();
            break;

        case Keyboard::Key::right :
            acceptor->line.cursor_r();
            acceptor->display();
            break;

        case Keyboard::Key::lshift :
        case Keyboard::Key::rshift :
            acceptor->line.fix_anchors();
            acceptor->shifted++;
            break;
        
        #ifdef __APPLE__
        case Keyboard::Key::lsystem :
        case Keyboard::Key::rsystem :
        #else
        case Keyboard::Key::lcontrol :
        case Keyboard::Key::rcontrol :
        #endif

            acceptor->ctrled++;
            break;
        
        case Keyboard::Key::c :
            if (acceptor->ctrled) {
                acceptor->copy_to_clipboard();
            }
            break;
        
        case Keyboard::Key::v :
            if (acceptor->ctrled) {
                acceptor->paste_from_clipboard();
            }
            break;
        
        case Keyboard::Key::x :
            if (acceptor->ctrled) {
                acceptor->copy_to_clipboard();
                acceptor->line.cut();
                acceptor->display();
            }
            break;
        
        case Keyboard::Key::a :
            if (acceptor->ctrled) {
                acceptor->line.select_all(acceptor->shifted);
                acceptor->display();
            }
            break;
        
        case Keyboard::Key::home :
            acceptor->line.cursor_home();
            acceptor->display();
            break;

        case Keyboard::Key::end :
            acceptor->line.cursor_end();
            acceptor->display();
            break;
        
        case Keyboard::Key::enter :
            acceptor->e_text_changed.emit({acceptor->line.c_str()});
        
        default:
            break;
    }

    return EventAccResult::done;
}

KeyUpTextFieldAcceptor::KeyUpTextFieldAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult KeyUpTextFieldAcceptor::operator()(const Event::KeyUp &event, const EventAccResult *) {
    
    switch (event.code) {

        case Keyboard::Key::lshift :
        case Keyboard::Key::rshift :
            acceptor->shifted--;
            if (!acceptor->shifted) {
                acceptor->line.free_anchors();
            }
            break;

        #ifdef __APPLE__
        case Keyboard::Key::lsystem :
        case Keyboard::Key::rsystem :
        #else
        case Keyboard::Key::lcontrol :
        case Keyboard::Key::rcontrol :
        #endif
            acceptor->ctrled--;
            break;
        
        default:
            break;
    }

    return EventAccResult::done;
}
