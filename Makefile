build: Cards.c
	gcc Cards.c -o cards -g -Wall

run: cards
	./cards

clean:
	rm cards 