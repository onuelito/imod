IMOD_LOCATION=../../..

IMODGP=$(IMOD_LOCATION)/graphics

LIBDIR=lib

LDPATHS=\
	-L /usr/local/lib \
	-L $(LIBDIR)

LDFLAGS=-limodgraphics -lGLESv2 -lm

INCLUDE=\
	-I /usr/local/include \
	-I $(IMOD_LOCATION)/include \

SOURCES=\
	main.c \

BIN_NAME=bin

all: libs
	$(CC) $(SOURCES) -o $(BIN_NAME) -Wl,-rpath=$(LIBDIR) $(LDPATHS) $(INCLUDE) $(LDFLAGS)

libs:
	mkdir -p $(LIBDIR)
	make -C $(IMODGP) 
	cp -rf $(IMODGP)/lib/* $(LIBDIR)


clean:
	rm -rf $(LIBDIR)
	rm -rf $(BIN_NAME)
