#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <direct.h>
//#include <filesystem>
#include <cmath>

using namespace std;

struct tab
{
	int num[20] = { -1 ,-1 ,-1  ,-1  ,-1  ,-1  ,-1  ,-1  ,-1  ,-1 ,-1 ,-1 ,-1  ,-1  ,-1  ,-1  ,-1  ,-1  ,-1  ,-1 };
	int contain = 0;
	bool null_sentence = false;
};
tab t[100000];
string str = "",pre_str="";
bool enough;
int counts = 0;// �e�B�z�y�l�s��
int counts_o = 0; // ��l�y�l�s��
int contain_count = 0; // �s�F�X�ӥy�l
int null_sentence[100000] = { 0 }; // �ťy�l

int count_null = 0; // �p�⦳�X�Ӫťy�l
int count_null_sentence = 0; // �p�⦳�X�ӭ�l�ťy�l
int now_null_count = 0;

void normalize();
void cut_sentence();
void count_word(string);
void cut_origin_sentence();
void normalize_ori();
void save_tab_and_max_s_num();
void output_sentence();
void remove_null_sentence();

int main()
{

	normalize();
	cut_sentence();
	remove_null_sentence();
	cout << "���b�إ�table...." << endl;
	save_tab_and_max_s_num();
	output_sentence();
	/**/
	normalize_ori();
	cut_origin_sentence();
}

void normalize()
{
	ifstream fin("input.txt");
	ofstream fout("normalize.txt");
	bool must_char = false;
	string temp = "";
	while (!fin.eof())
	{
		getline(fin, str);
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] == '.' || str[i]==' ')
			{
				// �ˬd�O�_��"1. "
				bool just_num = true;
				if (temp == "")
					just_num = false;
				else
				{
					for (int k = 0; k < temp.length(); k++)
					{
						if (!isdigit(temp[k]))
						{
							just_num = false;
							break;
						}
					}
				}

				if (just_num == true && str[i] == '.')
				{
					fout << str[i];
					must_char = false;
					temp += str[i];
				}
				// �y�I�᭱���@�w�n�Ochar
				else if( must_char == false )
				{
					fout << str[i];
					must_char = true;
					temp = "";
				}
				// example: "..."
				else if (must_char == true && str[i] == '.')
				{
					fout << str[i];
					must_char = true;
					temp = "";
				}
			}
			else
			{
				fout << str[i];
				must_char = false;
				temp += str[i];
			}
		}
		fout << endl;
	}
	fin.close();
	fout.close();
}

void cut_sentence()
{
	ifstream fin("normalize.txt");
	// fin("input.txt");
	ofstream fout("sentence_temp.txt");
	string temp = "";
	while (!fin.eof())
	{
		//temp = "";
		getline(fin, str);
		for (int i = 0; i < str.length(); i++)
		{
			if (i == str.length() - 1 && str[i] != '.')
			{
				cout << "1";
				// Ū��y���ӥB���O�y�I�A�N�[�W�̫᪺�r
				if (i == str.length() - 1 && str[i] != '.')
					temp += str[i];
				// �p��ۼƬO�_����
				count_word(temp);

				// �r�ư��N��X
				if (enough == true)
				{
					fout << counts << ' ' << counts_o << ' ';
					fout << temp << endl;
					counts++; // �e�B�z�y�l�s��++
					temp = "";
				}
				// �����N�~��s�btemp
				else
				{
					temp += " ";
				}
				counts_o++; // ��l�y�l�s��++
			}
			else if (str[i] == '.' )
			{
				// �p��ۼƬO�_����
				count_word(temp);
				if (enough == true)
				{
					t[counts].num[contain_count] = counts_o;
					contain_count++;
					t[counts].contain = contain_count;
					contain_count = 0;

					//fout << counts << ' ' << counts_o << ' ';
					fout << temp << endl;
					counts++; // �e�B�z�y�l�s��++
					temp = "";
				}
				else if (temp == "")
				{
					t[counts].num[contain_count] = counts_o;
					contain_count++;
					t[counts].contain = contain_count;
					contain_count = 0;

					//fout << counts << ' ' << counts_o << ' ';
					fout << temp << endl;
					counts++; // �e�B�z�y�l�s��++
					temp = "";
				}
				else
				{
					t[counts].num[contain_count] = counts_o;
					contain_count++;

					temp += " ";
				}
				counts_o++; // ��l�y�l�s��++
			}
			else
				temp += str[i];
		}
	}
	if (temp != "")
	{
		t[counts].contain = contain_count;
		contain_count = 0;

		//fout << counts << ' ' << counts_o-1 << ' ';
		fout << temp << endl;
		counts++; // �e�B�z�y�l�s��++
		temp = "";
		//counts_o++; // ��l�y�l�s��++
	}
}

void save_tab_and_max_s_num()
{
	int now_null = 0;
	ofstream fout("s_table.txt");
	for (int i = 0; i < counts; i++)
	{
		if (t[i].null_sentence == true)
		{
			now_null++;
		}
		else
		{
			fout << (i-now_null) << ' ' << t[i].contain << ' ';
			for (int k = 0; k < t[i].contain; k++)
			{
				fout << t[i].num[k] << ' ';
			}
			fout << endl;
		}
	}
	fout.close();

	counts -= count_null; // �h���ťy�l���ƥ�
	ofstream fout2("max_s_num.txt");
	fout2 << counts << endl;
	fout2.close();
	
}

void count_word(string temp)
{
	string stemp = "";
	int scount = 0;
	for (int i = 0; i < temp.length(); i++)
	{
		// �J��r�����O�ť�
		if (temp[i] != ' ')
			stemp += temp[i];
		// �J��ť�
		else
		{
			scount++;
			stemp = "";
		}
	}
	if (scount < 7)
		enough = false;
	else
		enough = true;
}

void normalize_ori()
{
	ifstream fin("input_origin.txt");
	ofstream fout("normalize_ori.txt");
	bool must_char = false;
	string temp = "";
	while (!fin.eof())
	{
		getline(fin, str);
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] == '.' || str[i] == ' ')
			{
				// �ˬd�O�_��"1. "
				bool just_num = true;
				if (temp == "")
					just_num = false;
				else
				{
					for (int k = 0; k < temp.length(); k++)
					{
						if (!isdigit(temp[k]))
						{
							just_num = false;
							break;
						}
					}
				}

				if (just_num == true && str[i] == '.')
				{
					fout << str[i];
					must_char = false;
					temp += str[i];
				}
				// �y�I�᭱���@�w�n�Ochar
				else if (must_char == false)
				{
					fout << str[i];
					must_char = true;
					temp = "";
				}
				// example: "..."
				else if (must_char == true && str[i] == '.')
				{
					fout << str[i];
					must_char = true;
					temp = "";
				}
			}
			else
			{
				fout << str[i];
				must_char = false;
				temp += str[i];
			}
		}
		fout << endl;
	}
	fin.close();
	fout.close();
}

void remove_null_sentence()
{
	ifstream fin("sentence_temp.txt");
	ofstream fout("sentence_temp2.txt");
	string temp = "";

	for (int i = 0; i < counts; i++)
	{
		temp = "";
		getline(fin, temp);
		if (temp == "")
		{
			t[i].null_sentence = true;
			count_null++;
			count_null_sentence += t[i].contain;
			for (int k = 0; k < t[i].contain; k++)
			{
				null_sentence[now_null_count] = t[i].num[k];
				//cout << i << ' ' << t[i].num[k] << ' ';
				now_null_count++;
			}
			//cout << endl;
		}
		else
			fout << temp << endl;
	}
	
}

void cut_origin_sentence()
{
	ifstream fin("normalize_ori.txt");
	ofstream fout("sentence_origin.txt");
	string temp = "";
	str = "";
	int origin_count = 0;
	while (!fin.eof())
	{
		//temp = "";
		getline(fin, str);
		for (int i = 0; i < str.length(); i++)
		{
			if (i == str.length() - 1 && str[i] != '.')
			{
				// Ū��y���ӥB���O�y�I�A�N�[�W�̫᪺�r
				if (i == str.length() - 1 && str[i] != '.')
					temp += str[i];
				// �p��ۼƬO�_����
				//fout << origin_count << ' ';
				fout << temp << endl;
				origin_count++; // �e�B�z�y�l�s��++
				temp = "";
			}
			else if (str[i] == '.')
			{
				//fout << origin_count << ' ';
				fout << temp << endl;
				origin_count++; // �e�B�z�y�l�s��++
				temp = "";
			}
			else
				temp += str[i];
		}
	}
	if (temp != "")
	{
		//fout << origin_count << ' ';
		fout << temp << endl;
		origin_count++;
		//counts_o++; // ��l�y�l�s��++
	}
}

void output_sentence()
{
	ifstream fin("sentence_temp2.txt");
	ofstream fout("sentence.txt");
	string input_str = "";
	
	fout << counts << endl;
	while (!fin.eof())
	{
		getline(fin,input_str);
		fout << input_str << endl;
	}
}