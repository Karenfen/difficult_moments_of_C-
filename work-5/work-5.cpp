#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include <algorithm>



// шаблонная функция для первого задания
template<typename ITER>
void uniqueWordList(ITER it, ITER end)
{
  // создаём временную мапу, отсортированную по алфавиту, где ключ - это слово, а значение - колличество повторений.
  std::map<std::string, int> temp_map;
  for (; it != end ; ++it)
  {
      ++temp_map[*it];  // создаём элемент, если такого ключа нет и увеличиваем значение 
  }

  std::vector<std::pair<std::string, int>> temp_vec;


  for (const auto& [word, counter] : temp_map)
  {
      temp_vec.push_back(std::make_pair(word, counter));
      std::cout << std::setw(10) << word << "\t" << counter << std::endl; // выводим строки по алфавиту и счетчик их появления в тексте
  }

  std::sort(temp_vec.begin(), temp_vec.end(), 
    [](const auto& left, const auto& right){
      return (left.second > right.second);
    });

    std::cout << " sort by counter\n";

    for (const auto& [word, counter] : temp_vec)
    {
      std::cout << std::setw(10) << word << "\t" << counter << std::endl;
    }
    
}

// Функция для второго задания
void wordSentenceProcessing(std::string& text)
{
    std::priority_queue<std::pair<int, std::string>> temp_qu;  // создаём очередь из предложений с приоритетом по их длине
    auto start_sentence {text.begin()};  // итератор начала предложения
    auto end_sentence {text.end()};   // итератор на конец предложения

    while (start_sentence != text.end())
    {   // находим итератор конца предложения
      end_sentence = std::next(std::find_if(start_sentence, text.end(), [](char& symbol){ return ((symbol == '.') || (symbol == '?') || (symbol =='!'));}));

      std::string sentence { start_sentence, end_sentence };  // создаём временную строку по итераторам на начало и конец найденного предложения
      temp_qu.push(std::make_pair(static_cast<int>(sentence.length()), sentence));  // добавляем в очередь
      
      start_sentence = end_sentence;  // перемещаем итератор начала предложения на конец предыдущего
      if (start_sentence != text.end()) // если это не конец строки, пропускаем пробел
        ++start_sentence; 
    }
    
    while (!temp_qu.empty())
    {
      std::cout << temp_qu.top().first << "\t" << temp_qu.top().second << std::endl;  // выводим строки по их длине
      temp_qu.pop();  // удаляем напечатанную строку из очереди
    }
}



int main()
{
    //  Проверка задания №1
    std::cout << "\t... TASK - 1 ...\n";

  {
    std::list<std::string> words_vec {"word", "hello", "world", "hello", "world", "world", "intel", "book", "book"};
    std::list<std::string> words_lst {"children", "prince", "country", "country", "children", "children", "father", "father", "father"};
    
    std::cout << " Words in vector:\n";
    uniqueWordList(words_vec.begin(), words_vec.end());

    std::cout << " Words in list:\n";
    uniqueWordList(words_lst.begin(), words_lst.end());
  }
    
    //  Проверка задания №2
    std::cout << "\t... TASK - 2 ...\n";

  {
    std::string text;
    std::cout << " Enter a text:\n";
    std::getline(std::cin, text);
    
    wordSentenceProcessing(text);
  }
}