# Created by: Nicola Vitale <nivit@email.it>
# $FreeBSD$

PORTNAME=	truthtable
PORTVERSION=	1.2.2
PORTREVISION=	3
CATEGORIES=	math java
MASTER_SITES=	http://www.kinostudios.com/java/ \
		http://nivi.interfree.it/distfiles/${PORTNAME}/${PORTVERSION}/
DISTNAME=	TruthTable
EXTRACT_SUFX=	.jar
EXTRACT_ONLY=

MAINTAINER=	nivit@FreeBSD.org
COMMENT=	Truth Tables generator (written in Java)

BROKEN=		unfetchable
DEPRECATED=	Broken for more than 6 months
EXPIRATION_DATE=	2020-05-05

USE_JAVA=	yes
NO_BUILD=	yes

DATADIR=	${JAVASHAREDIR}/${PORTNAME}
SUB_FILES=	${PORTNAME}
SUB_LIST=	TRUTHTABLEJAR=${TRUTHTABLEJAR}

TRUTHTABLEJAR=	${DATADIR}/${DISTNAME}${EXTRACT_SUFX}

do-install:
	${MKDIR} ${STAGEDIR}${DATADIR}
	${INSTALL_DATA} ${DISTDIR}/${DISTFILES} ${STAGEDIR}${DATADIR}
	${INSTALL_SCRIPT} ${WRKDIR}/${PORTNAME} ${STAGEDIR}${PREFIX}/bin/${PORTNAME}

.include <bsd.port.mk>
