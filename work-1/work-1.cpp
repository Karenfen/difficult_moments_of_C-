#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include <tuple>
#include <variant>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Person
{
    string surname;
    string name;
    optional<string> patronymic;
};

// Перегрузка оператора << для Persone
ostream& operator<< (ostream &out, const Person &person)
{
    out  << setw(15) << person.surname << setw(15) << person.name << setw(20);
    if (person.patronymic.has_value())
        out << person.patronymic.value();
    else 
        out << " ";
    return out;
}

// Перегрузка оператора < для Persone
bool operator< (const Person &left, const Person &right)
{
    return tie(left.surname, left.name, left.patronymic) < tie(right.surname, right.name, right.patronymic);
}

// Перегрузка оператора == для Persone
bool operator== (const Person &left, const Person &right)
{
    return tie(left.surname, left.name, left.patronymic) == tie(right.surname, right.name, right.patronymic);
}

struct  PhoneNumber
{
    uint16_t country_code;
    uint16_t city_code;
    string phone_number;
    optional<int> extension_number;
};

// Перегрузка оператора << для PhoneNumber
ostream& operator<< (ostream &out, const PhoneNumber &number)
{
    out << "+" << number.country_code << "(" << number.city_code << ")" << number.phone_number << " ";
    if (number.extension_number.has_value())
        out << number.extension_number.value();
    return out;
}

class PhoneBook
{
private:
    vector<pair<Person, PhoneNumber>> contacts;
public:
    PhoneBook(ifstream &file_book);
    ~PhoneBook();
   friend ostream& operator<< (ostream &out, const PhoneBook &phone_book);
    void SortByName();
    void SortByPhone();
    tuple<string, PhoneNumber> GetPhoneNumber(const string &find_surename);
    void ChangePhoneNumber(const Person& find_person, const PhoneNumber& new_number);
};
    
PhoneBook::PhoneBook(ifstream &file_book)
{
    while (!file_book.eof())
    {
        Person new_person;
        PhoneNumber new_number;
        string temp_patronymic;
        string temp_extension_number;

        file_book >> new_person.surname >> new_person.name >> temp_patronymic 
            >> new_number.country_code >> new_number.city_code >> new_number.phone_number >> temp_extension_number;

        if (temp_patronymic == "-")
        {
            new_person.patronymic = nullopt;
        }
        else
        {
            new_person.patronymic = temp_patronymic;
        }

        if (temp_extension_number == "-")
        {
            new_number.extension_number = nullopt;
        }
        else
        {
            new_number.extension_number = stoi(temp_extension_number);
        }

        contacts.push_back(make_pair(new_person, new_number));
    }
    file_book.close();
}

PhoneBook::~PhoneBook()
{
    contacts.clear();
}

void PhoneBook::SortByName()
{
    sort(contacts.begin(), contacts.end(),
        [](pair<Person, PhoneNumber> contact1, pair<Person, PhoneNumber>contact2)
        { return contact1.first < contact2.first; } );
}

void PhoneBook::SortByPhone()
{
    sort(contacts.begin(), contacts.end(),
        [](pair<Person, PhoneNumber> contact1, pair<Person, PhoneNumber>contact2)
        { return tie(contact1.second.country_code, contact1.second.city_code, contact1.second.phone_number, contact1.second.extension_number) 
            < tie(contact2.second.country_code, contact2.second.city_code, contact2.second.phone_number, contact2.second.extension_number); } );
}

tuple<string, PhoneNumber> PhoneBook::GetPhoneNumber(const string &target_surename)
{
    uint16_t counter = 0;
    PhoneNumber answer_namber;
    string masseg;

    for_each(contacts.begin(), contacts.end(),
        [&counter, target_surename, &answer_namber](const auto& contact)
        {
            if (contact.first.surname == target_surename)
            {
                answer_namber = contact.second;
                counter++;
            }
        });

    if (counter == 0)
    {
        masseg = "not found";
    }
    if (counter > 1 )
    {
        masseg = "found more than 1";
    }
    return make_tuple(masseg, answer_namber);
}

void PhoneBook::ChangePhoneNumber(const Person& target_person, const PhoneNumber& new_number)
{
    vector<pair<Person, PhoneNumber>>::iterator it = find_if(contacts.begin(), contacts.end(), [&](const auto& contact){
        return (contact.first == target_person);
    });
    if (it != contacts.end())
        (*it).second = new_number;
}

// Перегрузка оператора << для PhoneBook
ostream& operator<< (ostream &out, const PhoneBook &phone_book)
{
    for (const auto& [person, number] : phone_book.contacts)
    {
        out << person << "\t" << number << endl;
    }
    return out;
}

int main() 
{
    ifstream file ("PhoneBook.txt"); // путь к файлу PhoneBook.txt
    try
    {
        if (!file.is_open())
            throw "file \"PhoneBook.txt\" not found";
    }
    catch(const char* exception)
    {
        cerr << "ERROR: " << exception << endl;
        return 1;
    }
    
    PhoneBook book(file);
    cout << book;
   
    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
    /*
    * лямбда функция, которая принимает фамилию и выводит номер телефона
    * этого человека, либо строку с ошибкой
    */
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Petrov");

    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
                           PhoneNumber{7, 123, "15344458", nullopt});
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
                           PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;
}