#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

// Псевдонимы типов используются для удобного доступа к данным при замере времени
using time1 = chrono::steady_clock;		  // часы, таймер
using seconds1 = chrono::duration<double>; // интервал времени, измеряемый в секундах

const string tmp_file = "tmp.txt";
const string cash_file = "cash.txt";
const string main_memory_file = "MainMemory.txt";


// заполнение файла, представляющего собой основную память, случайными значениями
void fill_main_memory_random(ofstream& out, const int pages, const int rows, const int cols)
{
	srand(time(nullptr));
	cout << "Заполнение файла основной памяти случайными значениями: \n";
	if (out.is_open())
	{
		for (int i = 0; i < pages; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				for (int k = 0; k < cols; k++)
					out << rand() % 9000 + 1000 << " "; // числа от 1000 до 9999
				out << "\n";
			}
			out << "\n";
		}
			cout << "Готово\n";
	}
	else  
		cout << "Не удалось открыть файл." << endl;
}

//bool copyFile(const string& sourceFile, const string& destinationFile)
//{
//	ifstream inputFile(sourceFile, ios::binary); // Открытие входного файла для чтения
//	ofstream outputFile(destinationFile, ios::binary); // Открытие выходного файла для записи
//
//	if (inputFile && outputFile)
//	{
//		// Копирование содержимого из входного файла в выходной файл
//		outputFile << inputFile.rdbuf();
//		inputFile.close(); // Закрытие входного файла
//		outputFile.close(); // Закрытие выходного файла
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

// 1) случай загрузки из кэша
template <typename T>
void found_in_caсhe(chrono::time_point<time1>& t_count, T num, int row)
{
	cout << "Затраченное время: " << chrono::duration_cast<seconds1>(time1::now() - t_count).count() << " секунд\n";
	cout << "Элемент загружен из кэша\n";
	cout << "Значение, хранящееся по введённому адресу: " << num << "\n";
	cout << "Строка " << row << " кэша:\n";
}


// 2) случай загрузки из основной памяти
// поиск элемента в основной памяти, в параметр row записывается индекс строки с элементом
template <typename T>
bool search_main_memory(T** arr, const int rows, const int cols, T num, int& row)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (arr[i][j] == num)
			{
				row = i;
				return true;
			}
	return false;
}

// 3) случай загрузки из основной памяти
template <typename T>
void found_in_main_memory(chrono::time_point<time1>& t_count, T num, int page, int row)
{
	cout << "Затраченное время: " << chrono::duration_cast<seconds1>(time1::now() - t_count).count() << " секунд\n";
	cout << "Элемент загружен из основной памяти\n";
	cout << "Значение, хранящееся по введённому адресу: " << num << "\n";
	cout << "Страница " << page << ", cтрока " << row << " основной памяти:\n";
}

template <typename T>
void found_in_main_memory_in_file(chrono::time_point<time1>& t_count, T num, int page, int row, ofstream& out)
{
	out << "Затраченное время: " << chrono::duration_cast<seconds1>(time1::now() - t_count).count() << " секунд\n";
	out << "Элемент загружен из основной памяти\n";
	out << "Значение, хранящееся по введённому адресу: " << num << "\n";
	out << "Страница " << page << ", cтрока " << row << " основной памяти:\n";
}


// функция вывода матрицы
template <typename T>
void print_matrix(T* arr, const int rows, const int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			cout << arr[i][j] << "\t";
		cout << "\n";
	}
}

template <typename T>
void print_matrix_in_file(T* arr, const int rows, const int cols, ofstream& out)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			out << arr[i][j] << "\t";
		out << "\n";
	}
}

// функция вывода массива
template <typename T>
void print_array(T* arr, const int rows)
{
	for (int i = 0; i < rows; i++)
		cout << arr[i] << " ";
}

template <typename T>
void print_array_in_file(T* arr, const int rows, ofstream& out)
{
	for (int i = 0; i < rows; i++)
		out << arr[i] << " ";
}

// изменение содержимого кэша по принципу обратной записи
// (т.е. обновлять основную память только когда элемент будет удалён из кэша)


// обновление основной памяти
void rewrite(const string& file_main_memory, const string& dop_file, short int* arr, int rows, int cols, int pages, int row, int page)
{
	ifstream in(file_main_memory);
	ofstream out(dop_file);

	int num;
	for (int i = 1; i <= pages; i++)
	{
		for (int r = 0; r < rows; r++)
		{
			if (page == i && r == row)
			{
				in >> num >> num >> num >> num;
				out << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << "\n";
			}
			else
			{
				for (int j = 0; j < cols; j++)
				{
					in >> num;
					out << num << " ";
				}
				out << "\n";
			}
		}
		out << "\n";
	}

	in.close();
	out.close();

	remove(main_memory_file.c_str());
	rename(tmp_file.c_str(), main_memory_file.c_str());
}