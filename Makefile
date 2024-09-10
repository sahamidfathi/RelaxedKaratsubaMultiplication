CC=g++
#CC=clang++

a.out:*.cpp
	$(CC) *.cpp -o a.out -lgmp

clean:
	rm -f *.o *.out
