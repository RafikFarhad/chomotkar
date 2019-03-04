MAKEFILE = Makefile

# We use ${HOME}, you can use /usr, /usr/local, etc. Note ${BINDIR} below
BASDIR  = /usr/local

LDFLAGS = -lcurl

OWNER   = ${LOGNAME}

GROUP   = ${GRPNAME}

BINDIR  = $(BASDIR)/bin

BMASK   = 0755

MAINS   = chomotkar

OBJECTS = main.o downloadHelper.o

SOURCES = main.c downloadHelper.c

ALL: $(MAINS)

chomotkar: main.o downloadHelper.o $(LIBRARY)
		$(CC) $(CFLAGS) -s -o chomotkar main.o downloadHelper.o $(LDFLAGS)


install:		$(MAINS)
# cp $(MAINS) $(BINDIR)
# cd $(BINDIR);\
# chown $(OWNER) $(MAINS);\
# chmod $(BMASK) $(MAINS)

remove:
		cd $(BINDIR);\
		rm -f $(MAINS)

clean:
		rm -f $(OBJECTS)

clobber:
		rm -f $(OBJECTS) $(MAINS)
# BINDIR := /usr/bin

# all:
# 	gcc -o chomotkar main.c downloadHelper.c -lcurl

# install:
# 	cp chomotkar ${BINDIR}/