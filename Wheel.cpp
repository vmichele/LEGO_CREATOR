#include "Wheel.h"

Wheel::Wheel(const QColor& color) :
    Lego(color) {

    calculateBoundingBox();
}

Wheel::Wheel(const Wheel& wheel) :
    Lego(wheel) {

    calculateBoundingBox();
}

void Wheel::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 4, 2, 5);
}

Wheel* Wheel::cloning(void) const {
    return new Wheel(*this);
}

QString Wheel::whoiam(void) const {
    return "Wheel";
}
