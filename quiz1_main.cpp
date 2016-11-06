#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

const int MAX_NAME_SIZE = 50; //후보자 이름 최대 50자

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Usage: quiz.exe <filename>" << endl;

		return 1;
	}

	ifstream myfile(argv[1]);

	if (!myfile)
	{
		cout << "Cannot open input file." << endl;

		return 1;
	}

	// 파일을 읽어 data container로 저장하는 부분

	char cName[MAX_NAME_SIZE]; // 파일에서 후보자 이름을 저장하는 배열
	unordered_multiset<string> Persons; // 후보자 이름을 저장한 unordered_multiset 

	vector<pair<string, int>> Result; // 후보자 이름와 Count 저장 vector
	vector<pair<string, int>> SortResult; // 마지막 당선된 후보자를 정렬하기 위한 vector

	if (myfile.is_open() == true)
	{
		while (myfile.eof() == false)
		{
			myfile.getline(cName, MAX_NAME_SIZE);

			//후보자 이름은 최대 50자이므로 50자가 넘어 가는 부분은 무시
			if (myfile.good() == false && myfile.eof() == false)
			{
				myfile.clear();
				myfile.ignore(MAX_NAME_SIZE, '\n');

				continue;
			}

			if (cName[0] != NULL)
			{
				Persons.insert(cName); //이름 저장
			}
		}
	}

	// unordered_multiset을 순회하며 후보자의 이름와 Count를 Vector형태로 저장

	int nVoteCount = 0; // 후보자 투표 수
	int nMaxVoteCount = 0; // 당선 후보자 투표 수

	auto PersonsEnd = Persons.end();
	for (auto Name_it = Persons.begin(); Name_it != PersonsEnd; ++Name_it, nVoteCount--)
	{
		// unordered_multiset은 데이터가 중복되어 입력되며 중복된 데이터가 순차적으로
		// 구성되어 있으므로 데이터 Count만 구하고 뒷부분은 넘어감
		if (nVoteCount == 0)
		{
			nVoteCount = Persons.count(*Name_it);
			Result.push_back(make_pair(*Name_it, nVoteCount));
			if (nVoteCount > nMaxVoteCount)
			{
				nMaxVoteCount = nVoteCount;
			}
		}
	}

	// 최다 득표자를 별도의 Vector로 저장
	auto ResultEnd = Result.end();
	for (auto R_it = Result.begin(); R_it != ResultEnd; ++R_it)
	{
		if (R_it->second == nMaxVoteCount)
		{
			SortResult.push_back(make_pair(R_it->first, R_it->second));
		}
	}

	// 이름순으로 정렬
	sort(SortResult.begin(), SortResult.end());

	// 결과 출력
	auto SortResultEnd = SortResult.end();
	for (auto SR_it = SortResult.begin(); SR_it != SortResult.end(); ++SR_it)
	{
		cout << SR_it->first << endl;
	}

	return 1;
}
