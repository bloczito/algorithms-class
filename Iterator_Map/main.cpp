#include <iostream>
#include <string>
#include "Map.h"
#include <vector>

using namespace std;

template<typename Iter>
void printDebugInfo(const std::pair<Iter, bool>& insert_info)
{
    std::cout << (insert_info.second ? "Dodano " + *insert_info.first : "Nie dodano ;/") << std::endl;
}

template<typename Iter>
void printDebugFindInfo(const Iter& find_info)
{
    std::cout << (find_info ? "Znaleziono " + *find_info : "Nie znaleziono ;/") << std::endl;
}

template<typename Iter>
void printDebugRemoveInfo(const Iter& next)
{
    std::cout << (next ? "Usunięto, następny element to: " + *next : "Nie usunięto, lub następny element to end()") << std::endl;
}

void printDebugRemoveInfo(bool removed)
{
    std::cout << (removed ? "Usunięto" : "Nie usunięto") << std::endl;
}

template<typename K, typename V>
void printSetSizeAndElems(const Map<K, V>& m)
{
    std::cout << "Size: " << m.size() << std::endl;
    for(const auto& e : m) std::cout << e << ", ";
    std::cout << std::endl;
}

int main()
{
    Map<int, std::string> m;

    {
        printDebugInfo(m.insert({8, "eight"}));
        printDebugInfo(m.insert({4, "four"}));
        printDebugInfo(m.insert({10, "ten"}));
        printDebugInfo(m.insert({10, "2nd ten"}));
        printDebugInfo(m.insert({2, "two"}));
        printDebugInfo(m.insert({5, "five"}));
        printDebugInfo(m.insert({9, "nine"}));
        printDebugInfo(m.insert({12, "twelve"}));
        printDebugInfo(m.insert({12, "2nd twelve"}));
        printDebugInfo(m.insert({1, "one"}));
        printDebugInfo(m.insert({14, "fourteen"}));
        printDebugInfo(m.insert({7, "seven"}));
        std::cout << std::endl;

        std::cout << "Size: " << m.size() << std::endl;
        for(auto it = m.begin(); it != m.end(); ++it) { std::cout << *it << ", "; }
        std::cout << std::endl;
        for(auto& e : m) { std::cout << e << ", "; }
        std::cout << std::endl;
        {
            auto m2 = m;
            printSetSizeAndElems(m2);
        }
        std::cout << std::endl;

        auto it = m.begin();
        std::cout << *++it << std::endl;
        std::cout << std::endl;

        it = m.find(10);
        printDebugFindInfo(it);
        *it = "10 (corrected)";
        printDebugFindInfo(m.find(5));
        printDebugRemoveInfo(m.remove(4));
        printDebugFindInfo(m.find(5));
        printDebugRemoveInfo(m.remove(12));
        printDebugRemoveInfo(m.remove(14));
        printDebugInfo(m.insert({0, "zero"}));
        printDebugInfo(m.insert({16, "sixteen"}));
        {
            Map<int, std::string> m3;
            m3 = m;
            printSetSizeAndElems(m3);
        }
        printSetSizeAndElems(m);
        printDebugRemoveInfo(m.remove(8));
        printSetSizeAndElems(m);
        std::cout << std::endl;

        std::vector<Map<int, std::string>> vm, vm2;
        vm.resize(1000);
        for(auto& e : vm)
        {
            e = m;
            e.insert({13, "thirteen"});
        }
        vm2.resize(1000);
        for(std::size_t i = 0u; i < vm.size(); ++i) { vm2[i] = std::move(vm[i]); }
        for(auto& e : vm2) { e.remove(10); }
        vm = vm2;
        vm2.clear();
        const auto m2 = std::move(vm.front());
        auto it2 = m2.find(13);
        printDebugFindInfo(it2);
// *it2 = "nie powinno sie skompilowac";
// m2[2] = "nie powinno sie skompilowac";
        printSetSizeAndElems(m);
        printSetSizeAndElems(m2);
        printDebugFindInfo(m2.find(5));
        std::cout << std::endl;

        auto m3 = m2;
        printSetSizeAndElems(m3);
        printDebugRemoveInfo(m3.remove(2));
        printDebugRemoveInfo(m3.remove(2));
        m = m3;
        m[2] = "two (corrected)";
        m[20] = "twenty";
    }

    std::cout << std::endl;
    printSetSizeAndElems(m);
    return 0;
}