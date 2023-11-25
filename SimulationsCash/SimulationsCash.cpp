// Cache_memory.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // для использования функций для замера времени
#include "function's.h"
//#include "Timer.h"

using namespace std;




int main()
{
	setlocale(LC_ALL, "rus");

	// файл основной памяти
	const string main_memory_file = "MainMemory.txt"; 
	// запасной файл для функции перезаписи в основную память
	const string tmp_file = "tmp.txt"; 
	const string cash_file = "cash.txt";
	// таймер
	chrono::time_point<time1> t; 
	
	
	// кол-во страниц
	const int pages = 10; 
	// кол-во строк (блоков)
	const int rows = 10;  
	// кол-во элементов в строке
	const int cols = 4;   

	// массив, содержащий память тегов
	short int* teg = new short int[rows];     
	// память тегов хранит в себе индексы страниц, строки которых сейчас в кэше
	for (int i = 0; i < rows; i++)
		teg[i] = 0;

	// матрица, в которой хранится содержимое кэша
	short int** cache = new short int* [rows];
	for (int i = 0; i < rows; i++)
		cache[i] = new short int[cols];

	// в начале кэш пустой
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cache[i][j] = 0;

	ifstream in;	// файловый поток для записи
	ofstream out;	// файловый поток для чтения

	// заполняем по умолчанию основную память случайными значениями
	out.open(main_memory_file);
	fill_main_memory_random(out, pages, rows, cols);
	out.close();


	// определяем команды (функции, которые выполняет программа) todo
	cout << "Список команд:\n";
	cout << "1) Поиск элемента по адресу\n";
	cout << "2) Поиск элемента по значению \n";
	cout << "3) Изменение значения по адресу\n";
	cout << "4) Выход из программы\n" << endl;

	short int num; // дополнительная переменная для прохода по файлу

	// страница, строка, столбец
	int page, row, col;


	int command = 0; // номер команды
	bool found = false;

	//  запуск цикла
	while (true)
	{
		cout << "Введите номер команды: ";
		cin >> command;
		try
		{
			switch (command)
			{
				// поиск и вывод элемента по заданному адресу
			case 1:

				cout << "Введите адрес страницы от 1-10: "; 
				cin >> page;
				cout << "Введите адрес строки от 1-10: ";
				cin >> row;
				cout << "Введите адрес столбца от 1-4: ";
				cin >> col;
				cout << endl;
				// запускаем (обнуляем таймер)
				t = time1::now();

				// если в кэше записана строка с необходимым элементом
				// т.е. если в памяти тегов указана нужная страница для данной строки
				if (teg[row - 1] == page)
				{
					found_in_caсhe(t, cache[row - 1][col - 1], row);
					print_array(cache[row - 1], cols);	// вывод строки row из кэша
					print_array_in_file(cache[row - 1], cols, out); // вывод строки кэша
					cout << endl;
				}
				// если же элемента в кэше нет
				else
				{
					out.open(tmp_file, ios::app);
					// перед удалением строки кэша нужно обновить соответствующее содержимое в основной памяти,
					// если в строке кэша на данный момент есть данные
					if (teg[row - 1] != 0)
						// перезаписываем данные в основную память
						rewrite(main_memory_file, tmp_file, cache[row - 1], rows, cols, pages, row - 1, teg[row - 1]);

					// ищем необходимый элемент в основной памяти, раз его нет в кэше
					in.open(main_memory_file);
					out.open(cash_file, ios::app);
					

					// сместитья в начало нужной страницы
					for (int i = 0; i < page * rows - rows; i++)
						in >> num >> num >> num >> num;

					// сместиться в начало нужной строки и записать данные в кэш
					for (int i = 0; i < row; i++)
						in >> cache[row - 1][0] >> cache[row - 1][1] >> cache[row - 1][2] >> cache[row - 1][3];

					// обновить память тегов (заменить индекс страницы, соответствующий новой информации в кэше)
					teg[row - 1] = page;

					// вывод информации пользователю
					found_in_main_memory(t, cache[row - 1][col - 1], page, row);
					found_in_main_memory_in_file(t, cache[row - 1][col - 1], page, row, out);
					print_array(cache[row - 1], cols); // вывод строки кэша
					print_array_in_file(cache[row - 1], cols,out); // вывод строки кэша
					cout << endl;
					

					in.close();
					out.close();
				}

				// вывод содержимого кеша
				cout << endl;
				cout << "Содержимое кэша" << endl;
				print_matrix(cache, rows, cols);
				cout << endl;
				//break;

				// вывод содержимого памяти тегов
				cout << "Содержимое памяти тегов \n";
				print_array(teg, rows);
				cout << endl << endl;
				break;

				
			case 2:
			
				int search_value;
				cout << "Введите значение для поиска в кэше: ";
				cin >> search_value;
				
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < cols; j++) {
						if (cache[i][j] == search_value) {
							cout << "Элемент найден в кэше: страница " << teg[i] << ", строка " << (i + 1) << ", столбец " << (j + 1) << endl;
							found = true;
						}
					}
				}

		
				if (!found) {
					in.open(main_memory_file);
					bool found_in_memory = false;
					while (in >> num) {
						if (num == search_value) {
							found_in_memory = true;
							break;
						}
					}
					in.close();

					if (found_in_memory) {
						cout << "Элемент найден в основной памяти, но отсутствует в кэше." << endl;
					}
					else {
						cout << "Элемент не найден в кэше и основной памяти." << endl;
					}
					break;
				}
			

			case 10:
				// вывод содержимого кеша в файл
				out.open(cash_file);
				print_matrix_in_file(cache, rows, cols, out);
				out.close();
				
				// вывод содержимого кеша
				cout << endl;
				cout << "Содержимое кэша" << endl;
				print_matrix(cache, rows, cols);
				cout << endl;
				//break;

				// вывод содержимого памяти тегов
				cout << "Содержимое памяти тегов \n";
				print_array(teg, rows);
				cout << endl << endl;
				break;
				
				cout << endl;
				break;


			case 3:

				int page, row, col;
				int new_info;
				cout << "Введите адрес страницы от 1-10: "; 
				cin >> page;
				cout << "Введите адрес строки от 1-10: ";
				cin >> row;
				cout << "Введите адрес столбца от 1-4: ";
				cin >> col;
				cout << "Введите новое значение (4-значное натуральное число): "; 
				cin >> new_info;

				// запуск таймера
				t = time1::now();

				// если элемент имеется в кэше
				if (teg[row - 1] == page)
				{
					cache[row - 1][col - 1] = new_info;

					cout << "Затраченное время: " << chrono::duration_cast<seconds1>(time1::now() - t).count() << " секунд\n";
					print_array(cache[row - 1], cols); // вывод обновлённой строки кэша
					cout << "\n";
				}
				// если элемента в кэше нет
				else
				{
					// перед удалением строки кэша нужно обновить соответствующее содержимое в основной памяти,
					// если в строке кэша на данный момент есть данные
					if (teg[row - 1] != 0)
						// перезаписываем данные в основную память
						rewrite(main_memory_file, tmp_file, cache[row - 1], rows, cols, pages, row - 1, teg[row - 1]);

					// ищем необходимый элемент в основной памяти, раз его нет в кэше
					in.open(main_memory_file);

					// сместитья в начало нужной страницы
					for (int i = 0; i < page * rows - rows; i++)
						in >> num >> num >> num >> num;

					// сместиться в начало нужной строки и записать данные в кэш
					for (int i = 0; i < row; i++)
						in >> cache[row - 1][0] >> cache[row - 1][1] >> cache[row - 1][2] >> cache[row - 1][3];

					cout << "Затраченное время: " << chrono::duration_cast<seconds1>(time1::now() - t).count() << " секунд\n";

					// обновить память тегов (заменить индекс страницы, соответствующий новой информации в кэше)
					teg[row - 1] = page;

					// обновить элемент в кэше
					cache[row - 1][col - 1] = new_info;

					print_array(cache[row - 1], cols); // вывод строки кэша
					cout << "\n";

					in.close();
				}
				// вывод содержимого кеша
				cout << endl;
				cout << "Содержимое кэша" << endl;
				print_matrix(cache, rows, cols);
				cout << endl;
				//break;

				// вывод содержимого памяти тегов
				cout << "Содержимое памяти тегов \n";
				print_array(teg, rows);
				cout << endl << endl;
				break;

				cout << endl;
				break;



				cout << endl;
				break;

				// Копирование файла 
			/*case 3:
				cout << "Копия файла сделана \n";
				copyFile(main_memory_file, tmp_file);
				break;*/

				// завершение программы 
			case 4:
				cout << "Завершение работы программы" << endl;

				// очищаем память из-под матрицы кэша и массива памяти тегов
				for (int i = 0; i < rows; i++)
					delete[] cache[i];
				delete[] cache;

				delete[] teg;

				return 0; // завершение программы
				break;

			}
		}
		catch (invalid_argument error)
		{
			cout << "Ошибка. Завершение программы" << endl;
			for (int i = 0; i < rows; i++)
				delete[] cache[i];
			delete[] cache;

			delete[] teg;
			break;
		}
	}
}


