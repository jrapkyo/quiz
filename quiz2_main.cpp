#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int MAX_CASE_NUM = 100;
const int MAX_STATEMENT_NUM = 10000;
const string simbols = "(+*) ";

void bracketCaseCal(const string text);
string replace(const string &text, const string &pattern, const string &replace, int &nCount);
int PostFix_Calculate(const string text);
string GetPostFix(const string text);

// ������ ��� �Լ�
// text : ������ �Է� 
int PostFix_Calculate(const string text)
{
	vector<int> stack; // ��ȣ������ �߰� ���� �����ϱ� ���� ����
	string str;
	int nPreValue = 0;
	int nAfterValue = 0;
	int nValue = 0;
	int nNumber = 0;
	
	stringstream PostFix_Expressions(text);
	while (PostFix_Expressions.eof() == false)
	{
		PostFix_Expressions >> str;
		// ������ ���
		if (simbols.find(str) == string::npos)
		{			
			stringstream(str) >> nNumber;
			stack.push_back(nNumber);
		}
		else //������� ���
		{			
			nAfterValue = stack.back();
			stack.pop_back();

			nPreValue = stack.back();
			stack.pop_back();			

			switch (simbols[simbols.find(str)])
			{
			case '+':				
				nValue = ((nPreValue + nAfterValue) % 100000000); // ������ ����				
				stack.push_back(nValue);
				
				break;

			case '*':				
				nValue = ((nPreValue * nAfterValue) % 100000000); // ������ ����				
				stack.push_back(nValue);
				
				break;
			}
		}
	}

	return stack.back();
}

// ġȯ ���ڿ��� ���������� �����ϴ� �Լ�
// text : ��ȣ���� ġȯ�� ���ڿ�

string GetPostFix(const string text)
{
	char cSumflag = 0; // 33, 11, 22 ���� �Ŀ� ������ ���ϱ⸦ �߰��ϱ� ���� �÷���
	string PostFix_Expressions; // ������ ���� ���ڿ�
	vector<char> stack; // ���������� �����ϱ� ���� ���Ǵ� Stack

	auto end = text.end();
	for (auto it = text.begin(); it != end; ++it)
	{
		// ������ ���
		if (simbols.find(*it) == string::npos)
		{
			// ġȯ ���ڿ� Ư�� �� ���ڰ� �ߺ��� ���
			// �ߺ� ���� �߰��� + ��ȣ�� �߰��Ѵ�.
			if (cSumflag == 1)
			{
				while (stack.size() != 0 && stack.back() != '(')
				{
					PostFix_Expressions += " ";
					PostFix_Expressions += stack.back();					
					stack.pop_back();
				}
				stack.push_back('+');

				PostFix_Expressions += " ";
			}

			// �÷��� �����ϰ� ������ ���ڿ��� ����
			PostFix_Expressions += *it;
			cSumflag = 1;

			continue;
		}

		// �������� ���
		// �÷��׸� ����
		cSumflag = 0;		
		PostFix_Expressions += " ";

		switch (*it)
		{
		case '(': 
			stack.push_back('('); 
			break;

		case ')':
			// ��ȣ ó��
			// ���� ������ ��ȣ�� ���ö����� ���ÿ� �ִ� ���ڿ��� ���������� ����			
			while (stack.back() != '(')
			{
				PostFix_Expressions += stack.back();
				PostFix_Expressions += " ";
				stack.pop_back();
			}
			stack.pop_back();
			cSumflag = 1; // ��ȣ�� ���ڻ��̿� ���ϱ⸦ �߰� �ϱ� ���� ����
			break;

		case '*':
			stack.push_back('*');
			break;

		default:
			
			break;
		}		
	}

	// ���ÿ� �����ִ� ���ڿ����� ���������� ����
	size_t stackSize = stack.size();
	for (size_t i = 0; i < stackSize; ++i)
	{
		PostFix_Expressions += " ";
		PostFix_Expressions += stack.back();
		stack.pop_back();
	}

	return PostFix_Expressions;
}

// ���ڿ��� Ư�� ���ڸ� ġȯ�ϴ� �Լ�
// text : �Է� ���ڿ�
// pattern : ġȯ Ÿ�� ����
// replace : ġȯ ����
// nCount : ġȯ�� ���� ��

string replace(const string &text, const string &pattern, const string &replace, int &nCount)
{
	string result;
	string::size_type pos = 0;
	string::size_type offset = 0;

	result = text;

	while ((pos = result.find(pattern, offset)) != string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
		nCount++;
	}

	return result;
}

// �Է¹��� ���ڿ��� �˸°� ġȯ�ϰ� ����ϴ� �Լ�
// text : ��ȣ ���ڿ�

void bracketCaseCal(const string text)
{
	int nPreCount = 0;
	int nAfterCount = 0;
	int ntemp = 0;
	string statement;

	// �Էµ� ��ȣ ���ڿ��� ġȯ�Ѵ�. 
	// ()�� 1�� {}�� 2�� []�� 3���� ġȯ�Ѵ�.
	// ���ӵ��� �ʴ� ��ȣ�� ���ϱ� �̹Ƿ� �׿� �°� 	
	// ���� ��ȣ�� 'X*(' �� ġȯ�ϰ� ���� ��ȣ�� ')'�� ġȯ�Ѵ�.

	statement = text;

	statement = replace(statement, "()", "1", ntemp);
	statement = replace(statement, "{}", "2", ntemp);
	statement = replace(statement, "[]", "3", ntemp);

	// �Ұ�ȣ�� ġȯ ���� �������� ������ �߸��� ���ڿ� ó��
	statement = replace(statement, "(", "1*(", nPreCount);
	statement = replace(statement, ")", ")", nAfterCount);

	if (nPreCount != nAfterCount)
	{
		cout << 0 << endl;
		return;
	}

	// �߰�ȣ�� ġȯ ���� �������� ������ �߸��� ���ڿ� ó��
	statement = replace(statement, "{", "2*(", nPreCount);
	statement = replace(statement, "}", ")", nAfterCount);

	if (nPreCount != nAfterCount)
	{
		cout << 0 << endl;
		return;
	}

	// ���ȣ�� ġȯ ���� �������� ������ �߸��� ���ڿ� ó��
	statement = replace(statement, "[", "3*(", nPreCount);
	statement = replace(statement, "]", ")", nAfterCount);

	if (nPreCount != nAfterCount)
	{
		cout << 0 << endl;
		return;
	}

	// ����� ��ȣ�� ���� �˻� �� ��ø�� ��ȣ�� ���� �˻��Ѵ�.
	if (statement.find("()") != string::npos)
	{
		cout << 0 << endl;
		return;
	}

	// ġȯ�� ��� ���� ���������� ��ȯ�Ѵ�.
	string PostFix_Expressions = GetPostFix(statement);

	// �������� ����Ѵ�.
	int nResult = 0;
	nResult = PostFix_Calculate(PostFix_Expressions);

	// ��� ���
	cout << nResult << endl;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Usage: quiz2.exe <filename>" << endl;
		return 1;
	}

	ifstream myfile(argv[1]);

	if (!myfile)
	{
		cout << "Cannot open input file." << endl;
		return 1;
	}	

	string nCaseCount; // �׽�ũ ���̽� �� ����
	char cStateMent[MAX_STATEMENT_NUM]; // ��ȣ ���ڿ� ����

	if (myfile.is_open() == true)
	{
 		while (myfile.eof() == false)
		{
			if (nCaseCount.empty())
			{
				getline(myfile, nCaseCount); // �׽�Ʈ ���̽� ��
				if (atoi(nCaseCount.c_str()) > MAX_CASE_NUM) // �׽�Ʈ ���̽� �� �˻�
				{
					cout << "invalid Test Case Number." << endl;
				}
			}
			else
			{
				myfile.getline(cStateMent, MAX_STATEMENT_NUM);
				
				if (cStateMent[0] == NULL)
				{
					continue;
				}

				//��ȣ ���ڿ��� �ִ� ���̴� 10,000���̹Ƿ� 10,000�ڰ� �Ѿ� ���� �κ��� ����
				if (myfile.good() == false && myfile.eof() == false)
				{
					myfile.clear();
					myfile.ignore(MAX_STATEMENT_NUM, '\n');

					continue;
				}

				bracketCaseCal(cStateMent);
			}
		}
	}

	return 1;
}