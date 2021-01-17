# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = $(shell ls src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)

.EXTRA_PREREQS = Makefile config.mk

all: options muwm

options:
	@echo muwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

obj/%.o: src/%.c
	${CC} -o $@ ${CFLAGS} -c $<

obj:
	mkdir -p obj

${OBJ}: obj

include/config.h:
	cp include/config.def.h $@

muwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f muwm ${OBJ} muwm-${VERSION}.tar.gz

dist: clean
	mkdir -p muwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		muwm.1 drw.h util.h ${SRC} transient.c muwm-${VERSION}
	tar -cf muwm-${VERSION}.tar muwm-${VERSION}
	gzip muwm-${VERSION}.tar
	rm -rf muwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f muwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/muwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < muwm.1 > ${DESTDIR}${MANPREFIX}/man1/muwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/muwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/muwm\
		${DESTDIR}${MANPREFIX}/man1/muwm.1


colors.o: src/colors.c include/util.h include/colors.h
cursor.o: src/cursor.c include/cursor.h
muwm.o: src/muwm.c include/colors.h include/cursor.h include/util.h \
 include/config.h
util.o: src/util.c include/util.h

.PHONY: all options clean dist install uninstall
