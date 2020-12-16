//
// Created by matt on 2020-04-06.
//
#include <iostream>
#include <utility>

template <typename K, typename V> class MapEntry;
template <typename K, typename V> std::ostream& operator<<(std::ostream&, const MapEntry<K, V>&);

template <typename K, typename V>
class MapEntry : public std::pair<K, V> {
public:
    using std::pair<K, V>::pair;

    MapEntry(const std::pair<K, V>& p) : std::pair<K, V>(p) {}
    MapEntry(std::pair<K, V>&& p) : std::pair<K, V>(std::move(p)) {}

    bool operator<(const MapEntry<K, V>& me2) {
        return this->first < me2.first;
    }

    bool operator>(const MapEntry<K, V>& me2) {
        return this->first > me2.first;
    }

    bool operator==(const MapEntry<K, V>& me2) {
        return this->first == me2.first;
    }


    bool operator<=(const MapEntry<K, V>& me2) {
        return !(*this > me2);
    }

    bool operator>=(const MapEntry<K, V>& me2) {
        return !(*this < me2);
    }

    bool operator!=(const MapEntry<K, V>& me2) {
        return !(*this == me2);
    }

    friend std::ostream& operator<<<K, V>(std::ostream&, const MapEntry<K, V>&);
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& out, const MapEntry<K, V>& me) {
    return out << me.second;
}