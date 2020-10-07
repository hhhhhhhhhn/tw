tw:
	cc -lncursesw -O3 -o tw main.c
install: tw
	mv tw /usr/local/bin/tw
uninstall:
	rm /usr/local/bin/tw
