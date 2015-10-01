CC = gcc
LIBS = -L/usr/lib -lc
CFLAGS = -std=c99 -pedantic

all: options lurk prod

options:
	@echo build options:
	@echo "CC = $(CC) $(CFLAGS)"

lurk:
	@echo CC -o $@
	@$(CC) -o $@ $@.c $(LIBS) $(CFLAGS)
	
prod:
	@echo CC -o $@
	$(CC) -o $@ $@.c $(LIBS) $(CFLAGS)

clean:
	@echo cleaning
	@rm -f lurk prod $(OBJ)
