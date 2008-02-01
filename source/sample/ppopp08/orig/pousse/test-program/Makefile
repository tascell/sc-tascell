CC = gcc
CFLAGS = -Wall -g

SYSLIB=-L/usr/X11R6/lib -lX11 -lm
LIBS=-L/home/drd/xforms/FORMS -lforms $(SYSLIB)


test-pousse: test-pousse.c
	$(CC) $(CFLAGS) test-pousse.c -o $@

xpousse: xpousse.c
	$(CC) $(CFLAGS) xpousse.c -o $@ $(LIBS)
