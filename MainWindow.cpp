#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MainWindow.h"
#include "GenerateRoadWindow.h"

#include "LegoFactory.h"
#include "BrickDialog.h"
#include "CornerDialog.h"
#include "RoadDialog.h"
#include "SlopDialog.h"
#include "CharacterDialog.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _legoColor(Qt::red),
    _world(),
    _roadPath() {

    // Register in factories
    initFactories();

    // Init preview element
    initPreview();
    initDialogs();

    // Create menus
    createFileMenu();
    createGenerateMenu();
    createHelpMenu();

    // Create right dock
    createParamsDock();
    createMoveDock();

    // Create scene
    createScene();

    // Create tabs
    _tabs = new QTabWidget(this);
    _tabs->addTab(_sceneFrame, "Construction");
    _tabs->addTab(new QWidget(this), "Interaction");

    // Set tabs mode
    setCentralWidget(_tabs);

    // Connections
    connect(_shapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDialog(int)));
    for (int k = 0; k < _shapeComboBox->count(); k++)
        connect(_legoDialog.at(k), SIGNAL(changedLego(LegoGeode*)), this, SLOT(legoUpdated(LegoGeode*)));
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browseColor()));
    connect(_createButton, SIGNAL(clicked()), this, SLOT(createLego()));
    connect(_fitButton, SIGNAL(clicked()), this, SLOT(fitLego()));
    connect(_xTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    connect(_yTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    connect(_zTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    connect(_leftRotateButton, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    connect(_rightRotateButton, SIGNAL(clicked()), this, SLOT(rotateRight()));
    connect(_generateRoadAction, SIGNAL(triggered()), this, SLOT(generateRoad()));

    // Change soft title
    setWindowTitle("LEGO Creator");

    // Maximize window
    //setWindowState(Qt::WindowMaximized);

    // Apply style sheet
    setStyle();
}

MainWindow::~MainWindow() {
    delete _currLego;
}

void MainWindow::initFactories(void) {
    // Register Brick
    LegoFactory<Brick, QString>::registerLego(QString("Brick"), new Brick);
    // Register BrickGeode
    LegoFactory<BrickGeode, QString>::registerLego(QString("BrickGeode"), new BrickGeode);
    // Register BrickDialog
    LegoFactory<BrickDialog, QString>::registerLego(QString("BrickDialog"), new BrickDialog);

    // Register Corner
    LegoFactory<Corner, QString>::registerLego(QString("Corner"), new Corner);
    // Register CornerGeode
    LegoFactory<CornerGeode, QString>::registerLego(QString("CornerGeode"), new CornerGeode);
    // Register CornerDialog
    LegoFactory<CornerDialog, QString>::registerLego(QString("CornerDialog"), new CornerDialog);

    // Register Road
    LegoFactory<Road, QString>::registerLego(QString("Road"), new Road);
    // Register RoadGeode
    LegoFactory<RoadGeode, QString>::registerLego(QString("RoadGeode"), new RoadGeode);
    // Register RoadDialog
    LegoFactory<RoadDialog, QString>::registerLego(QString("RoadDialog"), new RoadDialog);

    // Register Slop
    LegoFactory<Slop, QString>::registerLego(QString("Slop"), new Slop);
    // Register SlopGeode
    LegoFactory<SlopGeode, QString>::registerLego(QString("SlopGeode"), new SlopGeode);
    // Register SlopDialog
    LegoFactory<SlopDialog, QString>::registerLego(QString("SlopDialog"), new SlopDialog);

    // Register Character
    LegoFactory<Character, QString>::registerLego(QString("Character"), new Character);
    // Register CharacterGeode
    LegoFactory<CharacterGeode, QString>::registerLego(QString("CharacterGeode"), new CharacterGeode);
    // Register CharacterDialog
    LegoFactory<CharacterDialog, QString>::registerLego(QString("CharacterDialog"), new CharacterDialog);

    // ENREGISTRER ICI LES AUTRES CLASSES DE PIECE LEGO QUE L'ON CREERA
}

void MainWindow::initPreview(void) {
    _currLego = LegoFactory<Brick, QString>::create("Brick");
    static_cast<Brick*>(_currLego)->setColor(QColor(Qt::red));
    static_cast<Brick*>(_currLego)->setWidth(2);
    static_cast<Brick*>(_currLego)->setLength(4);
    _currLegoGeode = LegoFactory<BrickGeode, QString>::create("BrickGeode");
    _currLegoGeode->setLego(_currLego);
    _currLegoGeode->createGeode();
    _scene = new osg::Group;
    _scene->addChild(_currLegoGeode);
}

void MainWindow::initDialogs(void) {
    // BrickDialog
    if (BrickDialog* brickDialog = dynamic_cast<BrickDialog*>(LegoFactory<BrickDialog, QString>::create("BrickDialog"))) {
        brickDialog->initLego(_currLego);
        brickDialog->initLegoGeode(_currLegoGeode);
        _legoDialog << brickDialog;
    } else
        qDebug() << "Cannot create BrickDialog in MainWindow::initDialogs";

    // CornerDialog
    if (CornerDialog* cornerDialog = dynamic_cast<CornerDialog*>(LegoFactory<CornerDialog, QString>::create("CornerDialog")))
        _legoDialog << cornerDialog;
    else
        qDebug() << "Cannot create CornerDialog in MainWindow::initDialogs";

    // SlopDialog
    if (SlopDialog* slopDialog = dynamic_cast<SlopDialog*>(LegoFactory<SlopDialog, QString>::create("SlopDialog")))
        _legoDialog << slopDialog;
    else
        qDebug() << "Cannot create SlopDialog in MainWindow::initDialogs";

    // RoadDialog
    if (RoadDialog* roadDialog = dynamic_cast<RoadDialog*>(LegoFactory<RoadDialog, QString>::create("RoadDialog")))
        _legoDialog << roadDialog;
    else
        qDebug() << "Cannot create RoadDialog in MainWindow::initDialogs";

    // CharacterDialog
    if (CharacterDialog* characterDialog = dynamic_cast<CharacterDialog*>(LegoFactory<CharacterDialog, QString>::create("CharacterDialog")))
        _legoDialog << characterDialog;
    else
        qDebug() << "Cannot create CharacterDialog in MainWindow::initDialogs";

    for (int k = 1; k < _legoDialog.size(); k++) {
        _legoDialog.at(k)->setVisible(false);
    }
}



// ////////////////////////////////////
// Create Right Dock
// ////////////////////////////////////
void MainWindow::createParamsDock(void) {
    // ComboBox choose your brick
    _shapeComboBox = new QComboBox(this);
    QStringList brickForms;
    brickForms << "Brick" << "Corner" << "Slop" << "Road" << "Character";
    _shapeComboBox->addItems(brickForms);
    QFormLayout* shapeLayout = new QFormLayout;
    shapeLayout->addRow("LEGO shape:", _shapeComboBox);

    // Brick Preview
    QFrame* previewFrame = new QFrame(this);
    previewFrame->setFixedSize(250, 250);
    // Create osg viewer widget that displays bricks
    _brickViewer = new ViewerWidget;
    _brickViewer->initView();
    _brickViewer->changeCamera(_brickViewer->createCamera(osg::Vec4(.1, .1, .1, 1.), 0, 0, 100, 100));
    _brickViewer->changeScene(_scene.get());
    _brickViewer->initWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_brickViewer);
    previewFrame->setLayout(previewLayout);

    // Color Button
    _colorButton = new QPushButton("Color", this);
    _colorButton->setFixedWidth(100);

    // CreateButton
    _createButton = new QPushButton("Create", this);
    _createButton->setFixedWidth(100);


    // Buttons Layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_colorButton);
    buttonsLayout->addWidget(_createButton);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Preview", this));
    mainLayout->addWidget(previewFrame);
    mainLayout->addLayout(shapeLayout);
    for (int k = 0; k < _legoDialog.size(); k++)
        mainLayout->addWidget(_legoDialog.at(k));
    mainLayout->addLayout(buttonsLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    // Right Dock's Widget
    _paramsWidget = new QWidget(this);
    _paramsWidget->setLayout(mainLayout);

    // Right Dock
    _paramsDock = new QDockWidget("Create your brick", this);
    _paramsDock->setFixedSize(250, 500);
    addDockWidget(Qt::RightDockWidgetArea, _paramsDock);
    _paramsDock->setWidget(_paramsWidget);
    _paramsDock->setAllowedAreas(Qt::RightDockWidgetArea);
}

void MainWindow::createMoveDock(void) {
    // Translation according to x
    _xTransSpinBox = new QSpinBox;
    _xTransSpinBox->setMinimum(World::minLength);
    _xTransSpinBox->setMaximum(World::maxLength);
    _xTransSpinBox->setValue(0);

    // Translation according to y
    _yTransSpinBox = new QSpinBox;
    _yTransSpinBox->setMinimum(World::minWidth);
    _yTransSpinBox->setMaximum(World::maxWidth);
    _yTransSpinBox->setValue(0);

    // Translation according to z
    _zTransSpinBox = new QSpinBox;
    _zTransSpinBox->setMinimum(World::minHeight);
    _zTransSpinBox->setMaximum(World::maxHeight);
    _zTransSpinBox->setValue(0);

    QFormLayout* transLayout = new QFormLayout;
    transLayout->addRow("According to x:", _xTransSpinBox);
    transLayout->addRow("According to y:", _yTransSpinBox);
    transLayout->addRow("According to z:", _zTransSpinBox);

    QGroupBox* transGroupBox = new QGroupBox("Translation", this);
    transGroupBox->setLayout(transLayout);

    // Left Rotation Button
    _leftRotateButton = new QPushButton("Left", this);
    _leftRotateButton->setFixedWidth(100);

    // Right Rotation Button
    _rightRotateButton = new QPushButton("Right", this);
    _rightRotateButton->setFixedWidth(100);

    QHBoxLayout* rotationLayout = new QHBoxLayout;
    rotationLayout->addWidget(_leftRotateButton);
    rotationLayout->addWidget(_rightRotateButton);

    QGroupBox* rotationGroupBox = new QGroupBox("Rotation", this);
    rotationGroupBox->setLayout(rotationLayout);

    // Fit Button
    _fitButton = new QPushButton("Fit", this);
    _fitButton->setFixedWidth(100);

    // Delete Button
    _deleteButton = new QPushButton("Delete", this);
    _deleteButton->setFixedWidth(100);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_fitButton);
    buttonsLayout->addWidget(_deleteButton);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(transGroupBox);
    mainLayout->addWidget(rotationGroupBox);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    // Right Dock's Widget
    _moveWidget = new QWidget(this);
    _moveWidget->setLayout(mainLayout);

    // Right Dock
    _moveDock = new QDockWidget("Move your brick", this);
    //_moveDock->setFixedSize(250, 500);
    addDockWidget(Qt::RightDockWidgetArea, _moveDock);
    _moveDock->setWidget(_moveWidget);
    _moveDock->setAllowedAreas(Qt::RightDockWidgetArea);
    _moveDock->setEnabled(false);
}

void MainWindow::createScene(void) {
    _sceneFrame = new QFrame(this);
    _sceneFrame->setFixedSize(1150, 750);
    _sceneViewer = new ViewerWidget;
    _sceneViewer->initView();
    _sceneViewer->changeCamera(_brickViewer->createCamera(osg::Vec4(77.0/255.0, 188.0/255.0, 233.0/255.0, 1.), 0, 0, 100, 100));
    _sceneViewer->changeScene(_world.getScene().get());
    _sceneViewer->initWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_sceneViewer);
    _sceneFrame->setLayout(previewLayout);
}



// Open the color dialog to change our LEGO color
void MainWindow::browseColor() {
    _legoColor = QColorDialog::getColor(Qt::red, this);
    _currLego->setColor(_legoColor);
    _currLegoGeode->createGeode();
}

void MainWindow::chooseDialog(int dialogIndex) {
    for (int k = 0; k < _legoDialog.size(); k++) {
        if (k == dialogIndex)
            _legoDialog.at(k)->setVisible(true);
        else
            _legoDialog.at(k)->setVisible(false);
    }

    delete _currLego;
    switch (dialogIndex) {
    case 0:
        if ((_currLego = dynamic_cast<Brick*>(LegoFactory<Brick, QString>::create("Brick")))) {
            BrickDialog* dialog = static_cast<BrickDialog*>(_legoDialog.at(dialogIndex));
            Brick* lego = static_cast<Brick*>(_currLego);
            lego->setColor(_legoColor);
            lego->setWidth(dialog->getWidth());
            lego->setLength(dialog->getLength());
        } else {
            qDebug() << "Cannot cast in Brick* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<BrickGeode*>(LegoFactory<BrickGeode, QString>::create("BrickGeode"))))
            qDebug() << "Cannot cast in BrickGeode* within MainWindow::chooseDialog";
        break;
    case 1:
        if ((_currLego = dynamic_cast<Corner*>(LegoFactory<Corner, QString>::create("Corner")))) {
            Corner* lego = static_cast<Corner*>(_currLego);
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Corner* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<CornerGeode*>(LegoFactory<CornerGeode, QString>::create("CornerGeode"))))
            qDebug() << "Cannot cast in CornerGeode* within MainWindow::chooseDialog";
        break;
    case 2:
        if ((_currLego = dynamic_cast<Slop*>(LegoFactory<Slop, QString>::create("Slop")))) {
            Slop* lego = static_cast<Slop*>(_currLego);
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Slop* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<SlopGeode*>(LegoFactory<SlopGeode, QString>::create("SlopGeode"))))
            qDebug() << "Cannot cast in SlopGeode* within MainWindow::chooseDialog";
        break;
    case 3:
        if ((_currLego = dynamic_cast<Road*>(LegoFactory<Road, QString>::create("Road")))) {
            Road* lego = static_cast<Road*>(_currLego);
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Road* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<RoadGeode*>(LegoFactory<RoadGeode, QString>::create("RoadGeode"))))
            qDebug() << "Cannot cast in RoadGeode* within MainWindow::chooseDialog";
        break;
    case 4:
        if ((_currLego = dynamic_cast<Character*>(LegoFactory<Character, QString>::create("Character")))) {
            Character* lego = static_cast<Character*>(_currLego);
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Character* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<CharacterGeode*>(LegoFactory<CharacterGeode, QString>::create("CharacterGeode"))))
            qDebug() << "Cannot cast in CharacterGeode* within MainWindow::chooseDialog";
        break;
    }

    _currLegoGeode->setLego(_currLego);
    _currLegoGeode->createGeode();
    _scene->setChild(0, _currLegoGeode.get());

    _legoDialog.at(dialogIndex)->initLego(_currLego);
    _legoDialog.at(dialogIndex)->initLegoGeode(_currLegoGeode.get());
    _legoDialog.at(dialogIndex)->reInitComboBox();
}

void MainWindow::legoUpdated(LegoGeode* legoGeode) {
    _currLegoGeode = legoGeode;
    _currLego = _currLegoGeode->getLego();
}

void MainWindow::createLego(void) {
    _zTransSpinBox->setEnabled(true);

    _paramsDock->setEnabled(false);
    _moveDock->setEnabled(true);

    _world.addBrick(_currLegoGeode.get());

    // Reinit dialog
    _xTransSpinBox->setValue(0);
    _yTransSpinBox->setValue(0);
    _zTransSpinBox->setValue(0);

    if (dynamic_cast<Road*>(_currLego)) {
        _zTransSpinBox->setValue(World::minHeight);
        _zTransSpinBox->setEnabled(false);
    }
}

void MainWindow::fitLego(void) {
    _moveDock->setEnabled(false);
    _paramsDock->setEnabled(true);
}

void MainWindow::translate(int) {
    _world.translationXYZ(_xTransSpinBox->text().toInt(), _yTransSpinBox->text().toInt(), _zTransSpinBox->text().toInt());
}

void MainWindow::rotateLeft(void) {
    _world.rotation(true);
}

void MainWindow::rotateRight(void) {
    _world.rotation(false);
}

void MainWindow::chooseRoad(int i, int j, int width, int length, bool roadTop, bool roadRight) {
    Road* road = new Road;

    int nbRotations = 0;
    int roadType = -1;

    if (roadTop && roadRight) {
        roadType = round((rand()/(double)RAND_MAX) * 3);
        qDebug() << QString("Random1 = %1").arg(roadType);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 1;
            break;
        case 1:
            road->setRoadType(Road::intersection);
            nbRotations = 2;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 1;
            break;
        case 2:
            road->setRoadType(Road::intersection);
            nbRotations = 1;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        case 3:
            road->setRoadType(Road::cross);
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        }
    } else if (roadTop && !roadRight) {
        roadType = round((rand()/(double)RAND_MAX) * 2);
        qDebug() << QString("Random2 = %1").arg(roadType);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            nbRotations = 3;
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        case 1:
            road->setRoadType(Road::straight);
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 1;
            break;
        case 2:
            road->setRoadType(Road::intersection);
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        }
    } else if (!roadTop && roadRight) {
        roadType = round((rand()/(double)RAND_MAX) * 2);
        qDebug() << QString("Random3 = %1").arg(roadType);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            nbRotations = 1;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 0;
            break;
        case 1:
            road->setRoadType(Road::straight);
            nbRotations = 1;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 0;
            break;
        case 2:
            road->setRoadType(Road::intersection);
            nbRotations = 3;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 0;
            break;
        }
    } else {
        roadType = round((rand()/(double)RAND_MAX));
        qDebug() << QString("Random4 = %1").arg(roadType);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            nbRotations = 2;
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 0;
            break;
        case 1:
            road->setRoadType(Road::none);
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 0;
            break;
        }
    }

    RoadGeode* roadGeode = new RoadGeode(road);
    _world.addBrick(roadGeode);
    _world.translation(-32*floor(length/2)+16 + 32*i, -32*floor(width/2)+16 + 32*j, -10);
    for (int k = 0; k < nbRotations; k++)     {
        _world.rotation(true);
    }
}

void MainWindow::generateRoad(void) {
    GenerateRoadWindow* roadWindow = new GenerateRoadWindow(this);

    if (roadWindow->exec() == QDialog::Accepted) {
        int width = roadWindow->getWidth();
        int length = roadWindow->getLength();

        // 4 sides: left top right bottom; true = road, false = no road.
        _roadPath.clear();
        _roadPath = QVector<QVector<QVector<bool> > >(width, QVector<QVector<bool> >(length, QVector <bool>(4, false)));

        bool roadTop;
        bool roadLeft;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < length; j++) {
                if (i == 0 && j == 0) {
                    roadTop = round((rand()/(double)RAND_MAX));
                    roadLeft = round((rand()/(double)RAND_MAX));
                } else if (i == 0 && j != 0) {
                    roadTop = _roadPath[i][j-1][1];
                    roadLeft = round((rand()/(double)RAND_MAX));
                } else if (i != 0 && j == 0) {
                    roadTop = round((rand()/(double)RAND_MAX));
                    roadLeft = _roadPath[i-1][j][2];
                } else {
                    roadTop = _roadPath[i][j-1][1];
                    roadLeft = _roadPath[i-1][j][2];
                }

                qDebug() << QString("%1 %2 %3 %4 %5 %6").arg(i).arg(j).arg(width).arg(length).arg(roadTop).arg(roadLeft);

                chooseRoad(i, j, width, length, roadTop, roadLeft);

            }
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < length; j++) {
                for (int k = 0; k < 4; k++) {
                    qDebug() << QString("%1").arg(_roadPath[i][j][k]);
                }
                qDebug() << "\n";
            }
        }
    }
}


// ////////////////////////////////////
// Create menus
// ////////////////////////////////////

void MainWindow::createFileMenu(void) {
    // Create File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    // Add New sub menu
    _newAction = fileMenu->addAction("&New...");
    _newAction->setShortcut(QKeySequence::New);

    // Add separator
    fileMenu->addSeparator();

    // Add Open sub menu
    _openAction = fileMenu->addAction("&Open...");
    _openAction->setShortcut(QKeySequence::Open);

    // Add separator
    fileMenu->addSeparator();

    // Add Save sub menu
    _saveAction = fileMenu->addAction("&Save");
    _saveAction->setShortcut(QKeySequence::Save);

    // Add Save as sub menu
    _saveAsAction = fileMenu->addAction("Save &as...");
    _saveAsAction->setShortcut(QKeySequence::SaveAs);

    // Add separator
    fileMenu->addSeparator();

    // Add Quit sub menu
    _quitAction = fileMenu->addAction("&Quit");
    _quitAction->setShortcut(QKeySequence::Quit);
}

void MainWindow::createGenerateMenu(void) {
    // Create Generate menu
    QMenu* generateMenu = menuBar()->addMenu("&Generate");

    // Add Generate road sub menu
    _generateRoadAction = generateMenu->addAction("Generate &road...");
    _generateRoadAction->setShortcut(QKeySequence("CTRL+SHIFT+R"));

    // Add Generate building sub menu
    _generateBuildingAction = generateMenu->addAction("Generate &building...");
    _generateBuildingAction->setShortcut(QKeySequence("CTRL+SHIFT+B"));

    // Add Generate house sub menu
    _generateHouseAction = generateMenu->addAction("Generate &house...");
    _generateHouseAction->setShortcut(QKeySequence("CTRL+SHIFT+H"));
}

void MainWindow::createHelpMenu(void) {
    // Create ? menu
    QMenu* helpMenu = menuBar()->addMenu("&?");

    // Add Help sub menu
    _helpAction = helpMenu->addAction("&Help");
    _helpAction->setShortcut(QKeySequence::HelpContents);

    // Add separator
    helpMenu->addSeparator();

    // Add About sub menu
    _aboutAction = helpMenu->addAction("&About");
    _aboutAction->setShortcut(QKeySequence("Alt+F1"));
}



// ////////////////////////////////////
// STYLE SHEETS
// ////////////////////////////////////
void MainWindow::setStyle(void) {
    //_paramsWidget->setStyleSheet("background-image: url(../LEGO_GIT/IMG/tileBrick.png);");
    //_paramsWidget->setStyleSheet("background: yellow;");

    QString dockWidgetStyle = "";
    dockWidgetStyle += "QDockWidget { ";
    dockWidgetStyle += "    font-size: 10pt;";
    dockWidgetStyle += "    font-weight: bold;";
    dockWidgetStyle += "    font-style: italic;";
    dockWidgetStyle += "    font-family: 'KG Lego House';";
    dockWidgetStyle += "    border: solid 3px black;";
    dockWidgetStyle += "    color: #ffffff;";
    dockWidgetStyle += "}";

    QString dockWidgetTitleStyle;
    dockWidgetTitleStyle += "QDockWidget::title {";
    dockWidgetTitleStyle += "    text-align: center;";
    dockWidgetTitleStyle += "    padding: 3px;";
    dockWidgetTitleStyle += "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    dockWidgetTitleStyle += "                stop: 0 #6a6a6a, stop: 0.4 #444444,";
    dockWidgetTitleStyle += "                stop: 0.5 #272727, stop: 1.0 #4a4a4a);";
    //dockWidgetTitleStyle += "    background: red;";
    dockWidgetTitleStyle += "}";

    QString dockWidgetButtonsStyle;
    dockWidgetButtonsStyle += "QDockWidget {";
    dockWidgetButtonsStyle += "    titlebar-close-icon: url(\"./icones/closeIcon.png\");";
    dockWidgetButtonsStyle += "    titlebar-normal-icon: url(\"./icones/reduceIcon.png\");";
    dockWidgetButtonsStyle += "}";
    dockWidgetButtonsStyle += "QDockWidget::close-button, QDockWidget::float-button {";
    dockWidgetButtonsStyle += "    padding: 0px;";
    dockWidgetButtonsStyle += "}";
    dockWidgetButtonsStyle += "QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {";
    dockWidgetButtonsStyle += "    padding: 1px -1px -1px 1px;";
    dockWidgetButtonsStyle += "}";

    QString style = dockWidgetStyle + dockWidgetTitleStyle + dockWidgetButtonsStyle;
    setStyleSheet(style);
}
