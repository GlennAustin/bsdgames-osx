#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/pig/Makefile,v 1.2.6.1 2001/04/25 09:29:00 ru Exp $
# $DragonFly: src/games/pig/Makefile,v 1.4 2006/10/08 16:22:36 pavalos Exp $

PROG=	pig
MAN=	pig.6

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
