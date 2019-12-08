# 2019-2020
# Autores - Haohua Dong e Diogo Antunes

# compiler
CC = gcc
CFLAGS = -Wall -std=c99 -O3 
TARGET = campista

# objects (make automatically makes x.o from x.c)
OBJECTS = TentsSolver.o CAMPistA.o TreesTents.o generalStack.o


campista: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

debug: CFLAGS = -Wall -std=c99 -g 
debug: campista

valtest: 
	rm -f tests/*.tents
	@printf "Starting tests;\n"
	find tests/ -maxdepth 1 -name "*.camp"  -exec echo {} \; -exec valgrind -q --leak-check=full ./$(TARGET) {} \;
	@printf "done;\n"
test: 
	rm -f tests/*.tents
	@printf "Starting tests;\n"
	find tests/ -maxdepth 1 -name "*.camp"  -exec echo {} \; -exec ./$(TARGET) {} \;
	@printf "done;\n"

check:
	@printf "Starting checks;\n"
	find tests/ -maxdepth 1 -name "*.tents" -execdir echo Errors in {}: \; -execdir diff {} s/{} \;
	@printf "done;\n"

verify:
	@printf "Starting verifys;\n"
	find tests/ -maxdepth 1 -name "*.camp" -execdir printf "Verifying {}:\t\t\t" \; -execdir sh -c './ranger {}; echo $$?' \;
	@printf "done;\n"

testmem:
	reset
	rm -f massifreport
	valgrind  --stacks=yes --threshold=10.0 --massif-out-file=massifreport --tool=massif ./$(TARGET) test/boring27.camp 
	ms_print --threshold=10.0 massifreport

testtime:
	@printf "Starting time check;\n"
	time for n in {1..100}; do ./$(TARGET) tests/boring27.camp; done
	diff tests/boring27.tents tests/s/boring27.tents
	@printf "done;\n"


clean:
	rm -f *.o
	rm -f tests/*.tents
