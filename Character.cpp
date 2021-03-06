#include "Character.h"
#include <QObject>

Character::Character(CharacterType characterType, const QColor &color) :
    Lego(color),
    _characterType(characterType) {

     calculateBoundingBox();
}

Character::Character(const Character& character) :
    Lego(character) {

    _characterType = character._characterType;

    calculateBoundingBox();
}

void Character::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 2, 1, 12);
}

void Character::setCharacterType(int index) {
    switch (index) {
    case 0 :
        _characterType = classic;
        break;
    case 1 :
        _characterType = R2D2;
        break;
    }
}

Character* Character::cloning(void) const {
    return new Character(*this);
}

QString Character::whoiam(void) const {
    return "Character";
}
