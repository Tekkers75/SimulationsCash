#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

// ���������� ����� ������������ ��� �������� ������� � ������ ��� ������ �������
using time1 = chrono::steady_clock;		  // ����, ������
using seconds1 = chrono::duration<double>; // �������� �������, ���������� � ��������

const string tmp_file = "tmp.txt";
const string cash_file = "cash.txt";
const string main_memory_file = "MainMemory.txt";


// ���������� �����, ��������������� ����� �������� ������, ���������� ����������
void fill_main_memory_random(ofstream& out, const int pages, const int rows, const int cols)
{
	srand(time(nullptr));
	cout << "���������� ����� �������� ������ ���������� ����������: \n";
	if (out.is_open())
	{
		for (int i = 0; i < pages; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				for (int k = 0; k < cols; k++)
					out << rand() % 9000 + 1000 << " "; // ����� �� 1000 �� 9999
				out << "\n";
			}
			out << "\n";
		}
			cout << "������\n";
	}
	else  
		cout << "�� ������� ������� ����." << endl;
}

//bool copyFile(const string& sourceFile, const string& destinationFile)
//{
//	ifstream inputFile(sourceFile, ios::binary); // �������� �������� ����� ��� ������
//	ofstream outputFile(destinationFile, ios::binary); // �������� ��������� ����� ��� ������
//
//	if (inputFile && outputFile)
//	{
//		// ����������� ����������� �� �������� ����� � �������� ����
//		outputFile << inputFile.rdbuf();
//		inputFile.close(); // �������� �������� �����
//		outputFile.close(); // �������� ��������� �����
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

// 1) ������ �������� �� ����
template <typename T>
void found_in_ca�he(chrono::time_point<time1>& t_count, T num, int row)
{
	cout << "����������� �����: " << chrono::duration_cast<seconds1>(time1::now() - t_count).count() << " ������\n";
	cout << "������� �������� �� ����\n";
	cout << "��������, ���������� �� ��������� ������: " << num << "\n";
	cout << "������ " << row << " ����:\n";
}


// 2) ������ �������� �� �������� ������
// ����� �������� � �������� ������, � �������� row ������������ ������ ������ � ���������
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

// 3) ������ �������� �� �������� ������
template <typename T>
void found_in_main_memory(chrono::time_point<time1>& t_count, T num, int page, int row)
{
	cout << "����������� �����: " << chrono::duration_cast<seconds1>(time1::now() - t_count).count() << " ������\n";
	cout << "������� �������� �� �������� ������\n";
	cout << "��������, ���������� �� ��������� ������: " << num << "\n";
	cout << "�������� " << page << ", c����� " << row << " �������� ������:\n";
}

template <typename T>
void found_in_main_memory_in_file(chrono::time_point<time1>& t_count, T num, int page, int row, ofstream& out)
{
	out << "����������� �����: " << chrono::duration_cast<seconds1>(time1::now() - t_count).count() << " ������\n";
	out << "������� �������� �� �������� ������\n";
	out << "��������, ���������� �� ��������� ������: " << num << "\n";
	out << "�������� " << page << ", c����� " << row << " �������� ������:\n";
}


// ������� ������ �������
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

// ������� ������ �������
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

// ��������� ����������� ���� �� �������� �������� ������
// (�.�. ��������� �������� ������ ������ ����� ������� ����� ����� �� ����)


// ���������� �������� ������
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