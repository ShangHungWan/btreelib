COMPILER = g++
TARGET = btreelib.so
FLAGS = -Wall -Wextra -Werror -std=c++11 -O3 -fPIC -shared $$(python3 -m pybind11 --includes) -o $(TARGET)

main: src/main.cpp
	$(COMPILER) $(FLAGS) src/main.cpp

.PHONY: clean test performance
clean:
	rm -f $(TARGET)
test:
	make && python3 -m pytest
performance:
	make && mv $(TARGET) tests/ && cd tests && python3 performance.py && rm $(TARGET)