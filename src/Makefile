# ======================= GLOSSARY ⊂(｡•́‿•̀｡⊃)
GCC = g++ -std=c++17 -Wall -Werror -Wextra -Wpedantic

LIB=s21_matrix_oop.a
SRC=*.cc
OBJ=*.o

TESTS=tests/*.cc
TESTN=test

TESTF=-lgtest -lgcov
GCOVF=--coverage 
HTML=lcov -t $(TESTN) -o rep.info -c -d ./

CLANG = clang-format -style=Google
LEAKS = leaks_check.log

# ======================= TARGETS ⊂(｡•́‿•̀｡⊃)
all: $(LIB)

$(LIB): 
	$(GCC) $(GCOVF) -c $(SRC) && ar rcs $(LIB) $(OBJ) && ranlib $(LIB)

test: clean $(LIB)
	$(GCC) -g $(TESTS) $(LIB) $(CFLAGS) $(TESTF) -o $(TESTN) && ./$(TESTN)

gcov_report: test
	$(HTML) && genhtml -o report rep.info && open report/index.html

clean:
	rm -rf *.o *.a *.so *.log *.gcda *.gcno *.gch *.html *.css *.dSYM $(TESTN) rep.info report 

# ======================= CHECKS ⊂(｡•́‿•̀｡⊃)
style:
	$(CLANG) -n *.cc *.h && $(CLANG) -i *.cc *.h

leaks: clean test
	CK_FORK=no leaks -atExit -- ./$(TESTN) $t > $(LEAKS) && open $(LEAKS)

valgrind: clean test
	valgrind --leak-check=full --show-leak-kinds=all --log-file=$(LEAKS) ./$(TESTN) && open $(LEAKS)