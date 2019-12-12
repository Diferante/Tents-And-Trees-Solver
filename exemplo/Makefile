 #*****************************************************************************
 #  Copyright (C) 2019 - Haohua Dong e Diogo Antunes
 #
 #  This file is a part of Trees-And-Tents-Solver.
 #
 #  Trees-And-Tents-Solver is free software: you can redistribute it and/or
 #  modify it under the terms of the GNU General Public License as published by
 #  the Free Software Foundation, either version 3 of the License, or
 #  (at your option) any later version.
 #
 #  Trees-And-Tents-Solver is distributed in the hope that it will be useful,
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #  GNU General Public License for more details.
 #
 #  You should have received a copy of the GNU General Public License
 #  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 #
 #*****************************************************************************

# compiler
CC = gcc
CFLAGS = -Wall -std=c99 -O3 
LIB_PATH=../src
TARGET = campista

# objects (generalStack is in it's own repository)
OBJECTS = exemplo.o $(LIB_PATH)/TentsSolver.o $(LIB_PATH)/TreesTents.o $(LIB_PATH)/generalStack.o


campista: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

debug: CFLAGS = -Wall -std=c99 -g 
debug: campista

valtest: 
	rm -f tests/*.tents
	@printf "Starting tests;\n"
	find tests -maxdepth 1 -name "*.camp"  -exec echo {} \; -exec valgrind -q --leak-check=full ./$(TARGET) {} \;
	@printf "done;\n"
test: 
	rm -f tests/*.tents
	@printf "Starting tests;\n"
	find tests -maxdepth 1 -name "*.camp"  -exec echo {} \; -exec ./$(TARGET) {} \;
	@printf "done;\n"

check:
	@printf "Starting checks;\n"
	find tests -maxdepth 1 -name "*.tents" -execdir echo Errors in {}: \; -execdir diff {} s/{} \;
	@printf "done;\n"

testmem:
	reset
	rm -f massifreport
	valgrind  --stacks=yes --threshold=10.0 --massif-out-file=massifreport --tool=massif ./$(TARGET) test/boring27.camp 
	ms_print --threshold=10.0 massifreport


clean:
	rm -f *.o
	rm -f tests/*.tents
