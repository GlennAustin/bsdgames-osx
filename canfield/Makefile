#	@(#)Makefile	8.1 (Berkeley) 5/31/93

SUBDIR= canfield cfscores

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${VARDIR})
	mkdir -p ${VARDIR}
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.subdir.mk>
