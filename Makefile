.PHONY: main Doxyfile html test judge clean
SRC_DIR = ./source
TOOLS_DIR = $(SRC_DIR)/tools
JUDGE_DIR = ./judge

all: main

Doxyfile:
	doxygen -g

html: Doxyfile $(SRC_DIR)/Voting.h $(SRC_DIR)/Voting.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/UnitTests.cpp
	doxygen Doxyfile

main: $(SRC_DIR)/Voting.h $(SRC_DIR)/Voting.cpp $(SRC_DIR)/main.cpp
	g++ -pedantic -O2 -std=c++11 -Wall $(filter %.cpp, $^) -o main

test: $(SRC_DIR)/Voting.h $(SRC_DIR)/Voting.cpp $(SRC_DIR)/UnitTests.cpp
	g++ -pedantic -O2 -std=c++11 -Wall $(filter %.cpp, $^) -o test -lgtest -lgtest_main -lpthread

judge: $(SRC_DIR)/main.cpp $(SRC_DIR)/Voting.cpp
	@mkdir -p judge
	@python $(TOOLS_DIR)/judge.py -s $(SRC_DIR)/Voting.cpp --header $(SRC_DIR)/Voting.h -o $(JUDGE_DIR)/Voting.cpp
	@grep -A500 "int main" $(SRC_DIR)/main.cpp >> ./judge/Voting.cpp

awk:
	python $(TOOLS_DIR)/judge.py -s $(SRC_DIR)/Voting.cpp --header $(SRC_DIR)/Voting.h -o temp.cpp

clean:
	rm -f main
	rm -f test
	rm -f -r ./judge
