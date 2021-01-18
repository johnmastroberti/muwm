# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

MUWM_SRC = src/colors.c src/cursor.c src/ipc.c src/muwm.c src/util.c src/cmd.c
MUWM_OBJ = $(MUWM_SRC:src/%.c=obj/%.o)

MUCTL_SRC = src/ipc.c src/muctl.c src/util.c
MUCTL_OBJ = $(MUCTL_SRC:src/%.c=obj/%.o)


.EXTRA_PREREQS = Makefile config.mk

all: options muwm muctl

options:
	@echo muwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

obj/%.o: src/%.c
	${CC} -o $@ ${CFLAGS} -c $<

obj:
	mkdir -p obj

${MUWM_OBJ}: obj
${MUCTL_OBJ}: obj

muwm: ${MUWM_OBJ}
	${CC} -o $@ ${MUWM_OBJ} ${LDFLAGS}

muctl: ${MUCTL_OBJ}
	${CC} -o $@ ${MUCTL_OBJ} ${LDFLAGS}

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
	cp -f muctl ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/muwm
	chmod 755 ${DESTDIR}${PREFIX}/bin/muctl
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < muwm.1 > ${DESTDIR}${MANPREFIX}/man1/muwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/muwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/muwm\
		${DESTDIR}${PREFIX}/bin/muctl\
		${DESTDIR}${MANPREFIX}/man1/muwm.1


colors.o: src/colors.c include/util.h include/colors.h
cursor.o: src/cursor.c include/cursor.h
ipc.o: src/ipc.c include/ipc.h include/util.h
muctl.o: src/muctl.c include/ipc.h include/util.h
muwm.o: src/muwm.c include/colors.h include/cursor.h include/util.h \
 include/muwm.h include/config.h
util.o: src/util.c include/util.h
cmd.o: src/cmd.c include/cmd.h include/muwm.h include/ipc.h

.PHONY: all options clean dist install uninstall
