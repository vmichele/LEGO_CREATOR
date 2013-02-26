#ifndef LEGOGEODE_H
#define LEGOGEODE_H

#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osg/Geode>

#include "Lego.h"

class LegoGeode : public osg::Group {

public:
    LegoGeode(osg::ref_ptr<Lego> lego = NULL);
    LegoGeode(const LegoGeode& legoGeode);
    virtual ~LegoGeode();

    virtual void createGeode(void) {}
    virtual Lego* getLego(void) { return _lego; }
    virtual void setLego(Lego* lego) { _lego = lego; }

    osg::ref_ptr<osg::Drawable> createPlot(double radiusX, double radiusY, int height) const;
    osg::ref_ptr<osg::Drawable> createCylinder(double radiusX, double radiusY, double height, bool thin, double center = 0) const;

    //virtual LegoGeode* cloning(void) const = 0;
    virtual LegoGeode* cloning(void) const { return new LegoGeode(*this); }

protected:
    Lego* _lego;
};

#endif // LEGONODE_H
