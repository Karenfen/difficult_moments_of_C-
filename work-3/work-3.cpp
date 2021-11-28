#include <iostream>
#include <list>
#include <iomanip>
#include <vector>
#include "range.h"

void addAverageToList(std::list<double>& lst)
{
	double average { 0.0 };
	for (auto node : lst)
	{
		average += node;
	}
	lst.push_back(average / static_cast<double>(lst.size()));
}

class MyMatrix
{
private:
	int m_size;
	int** m_matrix;
public:
	MyMatrix(int size)
	{
		m_size = size;
		m_matrix = new int* [m_size];
		for (int i{ 0 }; i < m_size; ++i)
		{
			*(m_matrix + i) = new int [m_size];
		}
	}
	MyMatrix(int size, int** matrix)
	{
		m_size = size;
		m_matrix = matrix;
	}
	~MyMatrix()
	{
		for (int i{ 0 }; i < m_size; ++i)
		{
			delete [] m_matrix[i];
		}
		delete [] m_matrix;
	}
	void MatrixInit()
	{
		int value{ 0 };
		for (int row{ 0 }; row < m_size; ++row)
		{
			for (int col{ 0 }; col < m_size; ++col)
			{
				*(*(m_matrix + row) + col) = ++value;
			}
		}
	}
	void SetValue(int row, int col, int value)
	{
		if ((row < m_size && row >= 0) && (col < m_size && col >= 0))
			*(*(m_matrix + row) + col) = value;
	}
	void MatrixInit(const std::vector<int>& values)
	{
		if (m_size * m_size != values.size())
		{
			std::cout << "vector size is wrong: " << values.size() << std::endl;
			return;
		}
		int index{ 0 };
		for (int row{ 0 }; row < m_size; ++row)
		{
			for (int col{ 0 }; col < m_size; ++col)
			{
				SetValue( row, col, values[index++] );
			}
		}
	}
	int GetValue(int row, int col)
	{
		if ((row < m_size && row >= 0) && (col < m_size && col >= 0))
			return *(*(m_matrix + row) + col);
	}
	void PrintMatrix()
	{
		for (int row{ 0 }; row < m_size; ++row)
		{
			for (int col{ 0 }; col < m_size; ++col)
			{
				std::cout << std::setw(5) << GetValue(row, col);
			}
			std::cout << std::endl;
		}
	}
	int GetDeterminant()
	{
		if (m_size == 1)
			return GetValue( 0, 0 );
		if (m_size == 2)
			return ( ( GetValue( 0, 0 ) * GetValue(1, 1) ) - ( GetValue(0, 1) * GetValue(1, 0) ) );

		int result{ 0 };
		int sign{ 1 };
		std::vector<int> curent_vals;
		curent_vals.reserve(m_size - 1);
		MyMatrix subMatrix(m_size - 1);

		for (int numbMatrix{ 0 }; numbMatrix < m_size; ++numbMatrix)
		{
			for (int curent_row{ 1 }; curent_row < m_size; ++curent_row)
			{
				for (int curent_col{ 0 }; curent_col < m_size; ++curent_col)
				{
					if (curent_col == numbMatrix)
						continue;
					curent_vals.push_back( GetValue( curent_row, curent_col ) );
				}
			}
			subMatrix.MatrixInit(curent_vals);
			result += sign * GetValue(0, numbMatrix) * subMatrix.GetDeterminant();
			sign *= -1;
			curent_vals.clear();
		}
		return result;
	}
};



int main()
{
		// task 1
	std::cout << " TASK - 1 " << std::endl;
	{   
		std::list<double> myList{ 11.11, 22.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99 };

		std::cout << "my list: ";
		for (const auto& node : myList)
			std::cout << node << "\t";
		std::cout << std::endl;
		std::cout << "size my list: " << myList.size() << std::endl;

		addAverageToList(myList);

		std::cout << "average my list: " << myList.back() << std::endl;
		std::cout << "my list: ";
		for (const auto& node : myList)
			std::cout << node << "\t";
		std::cout << std::endl;

	}
		// task 2
	std::cout << " TASK - 2 " << std::endl;
	{
		MyMatrix matrix1(4);
		std::vector<int> values{ 1, -2, 0, 0, 3, -1, -1, 2, -2, 2, 3, 1, 1, 1, 2, 2};
		matrix1.MatrixInit(values);
		matrix1.PrintMatrix();
		std::cout << "determinant of the matrix = " << matrix1.GetDeterminant() << std::endl;
		

	}
		// task 3
	std::cout << " TASK - 3 " << std::endl;
	{
		std::string str{ "qwertyuiopasdfghjklzxcvbnm" };
		for (int x : range<int>(0, str.size(), 2))
			std::cout << str[x] << " ";
		std::cout << std::endl;
		for (int x : range<int>(str.size(), 0, -2))
			std::cout << str[x] << " ";
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
