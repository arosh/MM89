#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <limits>
#include <stack>
#include <sys/time.h>
#ifdef ENABLE_PROFILE
#include <gperftools/profiler.h>
#endif
using namespace std;
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
#define DEBUG(x) cerr << #x << " = " << x << endl

double gettime() {
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

struct Action {
  int y, x, d, sgn;
  Action() {}
  Action(int y_, int x_, int d_, int sgn_) : y(y_), x(x_), d(d_), sgn(sgn_) {}
};

struct MazeFixing {
  const vector<string> M;
  const int F;
  const int H, W;
  const int dy[4] = { 0, -1, 0, 1 };
  const int dx[4] = { 1, 0, -1, 0 };
  vector<vector<bool>> is_border;
  vector<vector<bool>> visited_one_path;
  vector<vector<bool>> visited_over_all;
  vector<vector<bool>> visited_any_path;
  MazeFixing(vector<string> M_, int F_) : M(M_), F(F_), H(M_.size()), W(M_[0].size()) {}

  bool IsOutside(int y, int x) {
    if (y < 0 || y >= H || x < 0 || x >= W)
      return true;
    if (M[y][x] == '.')
      return true;
    return false;
  }

  void SetUpBorder() {
    is_border.assign(H, vector<bool>(W, false));
    REP(y, H) {
      REP(x, W) {
        if (M[y][x] == '.') {
          REP(i, 4) {
            if (IsOutside(y + dy[i], x + dx[i]) == false) {
              is_border[y][x] = true;
            }
          }
        }
      }
    }
  }

  void FillVisitedOverAll() {
    REP(y, H) {
      REP(x, W) {
        visited_over_all[y][x] = visited_over_all[y][x] || visited_one_path[y][x];
      }
    }
  }

  void Follow(const vector<string> &maze, int start_y, int start_x, int start_d) {
    stack<Action> stk;
    stk.emplace(start_y, start_x, start_d, 1);
    while (!stk.empty()) {
      Action cur = stk.top();
      stk.pop();
      int y = cur.y;
      int x = cur.x;
      int d = cur.d;
      if (cur.sgn > 0) {
        if (visited_one_path[y][x]) {
          // check loop
          continue;
        }
        stk.emplace(y, x, d, -1);
        visited_one_path[y][x] = true;
        visited_any_path[y][x] = true;
        if (is_border[y][x]) {
          // goal
          FillVisitedOverAll();
        } else {
          assert(IsOutside(y, x) == false);
          int nd = -1;
          REP(i, 4) {
            if ("SLUR"[i] == maze[y][x])
              nd = (d + i) % 4;
          }
          if (nd != -1) {
            stk.emplace(y + dy[nd], x + dx[nd], nd, 1);
          } else if (maze[y][x] == 'E') {
            REP(i, 4) {
              stk.emplace(y + dy[i], x + dx[i], i, 1);
            }
          } else {
            assert(false);
          }
        }
      } else {
        visited_one_path[y][x] = false;
      }
    }
  }

  void Search(const vector<string> &maze) {
    visited_one_path.assign(H, vector<bool>(W, false));
    visited_over_all.assign(H, vector<bool>(W, false));
    visited_any_path.assign(H, vector<bool>(W, false));
    REP(y, H) {
      REP(x, W) {
        if (is_border[y][x]) {
          REP(i, 4) {
            if (IsOutside(y + dy[i], x + dx[i]) == false && M[y + dy[i]][x + dx[i]] != '.') {
              Follow(maze, y + dy[i], x + dx[i], i);
            }
          }
        }
      }
    }
  }

  int GetScore(const vector<string> &maze) {
    Search(maze);
    int c = 0;
    REP(y, H) {
      REP(x, W) {
        if (visited_over_all[y][x])
          ++c;
      }
    }
    return c;
  }

  int GetScorePlusCanMove(const vector<string> &maze) {
    int c = GetScore(maze);
    REP(y, H) {
      REP(x, W) {
        if (visited_any_path[y][x])
          ++c;
      }
    }
    return c;
  }

  int CountFixedCell(const vector<string> &maze) {
    int fixed = 0;
    REP(y, H) {
      REP(x, W) {
        if (maze[y][x] != M[y][x]) ++fixed;
      }
    }
    return fixed;
  }

  void CreateCandidatePoint(
      const vector<string> &status,
      const vector<vector<bool>> &visited,
      const vector<vector<bool>> &pointed,
      vector<int> &ys,
      vector<int> &xs) {
    assert(ys.empty());
    assert(xs.empty());
    REP(y, H) {
      REP(x, W) {
        if(status[y][x] != '.' &&
           status[y][x] != 'E' &&
           visited[y][x]) {
          REP(i, 4) {
            if(IsOutside(y + dy[i], x + dx[i]) == false && pointed[y+dy[i]][x+dx[i]] == false) {
              ys.push_back(y);
              xs.push_back(x);
            }
          }
        }
      }
    }
  }

  vector<string> Optimize() {
    double time_start = gettime();
    double time_use = 9.0;
    double time_end = time_start + time_use;
    double time_current;

    vector<string> best_status = M;
    int best_score = GetScore(best_status);
    vector<string> current_status = best_status;
    int current_score = best_score;
    vector<vector<bool>> current_visited = visited_any_path;
    vector<vector<bool>> current_pointed = visited_over_all;

    while ((time_current = gettime()) < time_end) {
      vector<int> ys, xs;
      CreateCandidatePoint(current_status, current_visited, current_pointed, ys, xs);
      assert(xs.size() == ys.size());
      int rand_index = xrand() % ys.size();
      int move_y = ys[rand_index];
      int move_x = xs[rand_index];
      vector<string> next_status = current_status;
      char move_d = "SLUR"[xrand() % 4];
      next_status[move_y][move_x] = move_d;
      if (CountFixedCell(next_status) > F)
        continue;
      int next_score = GetScorePlusCanMove(next_status);
      if (best_score < next_score) {
        best_score = next_score;
        best_status = next_status;
        cerr << "[" << time_current - time_start << "] best_score = " << best_score << endl;
      }
      double prob = 1.0 * xrand() / numeric_limits<unsigned long>::max();
      bool force_next = time_use * prob < (time_end - time_current);
      if (current_score < next_score || force_next) {
        current_score = next_score;
        current_status = next_status;
        current_visited = visited_any_path;
        current_pointed = visited_over_all;
        // cerr << "[" << time_current << "] current_score = " << current_score << endl;
      }
    }

    vector<string> ret;
    REP(y, H) {
      REP(x, W) {
        if (best_status[y][x] != M[y][x]) {
          ret.push_back(to_string(y) + " " + to_string(x) + " " + best_status[y][x]);
        }
      }
    }
    DEBUG(ret.size());
    DEBUG(F);
    assert((int)ret.size() <= F);
    return ret;
  }

  vector<string> Improve() {
    SetUpBorder();
    return Optimize();
  }
};

vector<string> improve(vector<string> maze, int F) {
#ifdef ENABLE_PROFILE
  ProfilerStart("./pprof.out");
#endif
  return MazeFixing(maze, F).Improve();
#ifdef ENABLE_PROFILE
  ProfilerStop();
#endif
}

int main() {
  // height: [10, 80]
  int H;
  cin >> H;
  // width: [10, 80]
  vector<string> maze(H);
  REP(i, H) {
    cin >> maze[i];
  }
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
