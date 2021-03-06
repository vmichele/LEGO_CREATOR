#include "Corner.h"

Corner::Corner(CornerType cornerType, const QColor &color) :
    Lego(color),
    _cornerType(cornerType) {

    calculateBoundingBox();
}

Corner::Corner(const Corner& corner) :
    Lego(corner) {

    _cornerType = corner._cornerType;

    calculateBoundingBox();
}

void Corner::calculateBoundingBox(void) {
    // according to corner type, the height is different
    switch (_cornerType) {
    case brick:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 3);
        break;
    case plate:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 1);
        break;
    }
}

Corner* Corner::cloning(void) const {
    return new Corner(*this);
}

QString Corner::whoiam(void) const {
    return "Corner";
}
