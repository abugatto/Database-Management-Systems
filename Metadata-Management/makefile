# cmake_minimum_required(VERSION 3.10)

# # set the project name and version
# project(Tutorial VERSION 0.1)

# #configure heaser
# configure_file(runDBMS.cpp DBMS.h)

# #add directory to list of paths in search
# target_include_directories(DBMS PUBLIC
#                            "${/Users/anthonybugatto/Desktop/School/Fall\ 2019/Database\ Systems/Database-Management-Systems/Metadata-Management 
# }"
#                            )

#Makefile
CXX = g++
SRCFILES = runDBMS.cpp DBMS.cpp Database.cpp Relation.cpp Tuple.cpp
CXXFLAGS = -std=c++11 -Wall -g -pthread -O0 -o P1

all:
	$(CXX) $(SRCFILES) $(CXXFLAGS)


#FLAGS = 0 PA1_test.sql dbmsSave.txt dbmsMetrics.txt