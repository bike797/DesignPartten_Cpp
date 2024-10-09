#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

bool isValidInput(int strike, int ball, int out) {
    return (strike >= 0 && ball >= 0 && out >= 0 && strike + ball + out == 3);
}

// 중복되지 않는 랜덤 숫자 3개 생성 함수
vector<int> generateRandomNumbers() {
    vector<int> numbers;
    while (numbers.size() < 3) {
        int num = rand() % 9 + 1;
        if (find(numbers.begin(), numbers.end(), num) == numbers.end()) {
            numbers.push_back(num);
        }
    }
    return numbers;
}

// 가능한 숫자 조합 갱신 로직
void updatePossibleNumbers(vector<vector<int>>& possibleNumbers, const vector<int>& guess, int strike, int ball) {
    vector<vector<int>> newPossibleNumbers;

    for (const auto& numbers : possibleNumbers) {
        int currentStrike = 0, currentBall = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (numbers[i] == guess[j]) {
                    if (i == j) {
                        currentStrike++;
                    } else {
                        currentBall++;
                    }
                }
            }
        }

        // 스트라이크, 볼 개수가 일치하면 새로운 가능한 숫자 조합에 추가
        if (currentStrike == strike && currentBall == ball) {
            newPossibleNumbers.push_back(numbers);
        }
    }

    possibleNumbers = newPossibleNumbers;
}


// minimax algorithm
/*
미니맥스 알고리즘은 게임 트리를 탐색하며 최적의 선택을 찾는 알고리즘입니다. 야구 게임에 적용하면 다음과 같이 활용할 수 있습니다.

게임 상태: 현재까지의 추측과 사용자의 피드백(스트라이크, 볼)을 기반으로 가능한 숫자 조합을 나타냅니다.
가능한 행동: 다음에 추측할 숫자를 선택하는 것입니다.
평가 함수: 현재 상태에서 게임이 얼마나 유리한지 평가하는 함수입니다. 예를 들어, 남은 가능한 숫자 조합의 수가 적을수록 좋은 상태라고 평가할 수 있습니다.
최대/최소: 컴퓨터는 최대의 이익을 얻기 위해 최대값을 선택하고, 상대(사용자)는 최소의 손해를 보기 위해 최소값을 선택하는 것으로 가정합니다.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ... (기존 코드)

int evaluate(const vector<vector<int>>& possibleNumbers) {
    // 평가 함수: 가능한 숫자 조합의 수가 적을수록 좋은 값을 반환
    return possibleNumbers.size();
}

int minimax(vector<vector<int>>& possibleNumbers, int depth, bool maximizingPlayer) {
    if (depth == 0 || possibleNumbers.empty()) {
        return evaluate(possibleNumbers);
    }

    if (maximizingPlayer) { // 컴퓨터의 차례
        int bestValue = INT_MIN;
        for (const auto& guess : possibleNumbers) {
            vector<vector<int>> newPossibleNumbers = possibleNumbers;
            // 가상으로 guess를 추측하고, 사용자의 반응을 예측하여 newPossibleNumbers를 업데이트
            // ... (updatePossibleNumbers 함수 호출)
            
            int value = minimax(newPossibleNumbers, depth - 1, false);
            bestValue = max(bestValue, value);
        }
        return bestValue;
    } else { // 사용자의 차례 (최악의 경우를 가정)
        int bestValue = INT_MAX;
        // ... (컴퓨터와 유사하게 처리)
        return bestValue;
    }
}
int alphabeta(vector<vector<int>>& possibleNumbers, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || possibleNumbers.empty()) {
        return evaluate(possibleNumbers);
    }

    if (maximizingPlayer) {
        int bestValue = INT_MIN;
        for (const auto& guess : possibleNumbers) {
            vector<vector<int>> newPossibleNumbers = possibleNumbers;
            // ... (updatePossibleNumbers 함수 호출)
            int value = alphabeta(newPossibleNumbers, depth - 1, alpha, beta, false);
            bestValue = max(bestValue, value);
            alpha = max(alpha, bestValue);
            if (beta <= alpha) {
                break; // beta cutoff
            }
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        for (const auto& guess : possibleNumbers) {
            vector<vector<int>> newPossibleNumbers = possibleNumbers;
            // ... (updatePossibleNumbers 함수 호출)
            int value = alphabeta(newPossibleNumbers, depth - 1, alpha, beta, true);
            bestValue = min(bestValue, value);
            beta = min(beta, bestValue);
            if (beta <= alpha) {
                break; // alpha cutoff
            }
        }
        return bestValue;
    }
}

vector<int> findBestGuess(vector<vector<int>>& possibleNumbers) {
    int bestValue = INT_MIN;
    vector<int> bestGuess;
    for (const auto& guess : possibleNumbers) {
        vector<vector<int>> newPossibleNumbers = possibleNumbers;
        // ... (updatePossibleNumbers 함수 호출)
        int value = minimax(newPossibleNumbers, depth - 1, false);
        if (value > bestValue) {
            bestValue = value;
            bestGuess = guess;
        }
    }
    return bestGuess;
}

int main() {
    srand(time(NULL));

    cout << "1부터 9까지 서로 다른 숫자 3개를 생각하세요." << endl;
    cout << "난이도를 선택하세요 (초급: 1, 중급: 2, 고급: 3): ";
    int difficulty;
    cin >> difficulty;

    vector<vector<int>> possibleNumbers;
    // ... (초기 가능한 숫자 조합 생성)

    while (true) {
        vector<int> computerGuess;
        // 난이도에 따라 다른 추측 전략 사용
        switch (difficulty) {
            case 1:
                // 초급: 완전 랜덤 추측
                computerGuess = generateRandomNumbers();
                break;
            case 2:
                // 중급: 가능한 숫자 조합 중 랜덤 선택
                computerGuess = possibleNumbers[rand() % possibleNumbers.size()];
                break;
            case 3:
                // 고급: 더욱 정교한 알고리즘 (예: 미니맥스 알고리즘 등)
                // ... (고급 알고리즘 구현)
                break;
        }

        cout << "컴퓨터의 숫자: " << computerGuess[0] << computerGuess[1] << computerGuess[2] << endl;
        cout << "스트라이크, 볼, 아웃을 입력하세요 (예: 2 1 0): ";

        int strike, ball, out;
        cin >> strike >> ball >> out;

        if (strike == 3) {
            cout << "컴퓨터가 맞췄습니다!" << endl;
            break;
        }

        updatePossibleNumbers(possibleNumbers, computerGuess, strike, ball);
    }

    return 0;
}