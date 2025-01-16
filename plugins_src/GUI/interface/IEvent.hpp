/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef IEVENT_HPP
#define IEVENT_HPP

enum class EventType {
    KeyEvent,
    MouseEvent,
};

enum class KeyType {
    A,
    Z,
    Q,
    W,
    S,
    D,
    E,
    ESC,
    Unknown,
};

enum class KeyAction {
    Pressed,
    Released
};

enum class MouseAction {
    ButtonPressed,
    ButtonReleased,
    Moved,
    Scrolled
};

class IEvent {
public:
    virtual ~IEvent() = default;
    [[nodiscard]] virtual EventType getType() const = 0;
};

class KeyEvent : public IEvent {
public:
    KeyEvent(KeyType k, KeyAction a) : key(k), action(a) {}

    [[nodiscard]] EventType getType() const override {
        return EventType::KeyEvent;
    }
    [[nodiscard]] KeyType getKey() const { return key; }
    [[nodiscard]] KeyAction getAction() const { return action; }
private:
    KeyType key;
    KeyAction action;
};

class MouseEvent : public IEvent {
public:
    MouseAction action;
    int x, y;
    int button;

    MouseEvent(MouseAction a, int mx, int my, int btn = -1): action(a), x(mx), y(my), button(btn) {}

    [[nodiscard]] MouseAction getAction() const { return action; }
    [[nodiscard]] int getX() const { return x; }
    [[nodiscard]] int getY() const { return y; }
    //sf::Mouse::Button getButton() const { return button; }
    [[nodiscard]] EventType getType() const override {
        return EventType::MouseEvent;
    }
};

#endif
