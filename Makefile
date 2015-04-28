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
	gcc -Wall -pthread -o question2 question2.c -lm

question3: question3.c
	gcc -Wall -pthread -o question3 question3.c -lm

question4: question4.c
	gcc -Wall -pthread -o question4 question4.c -lm	

question7: question7.c
	gcc -Wall -pthread -o question7 question7.c -lm	
	
question8: question8.c
	gcc -Wall -pthread -o question8 question8.c -lm	
	
question10: question10.c
	gcc -Wall -pthread -o question10 question10.c -lm

course: course.c
	gcc -Wall -pthread -o course course.c -lm

courseV2: courseV2.c
	gcc -Wall -pthread -o courseV2 courseV2.c -lm

##Mise en evidence efficacite des threads
question4efficace: question4efficace.c
	gcc -Wall -pthread -o question4efficace question4efficace.c -lm

question2efficace: question2efficace.c
	gcc -Wall -pthread -o question2efficace question2efficace.c -lm

question4pasEfficace: question4pasEfficace.c
	gcc -Wall -pthread -o question4pasEfficace question4pasEfficace.c -lm

question2pasEfficace: question2pasEfficace.c
	gcc -Wall -pthread -o question2pasEfficace question2pasEfficace.c -lm	


#########################
## program execution

run1: question1
	time ./question1

run2: question2
	time ./question2

run3: question3
	time ./question3

run4: question4
	time ./question4

run4efficace: question4efficace question2pasEfficace
	time ./question4efficace
	time ./question2pasEfficace

run4pasEfficace: question4pasEfficace question2efficace
	time ./question4pasEfficace
	time ./question2efficace

run7: question7
	time ./question7

run8: question8
	time ./question8

run10: question10
	time ./question10

run11: course
	time ./course

run12: courseV2
	time ./courseV2

#########################
## utilities

clean:
	rm -f question1 question2 question3 question4 question5 question7 question8 question10 course courseV2 tiny.txt small.txt medium.txt large.txt many.txt 

