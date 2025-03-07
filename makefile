all: clean compile run

compile: main.cpp Piece.cpp Board.cpp
	@echo "-----------------------------------------"
	@echo "Compiling..."
	@g++ -std=c++11 -o output main.cpp Piece.cpp Board.cpp
	@echo "Compilation successful."

run:
	@echo "-----------------------------------------"
	@echo "Running the program..."
	@echo "======================================================================="
	./output
	@echo "======================================================================="
	@echo "Program completed."

clean:
	@echo "-----------------------------------------"
	@echo "Removing compiled files..."
	@rm -f *.o
	@rm -f output
	@echo "Removed compiled files."