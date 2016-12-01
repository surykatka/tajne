#ifndef MONSTER_H
#define MONSTER_H

#include "citizen.h"

enum monsterType {
    ZOMBIE,
    VAMPIRE,
    MUMMY
};

const std::string monsterNames[]{
    "Zombie",
    "Vampire",
    "Mummy"
};

template<typename T, monsterType type>
class Monster {
public:
    Monster(T health, T attackPower) : _health(health),
                                       _attackPower(attackPower) { }

    T getHealth() const {
        return _health;
    }

    T getAttackPower() const {
        return _attackPower;
    }

    void takeDamage(T damage) {
        if (_health <= damage) {
            _health = 0;
        }
        else {
            _health -= damage;
        }
    }

    static const std::string name() {
        return monsterNames[type];
    }

    typedef T valueType;
private:
    T _health;
    T _attackPower;
};

template<typename T>
struct is_sheriff : std::false_type {
};

template<typename T>
struct is_sheriff<Sheriff<T> > : std::true_type {
};

template<typename M, typename U>
typename std::enable_if<!is_sheriff<U>::value>::type
attack(M &monster, U &victim) {
    victim.takeDamage(monster.getAttackPower());
};

template<typename M, typename U>
typename std::enable_if<is_sheriff<U>::value>::type
attack(M &monster, U &victim) {
    victim.takeDamage(monster.getAttackPower());
    monster.takeDamage(victim.getAttackPower());
};

template<typename T>
using Zombie = Monster<T, ZOMBIE>;

template<typename T>
using Vampire = Monster<T, VAMPIRE>;

template<typename T>
using Mummy = Monster<T, MUMMY>;

#endif //MONSTER_H
