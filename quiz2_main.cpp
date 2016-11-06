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

// 후위식 계산 함수
// text : 후위식 입력 
int PostFix_Calculate(const string text)
{
	vector<int> stack; // 괄호단위로 중간 값을 저장하기 위한 스택
	string str;
	int nPreValue = 0;
	int nAfterValue = 0;
	int nValue = 0;
	int nNumber = 0;
	
	stringstream PostFix_Expressions(text);
	while (PostFix_Expressions.eof() == false)
	{
		PostFix_Expressions >> str;
		// 숫자인 경우
		if (simbols.find(str) == string::npos)
		{			
			stringstream(str) >> nNumber;
			stack.push_back(nNumber);
		}
		else //연산식인 경우
		{			
			nAfterValue = stack.back();
			stack.pop_back();

			nPreValue = stack.back();
			stack.pop_back();			

			switch (simbols[simbols.find(str)])
			{
			case '+':				
				nValue = ((nPreValue + nAfterValue) % 100000000); // 나머지 연산				
				stack.push_back(nValue);
				
				break;

			case '*':				
				nValue = ((nPreValue * nAfterValue) % 100000000); // 나머지 연산				
				stack.push_back(nValue);
				
				break;
			}
		}
	}

	return stack.back();
}

// 치환 문자열을 후위식으로 변경하는 함수
// text : 괄호식이 치환된 문자열

string GetPostFix(const string text)
{
	char cSumflag = 0; // 33, 11, 22 등의 식에 생략된 더하기를 추가하기 위한 플래그
	string PostFix_Expressions; // 후위식 저장 문자열
	vector<char> stack; // 후위식으로 변경하기 위해 사용되는 Stack

	auto end = text.end();
	for (auto it = text.begin(); it != end; ++it)
	{
		// 숫자인 경우
		if (simbols.find(*it) == string::npos)
		{
			// 치환 문자열 특성 상 숫자가 중복된 경우
			// 중복 숫자 중간에 + 기호를 추가한다.
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

			// 플래그 설정하고 후위식 문자열에 저장
			PostFix_Expressions += *it;
			cSumflag = 1;

			continue;
		}

		// 연산자인 경우
		// 플래그를 리셋
		cSumflag = 0;		
		PostFix_Expressions += " ";

		switch (*it)
		{
		case '(': 
			stack.push_back('('); 
			break;

		case ')':
			// 괄호 처리
			// 이전 정의한 괄호가 나올때까지 스택에 있는 문자열을 후위식으로 저장			
			while (stack.back() != '(')
			{
				PostFix_Expressions += stack.back();
				PostFix_Expressions += " ";
				stack.pop_back();
			}
			stack.pop_back();
			cSumflag = 1; // 괄호와 숫자사이에 더하기를 추가 하기 위해 설정
			break;

		case '*':
			stack.push_back('*');
			break;

		default:
			
			break;
		}		
	}

	// 스택에 남아있는 문자열들을 후위식으로 저장
	size_t stackSize = stack.size();
	for (size_t i = 0; i < stackSize; ++i)
	{
		PostFix_Expressions += " ";
		PostFix_Expressions += stack.back();
		stack.pop_back();
	}

	return PostFix_Expressions;
}

// 문자열의 특정 문자를 치환하는 함수
// text : 입력 문자열
// pattern : 치환 타겟 문자
// replace : 치환 문자
// nCount : 치환된 문자 수

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

// 입력받은 문자열을 알맞게 치환하고 계산하는 함수
// text : 괄호 문자열

void bracketCaseCal(const string text)
{
	int nPreCount = 0;
	int nAfterCount = 0;
	int ntemp = 0;
	string statement;

	// 입력된 괄호 문자열을 치환한다. 
	// ()는 1로 {}는 2로 []는 3으로 치환한다.
	// 연속되지 않는 괄호는 곱하기 이므로 그에 맞게 	
	// 앞쪽 괄호는 'X*(' 로 치환하고 뒤쪽 괄호는 ')'로 치환한다.

	statement = text;

	statement = replace(statement, "()", "1", ntemp);
	statement = replace(statement, "{}", "2", ntemp);
	statement = replace(statement, "[]", "3", ntemp);

	// 소괄호의 치환 수가 동일하지 않으면 잘못된 문자열 처리
	statement = replace(statement, "(", "1*(", nPreCount);
	statement = replace(statement, ")", ")", nAfterCount);

	if (nPreCount != nAfterCount)
	{
		cout << 0 << endl;
		return;
	}

	// 중괄호의 치환 수가 동일하지 않으면 잘못된 문자열 처리
	statement = replace(statement, "{", "2*(", nPreCount);
	statement = replace(statement, "}", ")", nAfterCount);

	if (nPreCount != nAfterCount)
	{
		cout << 0 << endl;
		return;
	}

	// 대괄호의 치환 수가 동일하지 않으면 잘못된 문자열 처리
	statement = replace(statement, "[", "3*(", nPreCount);
	statement = replace(statement, "]", ")", nAfterCount);

	if (nPreCount != nAfterCount)
	{
		cout << 0 << endl;
		return;
	}

	// 선언된 괄호의 쌍을 검사 후 중첩된 괄호에 대해 검사한다.
	if (statement.find("()") != string::npos)
	{
		cout << 0 << endl;
		return;
	}

	// 치환된 곈산 식을 후위식으로 변환한다.
	string PostFix_Expressions = GetPostFix(statement);

	// 후위식을 계산한다.
	int nResult = 0;
	nResult = PostFix_Calculate(PostFix_Expressions);

	// 결과 출력
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

	string nCaseCount; // 테스크 케이스 수 저장
	char cStateMent[MAX_STATEMENT_NUM]; // 괄호 문자열 저장

	if (myfile.is_open() == true)
	{
 		while (myfile.eof() == false)
		{
			if (nCaseCount.empty())
			{
				getline(myfile, nCaseCount); // 테스트 케이스 수
				if (atoi(nCaseCount.c_str()) > MAX_CASE_NUM) // 테스트 케이스 수 검사
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

				//괄호 문자열은 최대 길이는 10,000자이므로 10,000자가 넘어 가는 부분은 무시
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