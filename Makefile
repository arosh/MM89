# -gを付けないとgperftoolsで関数名が見れなかった
# CXXFLAGS=-DLOCAL -D_GLIBCXX_DEBUG -std=c++11 -Wall -Wextra -Wshadow -g
# CXXFLAGS=-DLOCAL -DENABLE_PROFILE -std=c++11 -Wall -Wextra -Wshadow -O2 -g
CXXFLAGS=-DLOCAL -std=c++11 -Wall -Wextra -Wshadow -O2 -g
# CXXFLAGS=-DLOCAL -DNDEBUG -std=c++11 -Wall -Wextra -Wshadow -O2 -g
# LDLIBS=-lprofiler
