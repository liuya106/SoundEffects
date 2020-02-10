all : sanitycheck

sanitycheck: effects
	@/bin/bash -c "cmp <(xxd -g1 crow.wav | ./effects -fin 0 | xxd -r) <crow.wav \
	  && echo Sanity check passed. \
	  || echo Sanity check failed."

effects : effects.c
	gcc -Wall -Werror -fsanitize=address -g -o effects effects.c

clean:
	rm -f *.o effects
