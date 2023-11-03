SRC:=src/*.c
HDRS:=src/*.h
DEST:=nmc
CC:=cc
CFLAGS=-o $(DEST)

all: $(DEST)

$(DEST): $(SRC) $(HDRS)
		$(CC) $(CFLAGS) $(SRC)

clean:
		rm -rf $(DEST)