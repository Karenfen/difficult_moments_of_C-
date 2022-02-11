#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include <tuple>
#include <variant>
#include <algorithm>
#include <iomanip>

// for tests
#include "gtest/gtest.h"
#include "timer.h"





struct Person
{
    std::string surname;
    std::string name;
    std::optional<std::string> patronymic;
};

// Перегрузка оператора << для Persone
std::ostream& operator<< (std::ostream& out, const Person& person)
{
    out << std::setw(15) << person.surname << std::setw(15) << person.name << std::setw(20);
    if (person.patronymic.has_value())
        out << person.patronymic.value();
    else
        out << " ";
    return out;
}

// Перегрузка оператора < для Persone
bool operator< (const Person& left, const Person& right)
{
    return tie(left.surname, left.name, left.patronymic) < tie(right.surname, right.name, right.patronymic);
}

// Перегрузка оператора == для Persone
bool operator== (const Person& left, const Person& right)
{
    return tie(left.surname, left.name, left.patronymic) == tie(right.surname, right.name, right.patronymic);
}



struct  PhoneNumber
{
    uint16_t country_code;
    uint16_t city_code;
    std::string phone_number;
    std::optional<int> extension_number;
};

// Перегрузка оператора << для PhoneNumber
std::ostream& operator<< (std::ostream& out, const PhoneNumber& number)
{
    out << "+" << number.country_code << "(" << number.city_code << ")" << number.phone_number << " ";
    if (number.extension_number.has_value())
        out << number.extension_number.value();
    return out;
}

// Перегрузка оператора == для PhoneNumber
bool operator== (const PhoneNumber& left, const PhoneNumber& right)
{
    return tie(left.country_code, left.city_code, left.phone_number, left.extension_number) == tie(right.country_code, right.city_code, right.phone_number, right.extension_number);
}



class PhoneBook
{
private:
    std::vector<std::pair<Person, PhoneNumber>> contacts;
public:
    PhoneBook(std::ifstream& file_book);
    ~PhoneBook();
    friend std::ostream& operator<< (std::ostream& out, const PhoneBook& phone_book);
    void SortByName();
    void SortByPhone();
    std::tuple<std::string, PhoneNumber> GetPhoneNumber(const std::string& target_surename);
    void ChangePhoneNumber(const Person& target_person, const PhoneNumber& new_number);
};

PhoneBook::PhoneBook(std::ifstream& file_book)
{
    while (!file_book.eof())
    {
        Person new_person;
        PhoneNumber new_number;
        std::string temp_patronymic;
        std::string temp_extension_number;

        file_book >> new_person.surname >> new_person.name >> temp_patronymic
            >> new_number.country_code >> new_number.city_code >> new_number.phone_number >> temp_extension_number;

        if (temp_patronymic == "-")
        {
            new_person.patronymic = std::nullopt;
        }
        else
        {
            new_person.patronymic = temp_patronymic;
        }

        if (temp_extension_number == "-")
        {
            new_number.extension_number = std::nullopt;
        }
        else
        {
            new_number.extension_number = std::stoi(temp_extension_number);
        }

        contacts.push_back(std::make_pair(new_person, new_number));
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
        [](std::pair<Person, PhoneNumber> contact1, std::pair<Person, PhoneNumber>contact2)
        { return contact1.first < contact2.first; });
}

void PhoneBook::SortByPhone()
{
    sort(contacts.begin(), contacts.end(),
        [](std::pair<Person, PhoneNumber> contact1, std::pair<Person, PhoneNumber>contact2)
        { return tie(contact1.second.country_code, contact1.second.city_code, contact1.second.phone_number, contact1.second.extension_number)
        < tie(contact2.second.country_code, contact2.second.city_code, contact2.second.phone_number, contact2.second.extension_number); });
}

std::tuple<std::string, PhoneNumber> PhoneBook::GetPhoneNumber(const std::string& target_surename)
{
    uint16_t counter = 0;
    PhoneNumber answer_namber;
    std::string masseg;

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
    if (counter > 1)
    {
        masseg = "found more than 1";
    }
    return make_tuple(masseg, answer_namber);
}

void PhoneBook::ChangePhoneNumber(const Person& target_person, const PhoneNumber& new_number)
{
    std::vector<std::pair<Person, PhoneNumber>>::iterator it = find_if(contacts.begin(), contacts.end(), [&](const auto& contact) {
        return (contact.first == target_person);
        });
    if (it != contacts.end())
        (*it).second = new_number;
}

// Перегрузка оператора << для PhoneBook
std::ostream& operator<< (std::ostream& out, const PhoneBook& phone_book)
{
    for (const auto& [person, number] : phone_book.contacts)
    {
        out << person << "\t" << number << std::endl;
    }
    return out;
}




// Фиксация 
class testPhoneBook : public testing::Test
{
protected:
    void SetUp() override {
        if (!file.is_open())
            std::cerr << "file \"test.txt\" not found\n";

        book = new PhoneBook(file);
        file.close();
    }
    void TearDown() override {
        delete book;
    }
    PhoneBook* book;
    std::ifstream file{ "test.txt" };
};


// tests...
TEST(Test_Persone, operator_equal)
{
    Person pers1{ "Kiselev", "Ilia" };
    Person pers2{ "Kiselev", "Ilia" };

    ASSERT_EQ(pers1, pers2);

    pers1.patronymic = "Aleksandovich";
    ASSERT_FALSE(pers1 == pers2);

    pers2.patronymic = "Aleksandov";
    ASSERT_FALSE(pers1 == pers2);

    pers2.patronymic = "Aleksandovich";
    ASSERT_EQ(pers1, pers2);
    
    pers2.name = "Ivan";
    ASSERT_FALSE(pers1 == pers2);

    pers2.surname = "Ivanov";
    pers2.name = "Ilia";
    ASSERT_FALSE(pers1 == pers2);

    pers2.patronymic = "Ivanov";
    pers2.surname = "Kiselev";
    ASSERT_FALSE(pers1 == pers2);

    pers2.name = "Ivan";
    pers2.surname = "Ivanov";
    ASSERT_FALSE(pers1 == pers2);
}

TEST(Test_Persone, operator_less)
{
    Person pers1{ "Kiselev", "Ilia" };
    Person pers2{ "Kiselev", "Ilia", "Aleksandovich" };

    ASSERT_LT(pers1, pers2);

    pers1.patronymic = "Aleksandovich";
    ASSERT_FALSE(pers1 < pers2);

    pers2.patronymic = "Aleksandov";
    ASSERT_FALSE(pers1 < pers2);
    ASSERT_LT(pers2, pers1);

    pers2.patronymic = "Aleksandovich";
    ASSERT_FALSE(pers2 < pers1);

    pers2.name = "Ivan";
    ASSERT_LT(pers1, pers2);

    pers2.surname = "Ivanov";
    pers2.name = "Ilia";
    ASSERT_LT(pers2, pers1);

    pers2.patronymic = "Ivanovivh";
    pers2.surname = "Kiselev";
    ASSERT_LT(pers1, pers2);

    pers2.name = "Ivan";
    pers2.surname = "Ivanov";
    ASSERT_LT(pers2, pers1);
}

TEST(Test_Persone, print)
{
    Person pers1{ "Kiselev", "Ilia" };
    std::stringstream cout;
    cout << pers1;
    
    ASSERT_STREQ(cout.str().c_str() , "        Kiselev           Ilia                    ");

    cout.str("");
    pers1.patronymic = "Aleksandrovich";
    cout << pers1;

    ASSERT_STREQ(cout.str().c_str(), "        Kiselev           Ilia      Aleksandrovich");
}

TEST(Test_Persone, set_data)
{
    Person pers1{ "Kiselev", "Ilia" };

    ASSERT_EQ(pers1.name, "Ilia");
    ASSERT_EQ(pers1.surname, "Kiselev");
    ASSERT_EQ(pers1.patronymic, std::nullopt);

    pers1.name = "Ivan";
    pers1.surname = "Ivanov";
    pers1.patronymic = "Aleksandrovich";

    ASSERT_EQ(pers1.name, "Ivan");
    ASSERT_EQ(pers1.surname, "Ivanov");
    ASSERT_EQ(pers1.patronymic, "Aleksandrovich");
    ASSERT_FALSE(pers1.patronymic == std::nullopt);
}

TEST(Test_PhoneNumber, set_data)
{
    PhoneNumber number{ 7, 999, "7951479" };

    ASSERT_EQ(number.country_code, 7);
    ASSERT_EQ(number.city_code, 999);
    ASSERT_STREQ(number.phone_number.c_str(), "7951479");
    ASSERT_EQ(number.extension_number, std::nullopt);

    number.country_code = 24;
    number.city_code = 826;
    number.phone_number = "1234567";
    number.extension_number = 12;

    ASSERT_EQ(number.country_code, 24);
    ASSERT_EQ(number.city_code, 826);
    ASSERT_EQ(number.phone_number, "1234567");
    ASSERT_EQ(number.extension_number, 12);
    ASSERT_FALSE(number.extension_number == std::nullopt);
}

TEST(Test_PhoneNumber, print)
{
    std::stringstream cout;
    PhoneNumber number{ 7, 999, "7951479" };
    cout << number;

    ASSERT_STREQ(cout.str().c_str(), "+7(999)7951479 ");
    
    cout.str("");
    number.extension_number = 12;
    cout << number;

    ASSERT_STREQ(cout.str().c_str(), "+7(999)7951479 12");
}

TEST_F(testPhoneBook, exceptions)
{
    std::ifstream file{ "speed_test.txt" };

    if (!file.is_open())
        std::cerr << "file \"speed_test.txt\" not found\n";

    PhoneBook* new_book;

    ASSERT_NO_THROW(new_book = new PhoneBook(file));

    file.close();

    new_book = nullptr;
    delete new_book;


    ASSERT_NO_THROW(book->SortByName());
    ASSERT_NO_THROW(book->SortByPhone());

    ASSERT_NO_THROW(book->GetPhoneNumber(""));
    ASSERT_NO_THROW(book->GetPhoneNumber("Loev"));
    ASSERT_NO_THROW(book->GetPhoneNumber("Sokolov"));

    Person pers;
    PhoneNumber number;

    ASSERT_NO_THROW(book->ChangePhoneNumber(pers, number));

    pers.surname = "Ilin";
    pers.name = "Petr";
    ASSERT_NO_THROW(book->ChangePhoneNumber(pers, number));

    pers.name = "Ivan";
    pers.patronymic = "Artemovich";
    ASSERT_NO_THROW(book->ChangePhoneNumber(pers, number));

    pers.name = "Petr";
    number = PhoneNumber{ 0, 0, "" };
    ASSERT_NO_THROW(book->ChangePhoneNumber(pers, number));

    number = PhoneNumber{ 55, 15, "1236548", 256 };

    number.extension_number = std::nullopt;
    ASSERT_NO_THROW(book->ChangePhoneNumber(pers, number));
}

TEST_F(testPhoneBook, print)
{
    std::stringstream cout;
    std::stringstream test_out;

    Person pers1{ "Ilin", "Petr", "Artemovich" };
    PhoneNumber numb1{ 7, 17, "4559767"};
    Person pers2{ "Sokolov", " Arsenii"};
    PhoneNumber numb2{ 7, 16, "1992257", 16 };
    Person pers3{ "Sokolov", " Artur", "Mikhailovich"};
    PhoneNumber numb3{ 81, 8281, "7420182", 2 };

    test_out << pers1 << "\t" << numb1 << "\n" << pers2 << "\t" << numb2 << "\n" << pers3 << "\t" << numb3 << "\n";
    cout << *book;

    ASSERT_STREQ(cout.str().c_str(), test_out.str().c_str());
}

TEST_F(testPhoneBook, sort)
{
    std::stringstream cout;
    std::stringstream test_out;

    Person pers1{ "Ilin", "Petr", "Artemovich" };
    PhoneNumber numb1{ 7, 17, "4559767" };
    Person pers2{ "Sokolov", " Arsenii" };
    PhoneNumber numb2{ 7, 16, "1992257", 16 };
    Person pers3{ "Sokolov", " Artur", "Mikhailovich" };
    PhoneNumber numb3{ 81, 8281, "7420182", 2 };

    book->SortByPhone();

    test_out << pers2 << "\t" << numb2 << "\n" << pers1 << "\t" << numb1 << "\n" << pers3 << "\t" << numb3 << "\n";
    cout << *book;

    ASSERT_STREQ(cout.str().c_str(), test_out.str().c_str());

    book->SortByName();

    test_out.str("");
    cout.str("");
    test_out << pers1 << "\t" << numb1 << "\n" << pers2 << "\t" << numb2 << "\n" << pers3 << "\t" << numb3 << "\n";
    cout << *book;

    ASSERT_STREQ(cout.str().c_str(), test_out.str().c_str());
}

TEST_F(testPhoneBook, get_phone_number)
{
    PhoneNumber Ilin_number{ 7, 17, "4559767" };
    PhoneNumber Sokolov2_number{ 81, 8281, "7420182", 2};
    PhoneNumber empty_number;

    ASSERT_EQ(std::get<1>(book->GetPhoneNumber("Ilin")), Ilin_number);
    ASSERT_EQ(std::get<0>(book->GetPhoneNumber("Ivanov")), "not found");
    ASSERT_EQ(std::get<0>(book->GetPhoneNumber("Sokolov")), "found more than 1");
}

TEST_F(testPhoneBook, change_phone_number)
{
    PhoneNumber new_namber1{ 7, 914, "4159947", 16 };
    PhoneNumber new_namber2{ 8, 924, "8130256"};
    Person person{ "Ilin", "Petr", "Artemovich" };

    book->ChangePhoneNumber(person, new_namber1);
    ASSERT_EQ(std::get<1>(book->GetPhoneNumber(person.surname)), new_namber1);

    book->ChangePhoneNumber(person, new_namber2);
    ASSERT_EQ(std::get<1>(book->GetPhoneNumber(person.surname)), new_namber2);
}

TEST(test_PhoneBook, time)
{
    std::ifstream file{ "speed_test.txt" };

    if (!file.is_open())
        std::cerr << "file \"speed_test.txt\" not found\n";
    
    Timer timer("reed file");

    PhoneBook* book = new PhoneBook(file);

    file.close();
    //timer.print();
    ASSERT_FALSE(timer.getTime() > 1.0);
    


    timer.start("sort by name");
    book->SortByName();
    //timer.print();
    ASSERT_FALSE(timer.getTime() > 1.0);

    timer.start("sort by phone");
    book->SortByPhone();
    //timer.print();
    ASSERT_FALSE(timer.getTime() > 1.0);

    delete book;
}


int main()
{
    //std::ifstream file("PhoneBook.txt"); // путь к файлу PhoneBook.txt
    //try
    //{
    //    if (!file.is_open())
    //        throw "file \"PhoneBook.txt\" not found";
    //}
    //catch (const char* exception)
    //{
    //    std::cerr << "ERROR: " << exception << std::endl;
    //    return 1;
    //}

    //PhoneBook book(file);
    //std::cout << book;

    //std::cout << "------SortByPhone-------" << std::endl;
    //book.SortByPhone();
    //std::cout << book;

    //std::cout << "------SortByName--------" << std::endl;
    //book.SortByName();
    //std::cout << book;

    //std::cout << "-----GetPhoneNumber-----" << std::endl;
    ///*
    //* лямбда функция, которая принимает фамилию и выводит номер телефона
    //* этого человека, либо строку с ошибкой
    //*/
    //auto print_phone_number = [&book](const std::string& surname) {
    //    std::cout << surname << "\t";
    //    auto answer = book.GetPhoneNumber(surname);
    //    if (std::get<0>(answer).empty())
    //        std::cout << std::get<1>(answer);
    //    else
    //        std::cout << std::get<0>(answer);
    //    std::cout << std::endl;
    //};

    //// вызовы лямбды
    //print_phone_number("Ivanov");
    //print_phone_number("Petrov");

    //std::cout << "----ChangePhoneNumber----" << std::endl;
    //book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
    //    PhoneNumber{ 7, 123, "15344458", std::nullopt });
    //book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
    //    PhoneNumber{ 16, 465, "9155448", 13 });
    //std::cout << book;

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}