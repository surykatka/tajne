#ifndef CITIZEN_H
#define CITIZEN_H

#include <cassert>

template<typename T, T minAge, T maxAge, bool attack>
class Citizen {
public:
    template<bool cannotAttack = !attack, typename = typename std::enable_if<cannotAttack, T>::type>
    Citizen(T health, T age) : _health(health), _age(age) {
        assert(minAge <= age && age <= maxAge);
    }

    template<bool canAttack = attack, typename = typename std::enable_if<canAttack, T>::type>
    Citizen(T health, T age, T attackPower) : _health(health), _age(age),
                                              _attackPower(attackPower) {
        assert(minAge <= age && age <= maxAge);
    }

    T getHealth() const {
        return _health;
    }

    T getAge() const {
        return _age;
    }

    template<bool canAttack = attack>
    typename std::enable_if<canAttack, T>::type getAttackPower() const {
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

private:
    T _health;
    T _age;
    T _attackPower;
};


template<typename T>
using Sheriff = Citizen<T, 18, 100, true>;

template<typename T>
using Adult = Citizen<T, 18, 100, false>;

template<typename T>
using Teenager = Citizen<T, 11, 17, false>;

#endif //CITIZEN_H
