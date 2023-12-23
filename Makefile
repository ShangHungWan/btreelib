COMPILER = g++
TARGET = btree.so
FLAGS = -Wall -Wextra -Werror -std=c++11 -O3 -fPIC -shared $$(python3 -m pybind11 --includes) -o $(TARGET)

main: src/main.cpp
	$(COMPILER) $(FLAGS) src/main.cpp

.PHONY: clean test
clean:
	rm -f $(TARGET)
test:
	make && python3 -m pytest