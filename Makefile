CC=g++
SRC=src
INCLUDE=include
CFLAGS=-I$(INCLUDE) -lgmp

SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(SRCS:.cpp=.o)

a.out: $(OBJS)
	$(CC) $(OBJS) -o a.out $(CFLAGS)

%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(SRC)/*.o a.out

