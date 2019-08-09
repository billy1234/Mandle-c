compile-java:
	javac src/Mandle.java -d out/

run-java:
	javac src/Mandle -d out
	java Mandle 10 10

compile-c:
	gcc src/mandle.c -o out/mandle -O3

run-c:
	gcc src/mandle.c -o out/mandle
	./out/mandle 10 10

compile-timer:
	gcc src/timer.c -o out/timer

compile-test:
	gcc src/test.c -o out/test

test:
	make compile-timer
	gcc src/test.c -o out/test
	./out/timer -c ./out/test



benchmark:
	make compile-timer
	make compile-c
	make compile-java
	./out/timer -c ./out/mandle
	./out/timer -c java -cp ./out Mandle

