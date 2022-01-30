#include "connect4.h"
#include <climits>
using namespace std;

vector<vector<int>> current_map;  // 0 empty, 1 self, 2 another

vector<int> minimax(vector<vector<int>> map, int depth, int alpha, int beta, bool maxPlayer);
int terminal_score(const vector<vector<int>> &map);
// 4 INT_MAX, 3 +5000, 2 +100; -100, -5000, INT_MIN
// full: -1, 

void add_score(int &score, int mine, int enemy, int empty);

// The columns are numbered from 0 to 6
int decide(int yp_move) {
    if (yp_move == -1) {
        // A new game starts
        // TODO: Please remember to reset everything here (if needed)
        current_map.clear();
        for (int i = 0; i < 7; i++)
            current_map.push_back({0,0,0,0,0,0});
        current_map[3][0] = 1;
        return 3;
    }
    else {
        // YP placed his piece at column `yp_move`
        int row;
        for (row = 0; row < 6; row++)
            if (current_map[yp_move][row] == 0)  break;
        current_map[yp_move][row] = 2;

        vector<int> step = minimax(current_map, 6, INT_MIN, INT_MAX, true);
        for (row = 0; row < 6; row++)
            if (current_map[step[0]][row] == 0)  break;
        current_map[step[0]][row] = 1;
        return step[0];
    }

    return 6; // always place at the rightmost column
}

void add_score(int &score, int mine, int enemy, int empty)
{
    /* if (mine == 2 && enemy == 1)
        score += 10; */
    if (mine == 2 && empty == 2)
        score += 20;
    else if (mine == 3 && enemy == 1)
        score += 500;
    else if (mine == 3 && empty == 1)
        score += 50000;
    else if (mine == 4)
        score = INT_MAX;
/* 
    else if (enemy == 2 && mine == 1)
        score -= 10; */
    else if (enemy == 2 && empty == 2)
        score -= 20;
    /*else if (enemy == 3 && mine == 1)
        score -= 500;*/
    else if (enemy == 3 && empty == 1)
        score -= 50000;
    else if (enemy == 4)
        score = INT_MIN;
}

int terminal_score(const vector<vector<int>> &map)
{
    int score = 0, empty = 42;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (map[i][j] != 0)  empty --;
            // horizontal
            if (i < 4)
            {
                int mine = 0, enemy = 0, empty = 0;
                for (int count = 0; count < 4; count++)
                {
                    if (map[i+count][j] == 0)  empty ++;
                    if (map[i+count][j] == 1)  mine ++;
                    if (map[i+count][j] == 2)  enemy ++;
                }
                add_score(score, mine, enemy, empty);
                if (score == INT_MAX || score == INT_MIN)  return score;
            }
            // vertical
            if (j < 3)
            {
                int mine = 0, enemy = 0, empty = 0;
                for (int count = 0; count < 4; count++)
                {
                    if (map[i][j+count] == 0)  empty ++;
                    if (map[i][j+count] == 1)  mine ++;
                    if (map[i][j+count] == 2)  enemy ++;
                }
                add_score(score, mine, enemy, empty);
                if (score == INT_MAX || score == INT_MIN)  return score;
            }
            // diagonals Right Up
            if (i < 4 && j < 3)
            {
                int mine = 0, enemy = 0, empty = 0;
                for (int count = 0; count < 4; count++)
                {
                    if (map[i+count][j+count] == 0)  empty ++;
                    if (map[i+count][j+count] == 1)  mine ++;
                    if (map[i+count][j+count] == 2)  enemy ++;
                }
                add_score(score, mine, enemy, empty);
                if (score == INT_MAX || score == INT_MIN)  return score;
            }
            // diagonals Left Up
            if (i > 2 && j < 3)
            {
                int mine = 0, enemy = 0, empty = 0;
                for (int count = 0; count < 4; count++)
                {
                    if (map[i-count][j+count] == 0)  empty ++;
                    if (map[i-count][j+count] == 1)  mine ++;
                    if (map[i-count][j+count] == 2)  enemy ++;
                }
                add_score(score, mine, enemy, empty);
                if (score == INT_MAX || score == INT_MIN)  return score;
            }
        }
    }
    
    if (empty == 0)  score = -1;
    return score;
}

vector<int> minimax(vector<vector<int>> map, int depth, int alpha, int beta, bool maxPlayer)
{
    int score = terminal_score(map);
    if (depth == 0 || score == -1 || score == INT_MAX || score == INT_MIN)
    {
        vector<int> ret(2);
        ret[0] = -1;
        ret[1] = score;
        return ret;
    }

    if (maxPlayer)    // maximizing player
    {
        int value = INT_MIN;
        int max_column = 0;
        for (int i = 0; i < 7; i++)
        {
            if (map[i][5] != 0)    // The column is full.
            {
                if (max_column == i)    max_column ++;
                continue;
            }
            int row;
            for (row = 0; row < 5; row ++)
                if (map[i][row] == 0)    break;
            map[i][row] = 1;
            vector<int> ret = minimax(map, depth-1, alpha, beta, false);
            if (ret[1] > value)
            {
                value = ret[1];
                max_column = i;
            }
            if (value > alpha)  alpha = value;
            if (alpha >= beta)  break;
            map[i][row] = 0;
        }
        return {max_column, value};
    }
    else    // minimizing player
    {
        int value = INT_MAX;
        int min_column = 0;
        for (int i = 0; i < 7; i++)
        {
            if (map[i][5] != 0)    // The column is full.
            {
                if (min_column == i)    min_column ++;
                continue;
            }
            int row;
            for (row = 0; row < 5; row ++)
                if (map[i][row] == 0)    break;
            map[i][row] = 2;
            vector<int> ret = minimax(map, depth-1, alpha, beta, true);
            if (ret[1] < value)
            {
                value = ret[1];
                min_column = i;
            }
            if (value < beta)  beta = value;
            if (alpha >= beta)  break;
            map[i][row] = 0;
        }
        return {min_column, value};
    }
}