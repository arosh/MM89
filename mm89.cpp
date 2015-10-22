#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <limits>
#include <sys/time.h>
using namespace std;
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
#define DEBUG(x) cerr << #x << " = " << x << endl

double GetTime() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

unsigned long xrand() {
  static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
  unsigned long t;
  t = (x ^ (x << 11));
  x = y;
  y = z;
  z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

struct MazeFixing {
  const vector<string> M;
  const int F;
  const int H, W;
  const int dy[4] = { 0, -1, 0, 1 };
  const int dx[4] = { 1, 0, -1, 0 };
  vector<vector<bool>> is_border;
  vector<vector<bool>> visited_one_path;
  vector<vector<bool>> visited_over_all;
  MazeFixing(vector<string> M_, int F_) : M(M_), F(F_), H(M_.size()), W(M_[0].size()) {}

  bool IsOutside(int y, int x) {
    if (y < 0 || y >= H || x < 0 || x >= W)
      return true;
    if (M[y][x] == '.')
      return true;
    return false;
  }

  void Initialize() {
    is_border.assign(H, vector<bool>(W, false));
    REP(y, H) REP(x, W) {
      if (M[y][x] == '.') {
        REP(i, 4) {
          if (IsOutside(y + dy[i], x + dx[i]) == false) {
            is_border[y][x] = true;
          }
        }
      }
    }
  }

  void FillVisitedOverAll() {
    REP(y, H) REP(x, W) {
      visited_over_all[y][x] = visited_over_all[y][x] || visited_one_path[y][x];
    }
  }

  void Follow(const vector<string> &maze, int y, int x, int d) {
    if (visited_one_path[y][x]) {
      // check loop
      return;
    }
    if (IsOutside(y, x) == false) {
      visited_one_path[y][x] = true;
    }
    if (is_border[y][x]) {
      // goal
      FillVisitedOverAll();
    } else {
      if (maze[y][x] == 'S') {
        int nd = (d + 0) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'L') {
        int nd = (d + 1) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'U') {
        int nd = (d + 2) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'R') {
        int nd = (d + 3) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'E') {
        REP(nd, 4) {
          Follow(maze, y + dy[nd], x + dx[nd], nd);
        }
      } else {
        assert(false);
      }
    }
    visited_one_path[y][x] = false;
  }

  int GetScore(const vector<string> &maze) {
    visited_one_path.assign(H, vector<bool>(W, false));
    visited_over_all.assign(H, vector<bool>(W, false));
    REP(y, H) REP(x, W) {
      if (is_border[y][x]) {
        REP(i, 4) {
          if (IsOutside(y + dy[i], x + dx[i]) == false && M[y + dy[i]][x + dx[i]] != '.') {
            Follow(maze, y + dy[i], x + dx[i], i);
          }
        }
      }
    }
    int c = 0;
    REP(y, H) REP(x, W) {
      if (visited_over_all[y][x])
        ++c;
    }
    return c;
  }

  vector<string> Optimize() {
    double time_start = GetTime();
    double time_use = 9.0;
    double time_end = time_start + time_use;
    double time_current;

    vector<string> best_status = M;
    int best_score = GetScore(best_status);
    vector<string> current_status = best_status;
    int current_score = best_score;

    while ((time_current = GetTime()) < time_end) {
      int move_y = xrand() % H;
      int move_x = xrand() % W;
      if(current_status[move_y][move_x] == '.' || current_status[move_y][move_x] == 'E') continue;
      vector<string> next_status = current_status;
      char move_d = "SLUR"[xrand() % 4];
      next_status[move_y][move_x] = move_d;
      char c = 0;
      REP(y,H) REP(x,W) if(next_status[y][x] != M[y][x]) ++c;
      if(c > F) continue;
      int next_score = GetScore(next_status);
      if(best_score < next_score) {
        best_score = next_score;
        best_status = next_status;
        cerr << "[" << time_current - time_start << "] best_score = " << best_score << endl;
      }
      double prob = 1.0 * xrand() / numeric_limits<unsigned long>::max();
      bool force_next = time_use * prob < (time_end - time_current);
      if(current_score < next_score || force_next) {
        current_score = next_score;
        current_status = next_status;
        // cerr << "[" << time_current << "] current_score = " << current_score << endl;
      }
    }

    vector<string> ret;
    REP(y,H) REP(x,W) {
      if(best_status[y][x] != M[y][x]) {
        ret.push_back(to_string(y) + " " + to_string(x) + " " + best_status[y][x]);
      }
    }
    return ret;
  }

  vector<string> Improve() {
    Initialize();
    return Optimize();
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
