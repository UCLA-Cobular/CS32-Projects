cmake_minimum_required(VERSION 2.8.12.2)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

#add_executable(Project2 testSequence.cpp Sequence.cpp)
#add_executable(Project2 Sequence.cpp ScoreList.cpp testScoreList.cpp)
#add_executable(Project2 newSequence.cpp testnewSequence.cpp)
add_executable(Project2 newSequence.cpp testSequence.cpp)



#add_executable(Project2 testSequence.cpp Sequence.cpp Sequence.h ScoreList.cpp ScoreList.h)
#add_executable(Project2 testnewSequence.cpp ScoreList.cpp ScoreList.h newSequence.cpp newSequence.h)
#add_executable(Project2 testnewSequence.cpp ScoreList.cpp ScoreList.h newSequence.cpp newSequence.h)
