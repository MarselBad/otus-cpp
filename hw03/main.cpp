#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>

#include "custom_allocator.h"
#include "custom_container.h"


constexpr int fact(const int val) {
    if (val < 0) {
        throw std::invalid_argument("Factorial of a negative number is invalid!");
    }
    int ret = 1;
    for (int i = 1; i <= val; ++i) {
        ret *= i;
    }
    return ret;
}


int main(int, char const* [])
{       
    constexpr int n = 10;

    {
        // создание экземпляра std::map<int, int>
        std::map<int, int> m;
        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
        for (int i = 0; i < n; ++i) {
            m[i] = fact(i);
        }
    }

    {
        // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
        std::map<int, int, std::less<>, custom_allocator<std::pair<const int, int>, n>> m;
        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
        for (int i = 0; i < n; ++i) {
            m[i] = fact(i);
        }
        // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
        for (int i = 0; i < n; ++i) {
            std::cout << i << " " << m[i] << "\n";
        }
    }

    {
        // создание экземпляра своего контейнера для хранения значений типа int
        custom_list<int> my_list;
        // заполнение 10 элементами от 0 до 9
        for(int i = 0; i < n; ++i) {
            my_list.push_back(i);
        }
    }

    {
        // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором,
        // ограниченным 10 элементами
        custom_list<int, custom_allocator<int, n>> my_list;
        // заполнение 10 элементами от 0 до 9
        for(int i = 0; i < n; ++i) {
            my_list.push_back(i);
        }
        // вывод на экран всех значений, хранящихся в контейнере
        for (auto i : my_list) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

#if 0
    {
        using Alloc = custom_allocator<int, 10>;
        {
            std::vector<int, Alloc> a{1};
            std::vector<int, Alloc> b = a;
            std::cout << a[0] << std::endl;
            std::cout << b[0] << std::endl;
            a[0] = 3;
            a.push_back(4);
            std::cout << a[0] << ' ' << a[1] << std::endl;
            std::cout << b[0] << std::endl;
        }

        {
            Alloc a;
            auto p1 = a.allocate(1);
            auto p2 = a.allocate(1);
            a.deallocate(p1, 1);
            auto p3 = a.allocate(1);
            std::cout << p1 << std::endl;
            std::cout << p2 << std::endl;
            std::cout << p3 << std::endl;
        }
    }
    {
        custom_list<int> my_list;
        my_list.push_back(1);
        my_list.push_back(2);
    }
#endif

}
