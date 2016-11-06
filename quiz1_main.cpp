#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

const int MAX_NAME_SIZE = 50; //�ĺ��� �̸� �ִ� 50��

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

	// ������ �о� data container�� �����ϴ� �κ�

	char cName[MAX_NAME_SIZE]; // ���Ͽ��� �ĺ��� �̸��� �����ϴ� �迭
	unordered_multiset<string> Persons; // �ĺ��� �̸��� ������ unordered_multiset 

	vector<pair<string, int>> Result; // �ĺ��� �̸��� Count ���� vector
	vector<pair<string, int>> SortResult; // ������ �缱�� �ĺ��ڸ� �����ϱ� ���� vector

	if (myfile.is_open() == true)
	{
		while (myfile.eof() == false)
		{
			myfile.getline(cName, MAX_NAME_SIZE);

			//�ĺ��� �̸��� �ִ� 50���̹Ƿ� 50�ڰ� �Ѿ� ���� �κ��� ����
			if (myfile.good() == false && myfile.eof() == false)
			{
				myfile.clear();
				myfile.ignore(MAX_NAME_SIZE, '\n');

				continue;
			}

			if (cName[0] != NULL)
			{
				Persons.insert(cName); //�̸� ����
			}
		}
	}

	// unordered_multiset�� ��ȸ�ϸ� �ĺ����� �̸��� Count�� Vector���·� ����

	int nVoteCount = 0; // �ĺ��� ��ǥ ��
	int nMaxVoteCount = 0; // �缱 �ĺ��� ��ǥ ��

	auto PersonsEnd = Persons.end();
	for (auto Name_it = Persons.begin(); Name_it != PersonsEnd; ++Name_it, nVoteCount--)
	{
		// unordered_multiset�� �����Ͱ� �ߺ��Ǿ� �ԷµǸ� �ߺ��� �����Ͱ� ����������
		// �����Ǿ� �����Ƿ� ������ Count�� ���ϰ� �޺κ��� �Ѿ
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

	// �ִ� ��ǥ�ڸ� ������ Vector�� ����
	auto ResultEnd = Result.end();
	for (auto R_it = Result.begin(); R_it != ResultEnd; ++R_it)
	{
		if (R_it->second == nMaxVoteCount)
		{
			SortResult.push_back(make_pair(R_it->first, R_it->second));
		}
	}

	// �̸������� ����
	sort(SortResult.begin(), SortResult.end());

	// ��� ���
	auto SortResultEnd = SortResult.end();
	for (auto SR_it = SortResult.begin(); SR_it != SortResult.end(); ++SR_it)
	{
		cout << SR_it->first << endl;
	}

	return 1;
}
