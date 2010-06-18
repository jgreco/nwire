CC = gcc
#modify the CFLAGS variable as needed.  specifically you will probably need to modify the include directory arguments
CFLAGS = -O2 -Wall -Wextra -Wno-unused-parameter -pedantic -pipe -g
LIBS = -lncurses
OBJDIR = .build
OBJECTS = nwire.o map_2d.o
OBJECTS :=  $(addprefix ${OBJDIR}/,${OBJECTS})

nwire: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o nwire

${OBJDIR}/%.o : %.c
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi #create directory if it doesn't exist
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) nwire
