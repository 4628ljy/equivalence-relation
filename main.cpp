#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int N = 5;
using Matrix = vector<vector<int>>;

// 조건 1. 관계 행렬을 한 행씩 입력받는다.
bool readMatrix(Matrix& matrix) 
{
    for (int i = 0; i < N; i++) 
    {
        while (true) 
        {
            cout << i + 1 << "행 입력: ";

            string line;
            if (!getline(cin, line))
                return false;

            vector<int> row;
            bool valid = true;

            for (char ch : line) 
            {
                if (ch == ' ' || ch == '\t') 
                {
                    continue;
                }

                if (ch != '0' && ch != '1') 
                {
                    valid = false;
                    break;
                }

                row.push_back(ch - '0');
            }

            if (valid && row.size() == N) 
            {
                matrix[i] = row;
                break;
            }

            cout << "입력 오류: 0 또는 1을 정확히 5개 입력하세요.\n";
        }
    }

    return true;
}

void printMatrix(const Matrix& matrix) 
{
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++)
        {
            cout << matrix[i][j] << ' ';
        }
        cout << '\n';
    }
}

// 조건 2. 반사성, 대칭성, 추이성을 각각 판별한다.
bool isReflexive(const Matrix& matrix) 
{
    for (int i = 0; i < N; i++) 
    {
        if (matrix[i][i] == 0)
        {
            return false;
        }
    }
    return true;
}

bool isSymmetric(const Matrix& matrix)
{
    for (int i = 0; i < N; i++) 
    {
        for (int j = i + 1; j < N; j++)
        {
            if (matrix[i][j] != matrix[j][i]) 
            {
                return false;
            }
        }
    }
    return true;
}

bool isTransitive(const Matrix& matrix) 
{
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                // i → j와 j → k가 있는데 i → k가 빠져 있는지 확인한다.
                if (matrix[i][j] == 1 && matrix[j][k] == 1 && matrix[i][k] == 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool printProperties(const Matrix& matrix) 
{
    bool reflexive = isReflexive(matrix);
    bool symmetric = isSymmetric(matrix);
    bool transitive = isTransitive(matrix);

    cout << "반사성: " << (reflexive ? "성립" : "불성립") << '\n';
    cout << "대칭성: " << (symmetric ? "성립" : "불성립") << '\n';
    cout << "추이성: " << (transitive ? "성립" : "불성립") << '\n';

    if (reflexive && symmetric && transitive) 
    {
        cout << "동치 관계입니다.\n";
        return true;
    }

    cout << "동치 관계가 아닙니다.\n";
    return false;
}

// 조건 3. 원소 x의 동치류를 구한다.
vector<int> getEquivalenceClass(const Matrix& matrix, int x) 
{
    vector<int> members;

    for (int j = 0; j < N; j++)
    {
        if (matrix[x - 1][j] == 1)
        {
            members.push_back(j + 1);
        }
    }

    return members;
}

void printClasses(const Matrix& matrix) 
{
    cout << "\n[원소별 동치류]\n";

    for (int x = 1; x <= N; x++) 
    {
        vector<int> members = getEquivalenceClass(matrix, x);

        cout << '[' << x << "] = {";
        for (size_t i = 0; i < members.size(); i++)
        {
            if (i > 0)
            {
                cout << ", ";
            }
            cout << members[i];
        }
        cout << "}\n";
    }
}

// 조건 4. 반사 폐포, 대칭 폐포, 추이 폐포를 구한다.
void reflexiveClosure(Matrix& matrix) 
{
    for (int i = 0; i < N; i++) 
        matrix[i][i] = 1;
}

void symmetricClosure(Matrix& matrix) 
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++) 
        {
            if (matrix[i][j] == 1) 
            {
                matrix[j][i] = 1;
            }
        }
    }
}

void transitiveClosure(Matrix& matrix)
{
    for (int k = 0; k < N; k++) 
    {
        for (int i = 0; i < N; i++) 
        {
            for (int j = 0; j < N; j++) 
            {
                if (matrix[i][k] == 1 && matrix[k][j] == 1)
                {
                    matrix[i][j] = 1;
                }
            }
        }
    }
}

// 추가 기능. 새로 추가된 순서쌍과 개수를 출력한다.
void printChanges(const Matrix& before, const Matrix& after) 
{
    int count = 0;
    cout << "추가된 순서쌍: ";

    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            if (before[i][j] == 0 && after[i][j] == 1) 
            {
                cout << '(' << i + 1 << ", " << j + 1 << ") ";
                count++;
            }
        }
    }

    cout << "\n추가된 개수: " << count << "개\n";
}

// 각 폐포의 변환 전후 행렬과 판별 결과를 출력한다.
void showClosureResult(const string& name, const Matrix& before, const Matrix& after)
{
    cout << "\n[" << name << "] 변환 전\n";
    printMatrix(before);

    cout << '[' << name << "] 변환 후\n";
    printMatrix(after);

    printChanges(before, after);
    printProperties(after);
}

int main()
{
    Matrix matrix(N, vector<int>(N, 0));

    if (!readMatrix(matrix))
        return 0;

    cout << "\n[입력한 관계 행렬]\n";
    printMatrix(matrix);

    cout << "\n[최초 판별]\n";
    if (printProperties(matrix))
    {
        printClasses(matrix);
        return 0;
    }

    if (!isReflexive(matrix))
    {
        Matrix before = matrix;
        reflexiveClosure(matrix);
        showClosureResult("반사 폐포", before, matrix);
    }

    if (!isSymmetric(matrix)) 
    {
        Matrix before = matrix;
        symmetricClosure(matrix);
        showClosureResult("대칭 폐포", before, matrix);
    }

    if (!isTransitive(matrix))
    {
        Matrix before = matrix;
        transitiveClosure(matrix);
        showClosureResult("추이 폐포", before, matrix);
    }

    cout << "\n[최종 판별]\n";
    if (printProperties(matrix))
        printClasses(matrix);

    return 0;
}