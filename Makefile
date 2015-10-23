# -gを付けないとgperftoolsで関数名が見れなかった
# CXXFLAGS=-D_GLIBCXX_DEBUG -std=c++11 -Wall -Wextra -Wshadow -g
# CXXFLAGS=-DENABLE_PROFILE -std=c++11 -Wall -Wextra -Wshadow -O2 -g
CXXFLAGS=-DLOCAL -std=c++11 -Wall -Wextra -Wshadow -O2 -g
# CXXFLAGS=-DNDEBUG -std=c++11 -Wall -Wextra -Wshadow -O2 -g
# LDLIBS=-lprofiler
