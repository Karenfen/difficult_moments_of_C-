#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>


#if _WIN64 || _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif




// устанавливаем вместимость дома для третьего задания
namespace { constexpr uint8_t CAPASITY{ 20 };} 

// переменная для поимки вора
bool police{ false };  // подскажите, пожалуйста, как можно передать переменную в два потока по ссылке, чтобы один поток её изменял, а другой мог проверять её состояние 
std::mutex m_mutex;

// создаём новый класс для выходного потока 
class lock_cout
{
private:
    std::mutex m_mutex;
public:
template <typename data>
    void  operator<< (data& m_data)     // если можно перегрузит оператор для вызова " pcout << value << "string" << std::endl; " буду рад подсказке:)
    {
        std::lock_guard lg(m_mutex);
        std::cout << m_data << std::endl;
    }
};


// создаём обьект pcout
lock_cout pcout;

// вспомогательная функция проверки простого числа
bool check_to_prime(const uint64_t& number){
    
    // сразу отсеиваем 1,2 и чётные числа
    if (number == 1)
        return false;
    
    if (number == 2)
        return true;
    
    if (number % 2 == 0)
        return false;
    
    // после этого идем по нечётным делителям до половины числа
    uint64_t temp_numb { number / 2 };

    for (size_t i = 3; i < temp_numb; i += 2)
    {
        if (number % i == 0)
            return false;    // если нашли делитель, то возвращаем ложь
        else
            temp_numb = number / i;   // если исходное число не делится без остатка, то можно не идти дальше результата их деления
    }
    
    // если прошли весь интервал и не нашди делитель, значит число простое
    return true;
}

// основная функция для выдачи простого числа нужной позиции
uint64_t get_prime_numb(size_t target)
{
    // отсеиваем единственное чётное простое число
    if (target == 1)
        return 2;

    size_t counter{ 1 };
    uint64_t result { 1 };

    // идём по циклу пока наш счётчик не сравняется с введнным
    while (counter < target)
    {   
        // проверяем только нечётные числа 
        if (check_to_prime(result += 2)) 
        {
            // после нахождения очередного простого числа увеличивае счётчик
            ++counter;
        }
        
        // выводим процент выполнения
        std::cout <<  "\t" << ( 1.0 * counter / target * 100) << "%\t";

        // чистим консоль
        std::system(CLEAR);
    }
    // после возвращаем простое число в нужной позиции
    return result;
}

// хозяин приносит домой вещи разной стоимости
void owner(std::vector<int>& house)
{
    // приносит вещи пока дом не пуст или пока не заполнен
    while (!house.empty() && house.size() < CAPASITY)  
    {
        int thing{ rand() % 10000 };

        // эта проверка нужна была для полиции
        if (m_mutex.try_lock())
        {
            house.push_back(thing);
            std::cout << " the owner brought home a thing at a price: " << thing << std::endl;
            std::copy(house.begin(), house.end(), std::ostream_iterator<int>(std::cout, " "));
            pcout << "\n";
            m_mutex.unlock();
        }
        /*else    // этот блок пока не работает
        {
            // езли не получилось открыть дом, то вызывает полицию
            police = true;
            pcout << " the owner called the police";
            return;
        }*/

        // уходит опять за покупками
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    
}


// вор каждый раз забирает из дома самую дорогую вещь
void thief(std::vector<int>& hous)
{
    while (!hous.empty()) // пока дом не пуст
    {
        // эта проверка пока не работает
        /*if (police)
        {
            // если вызвали полицию, то вор убегает
            pcout << "the thief disappeared ";
            return;
        }*/

        {
            // вор закрывается в доме
            std::lock_guard<std::mutex> lock(m_mutex); 

            //  вор ищет самую дорогую вещь и забирает её
            hous.erase( std::max_element( hous.begin(), hous.end() ) ); 
        }

        pcout << "a thief stole a thing";

        // время пока вор вернётся
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
}

int main()
{
    size_t index{ 100 };
    auto number_prime(std::async(std::launch::async, get_prime_numb, index));
    //std::thread stream1([&number_prime, index](){ number_prime = get_prime_numb(index); }); // первый вариант


    //stream1.join();  // а вот сдесь нам уже нужен результат 
    std::cout << number_prime.get() << " - " << index << "'th prime number." << std::endl;

    std::vector<int> things{12, 20, 1200, 550, 35, 630, 2000, 170};

    std::thread restocking( owner, ref(things) );
    std::thread robbery( thief, ref(things) );
 
    restocking.join();
    robbery.join();
    
    if (things.empty())
        std::cout << " hous is empty!\n";
    else // это нужно было для случая, когда вор пойман
        std::copy(things.begin(), things.end(), std::ostream_iterator<int>{std::cout, " "});
     
    
}