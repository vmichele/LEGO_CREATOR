#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/Layer>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/ref_ptr>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/Notify>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/Light>
#include <osgText/Font>
#include <osgText/Text>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>

class Traffic : public osg::Node {
public:
    Traffic();
    osg::ref_ptr<osg::AnimationPath> getTrafficPath(void);
    osg::ref_ptr<osg::Node> getVehicules(void);
    void runTrafic(osg::ref_ptr<osg::Group> root);

public:
    osg::ref_ptr<osg::Group> _vehicules;
    osg::ref_ptr<osg::AnimationPath> _trafficPath;
};

#endif // TRAFFIC_H
