#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

template <typename T>
void Swop(T& first, T& second)
{
    cout << "start swop" << endl;
    T temp = first;
    first = second;
    second = temp;
    cout << "end swop\n";
}

template <typename T>
void SortPointers(vector<T> &vec)
{
    sort(vec.begin(), vec.end(), [](T first, T second)
        { return ((*first) < (*second)); } );
}

class Timer
{
private:
    using clock_t = chrono::high_resolution_clock;
    using second_t = chrono::duration<double, ratio<1> >;
    string m_name;
    chrono::time_point<clock_t> m_beg;
    double elapsed() const
    {
    return std::chrono::duration_cast<second_t>(clock_t::now()
    -m_beg).count();
    }
public:
    Timer() : m_beg(clock_t::now()) { }
    Timer(string name) : m_name(name), m_beg(clock_t::now()) { }
    void start(string name) {
        m_name = name;
        m_beg = clock_t::now();
    }
    void print() const {
    cout << m_name << ":\t" << elapsed() * 1000 << " ms" << '\n';
    }
};

uint64_t count_if_and_find (const string& book, const string& simbols)
{
    Timer timer ("count_if & find");
    uint64_t counter {0};

    counter = count_if(book.begin(), book.end(), [&simbols](const char curent)
                                                            { return simbols.find(curent) != simbols.npos; });
    timer.print();
    return counter;
}

uint64_t count_if_and_for (const string& book, const string& simbols)
{
    Timer timer ("count_if & for");
    uint64_t counter {0};

    counter = count_if(book.begin(), book.end(), [&simbols](const char curent)
                                                            { for (const char& target : simbols)
                                                                {
                                                                    if ( curent == target)
                                                                        return true;
                                                                } return false; });
    timer.print();
    return counter;
}

uint64_t for_and_find (const string& book, const string& simbols)
{
    Timer timer ("for & find");
    uint64_t counter {0};

    for (const char& curent : book)
    {
        if ( simbols.find(curent) != simbols.npos)
            ++counter;
    }

    timer.print();
    return counter;
}

uint64_t for_and_for (const string& book, const string& simbols)
{
    uint64_t counter {0};
    Timer timer ("for & for");

    for (const char& curent : book)
    {
        for (const char& target : simbols)
        {
            if ( curent == target)
            {
                ++counter;
                break;
            }
        }
    }
    timer.print();
    return counter;
}

int main()
{
    // Check Task-1
    cout << "\n======== Check Task-1 =========\n" << endl;
{
    const int a {5};
    const int b {10};
    const int* ptr_a = &a;
    const int* ptr_b = &b;

    cout << "a = " << a << "\t" << "b = " << b << endl;
    cout << "ptr a: " << ptr_a << "\t" << "ptr b: " << ptr_b << endl;
    cout << "*ptr a: " << *ptr_a << "\t" << "*ptr b: " << *ptr_b << endl;

    Swop(ptr_a, ptr_b);

    cout << "a = " << a << "\t" << "b = " << b << endl;
    cout << "ptr a: " << ptr_a << "\t" << "ptr b: " << ptr_b << endl;
    cout << "*ptr a: " << *ptr_a << "\t" << "*ptr b: " << *ptr_b << endl;
}
    // Check Task-2
    cout << "\n======== Check Task-2 =========\n" << endl;
{
    vector<int> vec_1 {5, 8, 1, 3, 2, 0, 4};
    vector<int*> vec_ptr;

    cout << "vec_1: ";
    for (const auto& value : vec_1)
    {
        cout << value << " ";
    }
    cout << "\n";
    
    for (auto& value : vec_1)
    {
        vec_ptr.push_back(&value);
    }

    cout << "*vec_ptr: ";
    for (const auto& value : vec_ptr)
    {
        cout << *value << " ";
    }
    cout << "\n";

    cout << "start sort\n";
    SortPointers(vec_ptr);
    cout << "end sort\n";

    cout << "*vec_ptr: ";
    for (const auto& value : vec_ptr)
    {
        cout << *value << " ";
    }
    cout << "\n";
    cout << "vec_1: ";
    for (const auto& value : vec_1)
    {
        cout << value << " ";
    }
    cout << "\n";
}
    // Check Task-3
    cout << "\n======== Check Task-3 =========\n" << endl;
{
    uint64_t counter {0};
    string vowels {"aeiouyAEIOUY"};

    ifstream file("War_and_Peace.txt");
    try
    {
        if(!file.is_open())
            throw "the file is not opened";
    }
    catch(const char* exception)
    {
        cerr <<  "ERROR: " << exception << '\n';
        return 1;
    }
    
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    string book(size, ' ');
    file.read(&book[0], size);

    // searching by using 'count_if' and 'find'
    counter = count_if_and_find(book, vowels);
    cout << "vowels found: " << counter << endl;

    // searching by using 'count_if' and 'for'
    counter  = count_if_and_for(book, vowels);
    cout << "vowels found: " << counter << endl;
    
    // searching by using 'for' and 'find'
    counter  = for_and_find(book, vowels);
    cout << "vowels found: " << counter << endl;

    // searching by using 'for' and 'for'
    counter = for_and_for(book, vowels);
    cout << "vowels found: " << counter << endl;
}
    return 0;
}