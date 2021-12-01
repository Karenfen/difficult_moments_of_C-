#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <deque>
#include <cmath>
#include <numeric>
#include <functional>

#define SIZE 100

void insert_sorted(std::vector<int>& vec, const int value)
{
    std::vector<int>::iterator position_insert { std::lower_bound(vec.begin(), vec.end(), value) };
    vec.insert(position_insert, value);
}

template<typename ValueType, typename ConteinerType>
void insert_sorted(ConteinerType& container, const ValueType& value)
{
    auto position_insert { std::lower_bound(container.begin(), container.end(), value) };
    container.insert(position_insert, value);
}


int main()
{
    // task 1
    std::cout << " Task - 1 \n\n";
    {
        std::vector<int> vec(10);
        std::generate(vec.begin(), vec.end(), [](){return (rand()%10);});
        std::sort(vec.begin(), vec.end());
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>{std::cout, " "});

        auto number { 5 };

        std::cout << "\n insert the number: " << number << std::endl;
        insert_sorted(vec, number);

        number = 3 ;

        std::cout << " insert the number: " << number << std::endl;
        insert_sorted(vec, number);

        number = 10 ;

        std::cout << " insert the number: " << number << std::endl;
        insert_sorted(vec, number);

        number = 6 ;

        std::cout << " insert the number: " << number << std::endl;
        insert_sorted(vec, number);
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>{std::cout, " "});

        std::cout << "\n\n";
        std::deque<double> my_deque { 1.15655, 5.54686, 3.2465, 3.5625, 8.5691, 6.652};
        std::sort(my_deque.begin(), my_deque.end());
        std::copy(my_deque.begin(), my_deque.end(), std::ostream_iterator<double>{std::cout, " "});

        double number_d = 2.1256;

        insert_sorted(my_deque, number_d);
        std::cout << "\n insert the number: " << number_d << std::endl;
        std::copy(my_deque.begin(), my_deque.end(), std::ostream_iterator<double>{std::cout, " "});

        std::string str {"sdjfgsgfbf"};
        std::sort(str.begin(), str.end());
        std::cout << "\n\n" << str << std::endl;

        char symbol{'c'};

        std::cout << " insert the symbol: " << symbol << std::endl;
        insert_sorted(str, symbol);
        std::cout << str << std::endl;
    }
    // task 2
    std::cout << "\n Task - 2 \n\n";
    {
        std::vector<double> analog_vec(SIZE);
        std::vector<int> digital_vec(SIZE);

        std::generate(analog_vec.begin(), analog_vec.end(), [](){ return (rand()/10000.0 + (rand()%100)*1.0); } );
        std::copy(analog_vec.begin(), analog_vec.begin() + 20, std::ostream_iterator<double>{std::cout, " "});
        std::transform(analog_vec.begin(), analog_vec.end(), digital_vec.begin(), []( const double& signal){ return (static_cast<int>(std::trunc(signal))); } );
        std::cout << " ... \n";
        std::copy(digital_vec.begin(), digital_vec.begin() + 20, std::ostream_iterator<int>{std::cout, "\t"});
        std::cout << " ... \n";

        std::function inacuraci = [](const double& first, const int& second) -> double{
            double error{ first - static_cast<double>(second) };
            return error*error;
        };
        std::cout << " total error: " << std::inner_product(analog_vec.begin(), analog_vec.end(), digital_vec.begin(), 0.0, std::plus<double>(), inacuraci) << std::endl;

    }
}