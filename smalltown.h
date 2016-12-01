#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <string>
#include <tuple>
#include "citizen.h"
#include "monster.h"

//compile-time fibonacci
namespace detail {
    template<std::size_t N>
    struct Fibonacci :
            std::integral_constant<std::size_t,
                    Fibonacci<N - 1>::value + Fibonacci<N - 2>::value> {
    };

    template<>
    struct Fibonacci<0> : std::integral_constant<std::size_t, 1> {
    };
    template<>
    struct Fibonacci<1> : std::integral_constant<std::size_t, 2> {
    };

    template<std::size_t N, std::size_t ... Is>
    constexpr std::array<std::size_t, N> fib_seq(std::index_sequence<Is...>) {
        return std::array<std::size_t, N>{{Fibonacci<Is>::value...}};
    }
}

constexpr std::size_t fib_size = 92; //size_t max fibonacci numbers
constexpr std::array<std::size_t, fib_size> fib = detail::fib_seq<fib_size>(
        std::make_index_sequence<fib_size>());

template<typename U>
bool is_fibonacci_number(U num) {
    for (std::size_t idx = 0; idx < fib_size; idx++) {
        if (fib[idx] == num) return true;
    }
    return false;
}


template<typename M, typename U, U t0, U t1, typename... C>
class SmallTown {
public:
    SmallTown(M monster, C... citizens) : _monster(monster), _time(t0),
                                          _citizens(citizens...),
                                          _citizens_alive(sizeof...(citizens)) {
    }

    std::tuple<std::string, typename M::valueType, std::size_t> getStatus() {
        std::tuple<std::string, typename M::valueType, std::size_t> ret(
                _monster.name(),
                _monster.getHealth(),
                _citizens_alive);
        return ret;
    }

    void tick(U timestep) {
        timestep %= t1;
        if (_monster.getHealth() > 0) {
            if (_citizens_alive == 0) {
                std::cout << "MONSTER WON" << std::endl;
            }
            else {
                if (is_fibonacci_number(_time)) {
                    _citizens_alive = attack_citizens(_citizens);
                }
            }
        }
        else if (_citizens_alive > 0) {
            std::cout << "CITIZENS WON" << std::endl;
        }
        else {
            std::cout << "DRAW" << std::endl;
        }
        _time = (_time + timestep) % t1;
    }

private:
// attack every living citizen
// return how many citizens are alive after attack
    template<std::size_t I = 0>
    inline typename std::enable_if<I < sizeof...(C), std::size_t>::type
    attack_citizens(std::tuple<C...> &citizens) {
        auto &current_citizen = std::get<I>(citizens);
        std::size_t alive = 0;
        if (current_citizen.getHealth() > 0) {
            alive = 1;
            attack(_monster, current_citizen);
            if (current_citizen.getHealth() == 0) {
                alive = 0;
            }
        }
        return attack_citizens<I + 1>(citizens) + alive;
    }

    template<std::size_t I = 0>
    inline typename std::enable_if<I == sizeof...(C), std::size_t>::type
    attack_citizens(std::tuple<C...> &) {
        return 0;
    }

    M _monster;
    U _time;
    std::tuple<C...> _citizens;
    std::size_t _citizens_alive;
};

#endif //SMALLTOWN_H
