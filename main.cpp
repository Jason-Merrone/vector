#include "vector.hpp"

#include <print>
template <typename T>
void report(T& vector)
{
    std::print("size : {}\ncapacity: {}\nvalues : ", vector.size(),
               vector.capacity());
    for (auto i = vector.begin(); i != vector.end(); i++)
    {
        std::print("{}, ", *i);
    }
    std::print("\n");
}
// ------------------------------------------------------------------
//
// Final project, a vector with a bool specialization
//
// ------------------------------------------------------------------
int main()
{
    // Simple vector construction
    usu::vector<int> v1;
    usu::vector<int> v2(20);
    usu::vector v3{ 1, 2, 3, 5, 7, 11 };
    std::print("\n-- v1 --\n");
    report(v1);
    std::print("\n-- v2 --\n");
    report(v2);
    std::print("\n-- v3 --\n");
    report(v3);
    // Simple vector add/insert/remove
    v3.add(23);
    std::print("\n-- add --\n");
    report(v3);
    v3.insert(0, 13);
    std::print("\n-- insert at 0 --\n");
    report(v3);
    v3.insert(4, 17);
    std::print("\n-- insert at 4 --\n");
    report(v3);
    v3.insert(9, 19);
    std::print("\n-- insert at 9 --\n");
    report(v3);
    v3.remove(0);
    std::print("\n-- remove at 0 --\n");
    report(v3);
    v3.remove(3);
    std::print("\n-- remove at 3 --\n");
    report(v3);
    v3.remove(7);
    std::print("\n-- remove at 7 --\n");
    report(v3);
    // Insert until new capacity is required
    v3.insert(0, 29);
    v3.insert(0, 31);
    v3.insert(0, 37);
    std::print("\n-- maxed capacity --\n");
    report(v3);
    v3.insert(0, 41);
    std::print("\n-- updated capacity --\n");
    report(v3);
    // resize with custom lambda
    usu::vector<int> v4({ 1, 2, 3, 5, 7, 11, 13, 17, 19 }, [](auto size)
                        {
                            return static_cast<usu::vector<int>::size_type>(size *
                                                                            1.5);
                        });
    std::print("\n-- initial capacity --\n");
    report(v4);
    v4.add(23);
    std::print("\n-- maxed capacity --\n");
    report(v4);
    v4.add(29);
    std::print("\n-- updated capacity --\n");
    report(v4);
    std::print("\n -- iteration --\n");
    // Simple iteration - postfix
    for (auto i{ v4.begin() }; i != v4.end(); i++)
    {
        std::print("{}, ", *i);
    }
    std::print("\n");
    // Simple iteration - prefix
    for (auto i{ v4.begin() }; i != v4.end(); ++i)
    {
        std::print("{}, ", *i);
    }
    std::print("\n");
    // Simple iteration - decrement
    auto j{ v4.begin() };
    ++j;
    ++j;
    std::print("{}, \n", *j);
    --j;
    std::print("{}, \n", *j);
    j--;
    std::print("{}, \n", *j);
    // for-each iteration
    for (auto&& value : v4)
    {
        std::print("{}, ", value);
    }
    std::print("\n");
    // Mapping of a function over all the elements
    usu::vector v5{ 1, 2, 3, 4, 7, 11 };
    std::printf("\n-- before applying function --\n");
    report(v5);
    v5.map([](auto& v)
           {
               v *= 2;
           });
    std::printf("\n-- after applying function --\n");
    report(v5);
    return 0;
}