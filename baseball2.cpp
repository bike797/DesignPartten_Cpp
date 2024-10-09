#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>

using namespace std;

// 중복되지 않는 랜덤 숫자 3개 생성 함수
// 1부터 9까지의 숫자 중 중복되지 않게 3개를 랜덤하게 선택하여 벡터에 저장
vector<int> generateRandomNumbers() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 9);

    vector<int> numbers;
    while (numbers.size() < 3) {
        int num = dis(gen);
        if (find(numbers.begin(), numbers.end(), num) == numbers.end()) {
            numbers.push_back(num);
        }
    }
    return numbers;
}

// 가능한 숫자 조합 갱신
// 사용자의 입력 (strike, ball)에 맞게 가능한 숫자 조합을 갱신
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

// 상태 평가 함수 (가능한 숫자 조합의 수가 적을수록 좋은 값을 반환)
int evaluate(const vector<vector<int>>& possibleNumbers) {
    return possibleNumbers.size();
}

// 미니맥스 알고리즘 (알파-베타 가지치기 포함)
int alphabeta(vector<vector<int>>& possibleNumbers, int depth, int alpha, int beta, bool maximizingPlayer, int strike, int ball) {
    if (depth == 0 || possibleNumbers.empty()) {
        return evaluate(possibleNumbers); // 더 이상 탐색할 수 없을 때 평가 함수 호출
    }

    if (maximizingPlayer) { // 컴퓨터의 차례 (최대값 찾기)
        int bestValue = INT_MIN;
        for (const auto& guess : possibleNumbers) {
            vector<vector<int>> newPossibleNumbers = possibleNumbers;
            updatePossibleNumbers(newPossibleNumbers, guess, strike, ball); // 가상으로 추측하고 상태 갱신
            int value = alphabeta(newPossibleNumbers, depth - 1, alpha, beta, false, strike, ball);
            bestValue = max(bestValue, value);
            alpha = max(alpha, bestValue);
            if (beta <= alpha) {
                break; // beta cutoff (더 이상 탐색할 필요 없음)
            }
        }
        return bestValue;
    } else { // 사용자의 차례 (최소값 찾기)
        int bestValue = INT_MAX;
        for (const auto& guess : possibleNumbers) {
            vector<vector<int>> newPossibleNumbers = possibleNumbers;
            updatePossibleNumbers(newPossibleNumbers, guess, strike, ball);
            int value = alphabeta(newPossibleNumbers, depth - 1, alpha, beta, false, strike, ball);
            bestValue = min(bestValue, value);
            beta = min(beta, bestValue);
            if (beta <= alpha) {
                break; // alpha cutoff (더 이상 탐색할 필요 없음)
            }
        }
        return bestValue;
    }
}

// 최적의 숫자 조합 찾기
vector<int> findBestGuess(vector<vector<int>>& possibleNumbers, int maxDepth, int strike, int ball) {
    int bestValue = INT_MIN;
    vector<int> bestGuess;
    for (const auto& guess : possibleNumbers) {
        vector<vector<int>> newPossibleNumbers = possibleNumbers;
        updatePossibleNumbers(newPossibleNumbers, guess, strike, ball);
        int value = alphabeta(newPossibleNumbers, maxDepth - 1, INT_MIN, INT_MAX, false, strike, ball);
        if (value > bestValue) {
            bestValue = value;
            bestGuess = guess;
        }
    }
    return bestGuess;
}

int main() {
    int strike = 0;
    int ball = 0;
    srand(time(NULL));

    // 1부터 9까지 서로 다른 숫자 3개를 생각하세요.
    cout << "1부터 9까지 서로 다른 숫자 3개를 생각하세요." << endl;

    // 난이도를 선택하세요 (초급: 1, 중급: 2, 고급: 3)
    cout << "난이도를 선택하세요 (초급: 1, 중급: 2, 고급: 3): ";
    int difficulty;
    cin >> difficulty;

    // 난이도에 따른 최대 탐색 깊이 설정
    int maxDepth;
    if (difficulty == 1) {
        maxDepth = 2; // 초급
    } else if (difficulty == 2) {
        maxDepth = 4; // 중급
    } else {
        maxDepth = 6; // 고급
    }

    // 가능한 모든 숫자 조합 생성
    // ... (1부터 9까지의 숫자를 사용하여 모든 가능한 조합 생성)
    vector<vector<int>> possibleNumbers;
    for (int i = 1; i <= 9; ++i) {
        for (int j = 1; j <= 9; ++j) {
            if (i != j) {
                for (int k = 1; k <= 9; ++k) {
                    if (i != k && j != k) {
                        possibleNumbers.push_back({i, j, k});
                    }
                }
            }
        }
    }

    while (true) {
        vector<int> computerGuess = findBestGuess(possibleNumbers, maxDepth, strike, ball);
        cout << "컴퓨터의 숫자: " << computerGuess[0] << computerGuess[1] << computerGuess[2] << endl;

        // 스트라이크, 볼, 아웃 입력 받기
        int strike, ball;
        cin >> strike >> ball;

        updatePossibleNumbers(possibleNumbers, computerGuess, strike, ball);

        // 3 스트라이크면 게임 종료
        if (strike == 3) {
            cout << "컴퓨터가 맞췄습니다!" << endl;
            break;
        }
    }

    return 0;
}