# Makefile for nmake - Suppress .lib and .exp output

CC=cl
CFLAGS=/nologo /I include
OBJS=utils.obj comms.obj main.obj 
OUT=implant.exe

all: $(OUT)
	@del /q *.obj *.lib *.exp

$(OUT): $(OBJS)
	$(CC) $(OBJS) /Fe$(OUT) /link /NOLOGO /SUBSYSTEM:WINDOWS 

main.obj: src\main.c include\utils.h
	$(CC) $(CFLAGS) /c src\main.c

utils.obj: src\utils.c include\utils.h
	$(CC) $(CFLAGS) /c src\utils.c

comms.obj: src\comms.c include\comms.h
	$(CC) $(CFLAGS) /c src\comms.c

clean:
	@del /q *.obj *.exe *.lib *.exp
