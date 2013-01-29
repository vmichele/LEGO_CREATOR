#include "BrickDialog.h"

BrickDialog::BrickDialog(QWidget* parent) :
    LegoDialog(parent) {
}

BrickDialog::BrickDialog(const BrickDialog& brickDialog) :
    LegoDialog(brickDialog) {

    // Brick type
    _brickTypeComboBox = new QComboBox(this);
    QStringList brickTypeList;
    brickTypeList << "Classic" << "Plate" << "Flat";
    _brickTypeComboBox->addItems(brickTypeList);
    QFormLayout* brickTypeLayout = new QFormLayout;
    brickTypeLayout->addRow("Brick type:", _brickTypeComboBox);

    // Brick width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(2);
    _widthSpinBox->setValue(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Brick length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    _lengthSpinBox->setValue(4);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(brickTypeLayout);
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_brickTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));
    connect(_brickTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxWidth(int)));

    setLayout(mainLayout);
}

void BrickDialog::setLego(int) {
    if (Brick* brick = dynamic_cast<Brick*>(_lego)) {
        if (BrickGeode* brickGeode = dynamic_cast<BrickGeode*>(_legoGeode)) {
            brick->setWidth(_widthSpinBox->text().toInt());
            brick->setLength(_lengthSpinBox->text().toInt());
            brick->setBrickType(_brickTypeComboBox->currentIndex());

            brickGeode->createGeode();

            emit changedLego(brickGeode);
        } else {
            qDebug() << "Cannot cast in BrickGeode* BrickDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Brick* BrickDialog::setLego";
    }
}

void BrickDialog::updateMaxWidth(int brickType) {
    if (brickType == 1)
        _widthSpinBox->setMaximum(16);
    else
        _widthSpinBox->setMaximum(2);
}

BrickDialog* BrickDialog::cloning(void) const {
    return new BrickDialog(*this);
}
