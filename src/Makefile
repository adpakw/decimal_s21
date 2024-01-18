GCC = gcc
CFLAGS = -c -Wall -Werror -Wextra -std=c11
LDFLAGS = -O2 -Wall -Werror -Wextra -std=c11
SOURCE = $(wildcard s21_*.c)
HEADER = $(wildcard s21_*.h)
OBJECTS = $(patsubst %.c,%.o, ${SOURCE})
TESTS_SOURCE = tests.c
HELPER_SOURCE = s21_decimal_helper.c
comparison_source = s21_decimal_comparison.c
ARITHMETIC_SOURCE = s21_decimal_arithmetics.c 
CAST_SOURCE = s21_cast.c
TESTS_OBJECTS = $(patsubst %.c,%.o, ${TESTS_SOURCE})
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
	RFLAGS = -lcheck -lsubunit -lm -lrt -lpthread -DLinux
	BREW = .
endif
ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	ADD_LIB =
	RFLAGS = -lcheck -lm -DMacos
	BREW := ${HOME}/homebrew
endif

PATH := $(BREW)/bin:$(PATH)

all: clean s21_decimal.a

s21_decimal.a: ${SOURCE}
	$(CC) $(CFLAGS) $(ADD_LIB) $(SOURCE) 
	ar rc $@ $(OBJECTS)
	ranlib $@
	cp $@ lib$@

test: ${TESTS_SOURCE} s21_decimal.a
	make clang
	${CC} $(LDFLAGS) -o test $^ $(RFLAGS)
	./test
	
helper:$(HELPER_SOURCE) 
	make clang
	${CC} $(LDFLAGS) -o help $^ $(RFLAGS)
	./help

comparison:$(comparison_source) $(HELPER_SOURCE)
	make clang
	${CC} $(LDFLAGS) -o comp $^ $(RFLAGS)
	./comp

ari:$(ARITHMETIC_SOURCE) $(HELPER_SOURCE) $(CAST_SOURCE)
	# make clang
	clang-format -i *.c *.h
	${CC} $(LDFLAGS) -o ari $^ $(RFLAGS)
	./ari

gcov_report: clean ${SOURCE}
	gcc --coverage ${SOURCE} tests.c -o s21_test ${RFLAGS}
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html

lcov:
ifeq ("", "$(shell PATH=$(PATH) which lcov)")
	$(error Need to install lcov)
endif

clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h
	rm -rf .clang-format

clean:
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/
	-rm -rf s21_test
	-rm -rf sprintf
	-rm -rf *.out
	-rm -rf help comp
	rm -rf ari

.PHONY: all clean lcov
