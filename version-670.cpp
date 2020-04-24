#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<algorithm>
#include<functional>
#include<ctime>
#include<unordered_map>

using namespace std;
typedef int dataType;

//save csv struct
typedef vector<vector<dataType>> inputStruct;

void showInputStruct(const inputStruct& input);
// read csv file
inputStruct readCsv(const char*);
//sort-Merge
inputStruct joinCsv_hashMerge(inputStruct&, inputStruct&, int, int);
//gropyBy with sort
void gropyBy_sort2(inputStruct&, int, int, vector<dataType>&);
int findMax2(int, int);
int findMin2(int, int);

ofstream out;
inputStruct res;

int main()
{
	iostream::sync_with_stdio(false);
	//out.open("E:\\ztdatabase\\myoutput_final_new.csv");
	clock_t stread = clock();
	//inputStruct input1 = readCsv("/home/web/ztedatabase/myinput1.csv");
	inputStruct input1 = readCsv("E:\\ztdatabase\\myinput1.csv");
	//showInputStruct(input1);
	//inputStruct input2 = readCsv("/home/web/ztedatabase/myinput2.csv");
	inputStruct input2 = readCsv("E:\\ztdatabase\\myinput2.csv");
	clock_t endread = clock();
	cout << "read time:" << (endread - stread) / CLOCKS_PER_SEC << endl;
	clock_t stjoin = clock();
	res = joinCsv_hashMerge(input1, input2, 3, 3);
	clock_t endjoin = clock();
	cout << "join file time:" << (endjoin - stjoin) / CLOCKS_PER_SEC << endl;
	input1.clear();
	input2.clear();
	vector<dataType> index(0);
	clock_t stsort2 = clock();
	gropyBy_sort2(res, 1, 4, index);
	index.push_back(res.size());
	clock_t endsort2 = clock();
	cout << "group file time:" << (endsort2 - stsort2) / CLOCKS_PER_SEC << endl;
	int size_index = index.size();
	clock_t stzhuan = clock();
	for (int i = 0;i < size_index - 1;i++)
	{
		int max = findMax2(index[i], index[i + 1]);
		int min = findMin2(index[i], index[i + 1]);
		res[i][0] = max;
		res[i][1] = min;
		res[i][2] = res[index[i]][4];
		res[i][3] = res[index[i]][2];
	}
	clock_t endzhuan = clock();
	cout << "zhuan time:" << (endzhuan - stzhuan) / CLOCKS_PER_SEC << endl;
	clock_t stsort = clock();
	sort(res.begin(), res.begin()+ size_index-1, [](vector<dataType>& input1, vector<dataType>& input2) {
		if (input1[0] < input2[0])
		{
			return true;
		}
		else if (input1[0] > input2[0])
		{
			return false;
		}
		else if (input1[0] == input2[0])
		{
			if (input1[2] < input2[2])
			{
				return true;
			}
			else if (input1[2] > input2[2])
			{
				return false;
			}
			else if (input1[2] == input2[2])
			{
				if (input1[3] < input2[3])
				{
					return true;
				}
				else if (input1[3] > input2[3])
				{
					return false;
				}
				else if (input1[3] == input2[3])
				{
					return false;
				}
			}
		}
		});
	clock_t endsort = clock();
	cout << "sort time:" << (endsort - stsort) / CLOCKS_PER_SEC << endl;
	getchar();
	for (int i = 0;i < size_index-1;i++)
	{
		cout << res[i][0] << "," << res[i][1] << endl;
		//out << res[i][0] << "," << res[i][1] << endl;
	}
	return 0;
}

inputStruct readCsv(const char* filename)
{
	ifstream inFile(filename, ios::in);
	string lineStr;
	inputStruct strArray;
	int d1, d2, d3;
	char sp;
	vector<dataType> lineArray;
	while (inFile>>d1>>sp>>d2>>sp>>d3)
	{
		lineArray.clear();
		lineArray.push_back(d1);
		lineArray.push_back(d2);
		lineArray.push_back(d3);
		strArray.push_back(lineArray);
	}
	return strArray;
}

void showInputStruct(const inputStruct& input)
{
	for (int i = 0;i < input.size();i++)
	{
		for (int j = 0;j < input[i].size();j++)
		{
			cout << input[i][j]<<" ";
		}
		cout << endl;
		//cout <<i<<"  "<<input[i][0] << "," << input[i][1] << "," << input[i][2] << "," << input[i][3] << "," << input[i][4] << "," << input[i][5] << endl;
	}
}

inputStruct joinCsv_hashMerge(inputStruct & input1, inputStruct & input2, int t1, int t2)
{
	int size1 = input1.size();
	int size2 = input2.size();
	inputStruct joinRes(0);
	unordered_multimap<int, vector<dataType>> t1map;
	vector<dataType> temp(0);
	pair<unordered_multimap<int, vector<dataType>>::iterator,
		unordered_multimap<int, vector<dataType>>::iterator> pair1;
	if (size1 <= size2)
	{
		for (int i = 0;i < size1;i++)
		{
			t1map.insert(make_pair(input1[i][t1 - 1], input1[i]));
		}
		for (int i = 0;i < size2;i++)
		{
			if (t1map.find(input2[i][t2 - 1]) != t1map.end())
			{
				pair1 = t1map.equal_range(input2[i][t2 - 1]);
				for (;pair1.first != pair1.second;pair1.first++)
				{
					temp.clear();
					temp.push_back(pair1.first->second[0]);
					temp.push_back(pair1.first->second[1]);
					temp.push_back(pair1.first->second[2]);
					temp.push_back(input2[i][0]);
					temp.push_back(input2[i][1]);
					temp.push_back(input2[i][2]);
					joinRes.push_back(temp);
				}
			}
		}
	}
	else if (size2 < size1)
	{
		for (int i = 0;i < size2;i++)
		{
			t1map.insert(make_pair(input2[i][t2 - 1], input2[i]));
		}
		for (int i = 0;i < size1;i++)
		{
			if (t1map.find(input1[i][t1 - 1]) != t1map.end())
			{
				pair1 = t1map.equal_range(input1[i][t1 - 1]);
				for (;pair1.first != pair1.second;pair1.first++)
				{
					temp.clear();
					temp.push_back(pair1.first->second[0]);
					temp.push_back(pair1.first->second[1]);
					temp.push_back(pair1.first->second[2]);
					temp.push_back(input1[i][0]);
					temp.push_back(input1[i][1]);
					temp.push_back(input1[i][2]);
					joinRes.push_back(temp);
				}
			}
		}
	}

	return joinRes;
}

void gropyBy_sort2(inputStruct & input, int t1, int t2, vector<dataType> & index)
{
	int size = input.size();
	sort(input.begin(), input.end(), [](vector<dataType>& fir, vector<dataType>& sec)
		{
			if (fir[1] < sec[1])
			{
				return true;
			}
			else if (fir[1] > sec[1])
			{
				return false;
			}
			else if (fir[1] == sec[1])
			{
				if (fir[4] == sec[4])
				{
					return false;
				}
				else if (fir[4] < sec[4])
				{
					return true;
				}
				else if (fir[4] > sec[4])
				{
					return false;
				}
			}
		});
	index.push_back(0);
	for (int i = 1;i < size;i++)
	{
		if ((input[i - 1][t1] != input[i][t1])
			|| input[i - 1][t2] != input[i][t2])
		{
			index.push_back(i);
		}
	}
}

int findMax2(int index1, int index2)
{
	int flag = res[index1][0];
	for (;index1 < index2;index1++)
	{
		if (flag < res[index1][0])
		{
			flag = res[index1][0];
		}
	}
	return flag;
}

int findMin2(int index1, int index2)
{
	int flag = res[index1][3];
	for (;index1 < index2;index1++)
	{
		if (res[index1][3] < flag)
		{
			flag = res[index1][3];
		}
	}
	return flag;
}