default: help

help:
	@echo Useful targets:
	@echo "  small.txt medium.txt large.txt many.txt:  generate some input files "
	@echo "  question1 question2: compile your programs"
	@echo "  run1 run2:  run your programs through the 'time' utility"
	@echo "  clean:  delete all generated files"

#########################
# workload generation

tiny.txt:
	./generator.cs 20 20 0 > $@

small.txt:
	./generator.cs 20 32 50 > $@

medium.txt:
	./generator.cs 20 50 50 > $@

large.txt:
	./generator.cs 20 64 50 > $@

many.txt:
	./generator.cs 1000 50 75 > $@

#########################
## program compilation

question1: question1.c
	gcc -Wall -pthread -o question1 question1.c -lm

question2: question2.c
	gcc -Wall -pthread -o question2 question2.c 

# add your own rules when you create new programs

#########################
## program execution

run1: question1
	time ./question1

run2: question2
	time ./question2

#########################
## utilities

clean:
	rm -f question1 question2 tiny.txt small.txt medium.txt large.txt many.txt 

