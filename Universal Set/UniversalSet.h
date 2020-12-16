#pragma once

#include <utility>
#include <iterator>
#include <cstdint>
#include <array>
#include <bitset>

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszac odpowiednie wyjątki!

// klasa reprezentująca zbiór uniwersalny (uniwersum)
template<typename T, std::size_t UNIVERSE_SIZE>
class UniversalSet
{
public:
// klasa reprezetująca zbiór (wektor), który jest podzbiorem uniwersum
    class Vector
    {
    public:
// iterator, który jest jednocześnie cost_iterator-em
        class Iterator : public std::iterator<std::input_iterator_tag, const T>
        {
        public:
            Iterator(const Iterator&) = default;
            Iterator(const Vector *vector_ptr) : vector_ptr(vector_ptr), index(0) {}
            Iterator(const Vector * vector_ptr, std::size_t index) : vector_ptr(vector_ptr), index(index) {}
            Iterator& operator=(const Iterator&) = default;
            Iterator(Iterator&&) noexcept = default;
            Iterator& operator=(Iterator&&) noexcept = default;

            friend class Vector;

        private:
            const Vector *vector_ptr;
            std::size_t index;

        public:
            bool operator==(const Iterator& it) const { return (index == it.index); }
            bool operator!=(const Iterator& it) const { return (index != it.index); }
            Iterator& operator++();
            Iterator operator++(int);
            const T& operator*() const;
            const T* operator->() const;
            operator bool() const;
        }; // ==================== KONIEC ITERATORA ======================

    public:
// ...
        Vector(const Vector&) = default;
        explicit Vector(const UniversalSet *universe_ptr) : universe_ptr(universe_ptr) {}
        Vector(const UniversalSet *universe_ptr, const std::bitset<UNIVERSE_SIZE> & bits) : universe_ptr(universe_ptr), bits(bits) {}
        Vector& operator=(const Vector&) = default;
        Vector(Vector&&) noexcept = default;
        Vector& operator=(Vector&&) noexcept = default;

        friend class Iterator;

    private:
        std::bitset<UNIVERSE_SIZE> bits;
        const UniversalSet<T, UNIVERSE_SIZE> *universe_ptr;

    public:
// Zwraca liczbę elementów reprezentowanych przez "Vector"
        std::size_t count() const { return bits.count(); }
// Jezeli element o indeksie "i" nie należy do "Vector" to dodaje ten element oraz zwraca "Iterator" do dodanego elementu i "true", w przeciwnym razie zwraca samo co "end()" i "false"
        std::pair<Iterator, bool> insert(std::size_t i);
// Jezeli element o indeksie "i" należy do "Vector" to zwraca "true", w przeciwnym razie zwraca "false"
        bool isMember(std::size_t i) const { return bits.test(i); }
// Jeżeli element o indeksie "i" należy do "Vector" to zwraca "Iterator" do tego elementu, w przeciwnym razie zwraca to samo co "end()"
        Iterator elemIterator(std::size_t i) const;
// Jezeli element o indeksie "i" należy do "Vector" to usuwa ten element i zwraca "true", w przeciwnym razie zwraca "false"
        bool remove(std::size_t i);
// Zwraca "true" jeżeli obiekt "v2" reprezentuje ten sam zestaw elementów z tego samego uniwersum, w przeciwnym razie zwraca "false"
        bool operator==(const Vector& v2) const { return (bits == v2.bits) && (*universe_ptr == v2); }
        bool operator!=(const Vector& v2) const { return (bits != v2.bits) && (*universe_ptr != v2); }
// Zwraca sumę zbiorow
        Vector operator+(const Vector& v2) const;
// Zwraca różnicę zbiorów
        Vector operator-(const Vector& v2) const;
// Zwraca część wspólną zbiorów
        Vector operator*(const Vector& v2) const;
// Zwraca roznicę symetryczną zbiorów
        Vector operator/(const Vector& v2) const;
// Zwraca "Iterator" na początek
        Iterator begin() const;
// Zwraca "Iterator" za koniec
        Iterator end() const;
    };

public:
    explicit UniversalSet(const std::array<T, UNIVERSE_SIZE>& elems) : elems_(elems) {}
    UniversalSet(const UniversalSet&) = default;
    UniversalSet& operator=(const UniversalSet&) = default;
    UniversalSet(UniversalSet&&) noexcept = default;
    UniversalSet& operator=(UniversalSet&&) noexcept = default;

private:
    const std::array<T, UNIVERSE_SIZE> elems_;

public:
    constexpr std::size_t universeSize() const { return UNIVERSE_SIZE; }
    const T& elem(std::size_t i) const { return elems_[i]; }
    const T& operator[](std::size_t i) const { return elems_[i]; }
// Zwraca pusty "Vector"
    Vector makeVector() const { return Vector(this); }
};


//METODY KLAY VECTOR

template<typename T, std::size_t UNIVERSE_SIZE>
std::pair<typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator, bool>
UniversalSet<T, UNIVERSE_SIZE>::Vector::insert(std::size_t i) {

    if(bits.test(i))
        return std::make_pair(Iterator(this, bits.size()), false);
    else {
        bits[i] = true;
        return std::make_pair(Iterator(this, i), true);
    }
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator
UniversalSet<T, UNIVERSE_SIZE>::Vector::elemIterator(std::size_t i) const {

    if(bits[i]) {
        return Iterator(this, i);
    } else {
        return Iterator(this, bits.size());
    }
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::remove(std::size_t i) {
    if (bits.test(i)) {
        bits[i] = 0;
        return true;
    } else {
        return false;
    }
}



//===== OPERATORY KLASY VECTOR =====
template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator+(const UniversalSet::Vector &v2) const {
    std::bitset<UNIVERSE_SIZE> result(bits | v2.bits);
    return Vector(universe_ptr, result);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator-(const UniversalSet::Vector &v2) const {
    std::bitset<UNIVERSE_SIZE> result(bits & ~v2.bits);
    return Vector(universe_ptr, result);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator*(const UniversalSet::Vector &v2) const {
    std::bitset<UNIVERSE_SIZE> result(bits & v2.bits);
    return Vector(universe_ptr, result);
}
template<typename T, size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector
UniversalSet<T, UNIVERSE_SIZE>::Vector::operator/(const UniversalSet::Vector &v2) const {
    std::bitset<UNIVERSE_SIZE> result(bits ^ v2.bits);
    return Vector(universe_ptr, result);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T, UNIVERSE_SIZE>::Vector::begin() const {
    int start = 0;
    while (!bits[start])
        ++start;

    return Iterator(this, start);
}

template<typename T, size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T, UNIVERSE_SIZE>::Vector::end() const {
    return Iterator(this, bits.size());
}



//============= ITERATOR ===================

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator &UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator++() {
    do {
        ++index;
    } while (!vector_ptr->bits[index] && index < vector_ptr->bits.size());

    return *this;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator++(int) {
    Iterator it(*this);
    ++(*this);
    return it;
}

template<typename T, std::size_t UNIVERSE_SIZE>
const T &UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator*() const {
    return vector_ptr->universe_ptr->elems_[index];
}

template<typename T, size_t UNIVERSE_SIZE>
const T *UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator->() const {
    return &vector_ptr->universe_ptr->elements[index];
}

template<typename T, size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator bool() const {
    return index < UNIVERSE_SIZE;
}

