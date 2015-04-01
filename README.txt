IST Semester Operating Systems programming assignment
=====================================================

This archive is  the "project starter kit" for the  IST-OPS course. It
contains a few scripts and files to get you started on the programming
assignment.

The web page for IST-OPS is located at:
http://moodle.insa-lyon.fr/course/view.php?id=1140#section-1

In case you have any question, don't hesitate to ask (both of) us:
- Lionel Morel <lionel.morel@insa-lyon.fr>
- Guillaume Salagnac <guillaume.salagnac@insa-lyon.fr>


Number generator
----------------

The project is about decomposing  large numbers into prime factors. We
provide you  with a  script which generates  an arbitraty  quantity of
"random"  numbers. Each  number is  scaled  down to  a certain  binary
magnitude (e.g. if you set this parameter to 10, then all your numbers
will  be smaller  than 1024).

The  script  also include  a  "redundancy"  parameter to  artificially
control the "variety" of the results. This feature will only be useful
in the last part of the project.


The script prints out numbers on its standard output, so you should
redirect that into a file:

    ./generator.cs 50 32 0 > fifty_distinct_smallish_numbers.txt

    ./generator.cs 10 64 100 > one_large_number_repeated_ten_times.txt


Program templates
-----------------

The files 'question1.c'  and 'question2.c' are templates  that you can
fill when working on the first questions of the program.

After that,  you will have  to create new  C files for  each exercise.
Don't overwrite your previous work with new programs, because you will
be require to compare successive versions of your work.

Makefile
--------

We provide you with  a simple Makefile for you to  build upon (add you
own rules along the way).

Here are the predefined targets:

- tiny.txt,  small.txt, medium.txt,  large.txt:  invoke the  generator
  with various parameters and save the resulting numbers in text files

- question1, question2: compile your C programs into an executable.

- run1, run2: execute your programs within the "time" command

- clean: delete generated files

Input Files
-----------

In  addition  to  the  number  generator, we  provide  you  with  some
hand-written number lists:

- primes.txt: contains only prime numbers of increasing value. You can
  use this file to check that your factorization algorithm is correct.

- tricky.txt:  contains various  arbitrary  numbers  (both primes  and
  non-primes)
