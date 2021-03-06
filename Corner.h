#ifndef CORNER_H
#define CORNER_H

#include "Lego.h"

class Corner : public Lego {

public:
    enum CornerType { brick, plate };

    Corner(CornerType cornerType = brick, const QColor& color = QColor(Qt::red));
    Corner(const Corner& corner);

    CornerType getCornerType(void) const { return _cornerType; }

    void setCornerType(CornerType cornerType) { _cornerType = cornerType; }
    void setCornerType(int index) { index == 0 ? _cornerType = brick : _cornerType = plate; }

    int calculateHeight(void) const { return _cornerType == brick ? 3 : 1; }

    virtual void calculateBoundingBox(void);

    virtual Corner* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    CornerType _cornerType;
};

#endif // CORNER_H
