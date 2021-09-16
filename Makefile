tw: cmdline.%
	cc -lncursesw -O3 -o tw cmdline.c main.c
cmdline.%: gengetopt.ggo
	gengetopt <gengetopt.ggo
install: tw
	mv tw /usr/local/bin/tw
uninstall:
	rm /usr/local/bin/tw
