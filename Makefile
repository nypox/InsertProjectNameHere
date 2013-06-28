BDIR = ./build/
CDIR = ./core/

nctox: head core test
	gcc $(BDIR)nctest.o $(BDIR)DHT.o $(BDIR)network.o $(BDIR)log.o -lncurses -o $(BDIR)nctox

test:
	cp ./testing/nctest.c $(BDIR)
	gcc -c $(BDIR)nctest.c -o $(BDIR)nctest.o

core: log network DHT

DHT:
	cp $(CDIR)DHT.c $(BDIR)
	gcc -c $(BDIR)DHT.c -o $(BDIR)DHT.o
network:
	cp $(CDIR)network.c $(BDIR)
	gcc -c $(BDIR)network.c -o $(BDIR)network.o
log:
	cp $(CDIR)log.c $(BDIR)
	gcc -c $(BDIR)log.c -o $(BDIR)log.o
clean:
	rm $(BDIR)*

# This just copies the headers from core to build
head:
	cp $(CDIR)*.h $(BDIR)
