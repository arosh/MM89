#include <bits/stdc++.h>
using namespace std;
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)

struct MazeFixing {
  const vector<string> M;
  const int F;
  const int H, W;
  const int dy[4] = { 0, -1, 0, 1 };
  const int dx[4] = { 1, 0, -1, 0 };
  vector<vector<bool>> is_border;
  MazeFixing(vector<string> M_, int F_) : M(M_), F(F_), H(M_.size()), W(M_[0].size()) {}

  void Initialize() {
    is_border.assign(H, vector<bool>(W));
    REP(y, H) REP(x, W) {
      is_border[y][x] = false;
      if (M[y][x] == '.') {
        REP(i, 4) {
          if (is_outside(y + dy[i], x + dx[i]) == false) {
            is_border[y][x] = true;
          }
        }
      }
    }
  }

  bool is_outside(int y, int x) {
    if (y < 0 || y >= H || x < 0 || x >= W)
      return true;
    if (M[y][x] == '.')
      return true;
    return false;
  }

  int GetScore(vector<string> maze) {
    return -1;
  }

  vector<string> Improve() {
    Initialize();
    return vector<string>();
  }
};

vector<string> improve(vector<string> maze, int F) {
  return MazeFixing(maze, F).Improve();
}

int main() {
  // height: [10, 80]
  int H;
  cin >> H;
  // width: [10, 80]
  vector<string> maze(H);
  REP(i, H) cin >> maze[i];
  // fixes: [N/10, N/3], N is the number of non '.' cells in the maze
  int F;
  cin >> F;
  vector<string> ret = improve(maze, F);
  cout << ret.size() << endl;
  REP(i, ret.size()) {
    cout << ret[i] << endl;
  }
  cout.flush();
}
