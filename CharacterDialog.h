#ifndef CHARACTERDIALOG_H
#define CHARACTERDIALOG_H

#include "LegoDialog.h"
#include "Character.h"
#include <QtGui>
#include "CharacterNode.h"

class CharacterDialog : public LegoDialog
{
    Q_OBJECT

public:
    CharacterDialog(QWidget* parent = 0);
    CharacterDialog(const CharacterDialog& characterDialog);

    void reInitComboBox(void);

    virtual CharacterDialog* cloning(void) const;

public slots:
    virtual void setLego(int);

private:
    QComboBox* _characterTypeComboBox;
};

#endif // CHARACTERDIALOG_H
