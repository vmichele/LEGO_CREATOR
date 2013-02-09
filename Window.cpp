#include "Window.h"

Window::Window(const QColor &color, QObject* parent) :
    Lego(color, parent) {

    calculateBoundingBox();
}

Window::Window(const Window& window) :
    Lego(window) {

    calculateBoundingBox();
}

void Window::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 4, 2, 6);
}

Window* Window::cloning(void) const {
    return new Window(*this);
}

QString Window::whoiam(void) const {
    return "I am a Window";
}
