CC := gcc
CFLAGS := -Wall -std=c99 -Wno-missing-braces
INC = -I include/
LIB = -L libs/ -lraylib -lgdi32 -lwinmm

imgviewer: imgviewer.o
	$(CC) imgviewer.o imgres.res -Wl,--subsystem,windows -o imgviewer $(LIB)

imgviewer.o: imgviewer.c
	$(CC) -c imgviewer.c $(CFLAGS) $(INC)

imgres.res: imgres.rc
	windres imgres.rc -O coff -o imgres.res


clean:
	rm *.o
