HEADER= Bem.h map.h Person.h
Snake: $(HEADER)
	gcc Bem.c map.c Person.c Snake.c -lcurses -o Snake

.PHONY : clean
clean:
	@rm -f Snake
