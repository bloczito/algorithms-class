# pragma once
#include <utility>
#include <iostream>


template <typename K, typename V>
class MapElem {
public:
    mutable std::pair<K, V> element;

    MapElem() = default;
    MapElem(const K & key, const V & value)
            : element(std::make_pair(key, value)) { }

    bool operator==(const MapElem & source) const{
        return element.first == source.element.first;
    }

    bool operator<(const MapElem & source) const {
        return element.first < source.element.first;
    }

    bool operator>(const MapElem & source) const {
        return element.first > source.element.first;
    }

    bool operator<=(const MapElem & source) const {
        return element.first <= source.element.first;
    }

    friend std::ostream & operator<< (std::ostream & stream,
                                      const MapElem & elem) {

        return stream << "[" << elem.element.first << " "
                      << elem.element.second << "], ";
    }
};

