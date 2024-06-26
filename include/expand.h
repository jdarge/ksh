/*	$NetBSD: expand.h,v 1.7 2018/05/08 16:37:59 kamil Exp $	*/

/*
 * Expanding strings
 */
/* $Id: expand.h,v 1.7 2018/05/08 16:37:59 kamil Exp $ */

#define X_EXTRA 8       /* this many extra bytes in X string */

typedef struct XString {
    char *beg;          /* end, begin of string */
    char *end;
    size_t len;         /* length */
    Area *areap;        /* area to allocate/free from */
} XString;

//typedef char *XStringP;

/* initialize expandable string */
#define Xinit(xs, xp, length, area) do { \
            (xs).len = length; \
            (xs).areap = (area); \
            (xs).beg = alloc((xs).len + X_EXTRA, (xs).areap); \
            (xs).end = (xs).beg + (xs).len; \
            xp = (xs).beg; \
        } while (0)
/* __USE(xp); \ */

/* stuff char into string */
#define Xput(xs, xp, c) (*xp++ = (c))

/* check if there are at least n bytes left */
#define XcheckN(xs, xp, n) do { \
            int more = ((xp) + (n)) - (xs).end; \
            if (more > 0) \
            xp = Xcheck_grow_(&xs, xp, more); \
        } while (0)

/* check for overflow, expand string */
#define Xcheck(xs, xp) XcheckN(xs, xp, 1)

/* begin of string */
#define Xstring(xs, xp) ((xs).beg)
#define Xnleft(xs, xp) ((xs).end - (xp))        /* may be less than 0 */
#define Xlength(xs, xp) ((xp) - (xs).beg)
#define Xsavepos(xs, xp) ((xp) - (xs).beg)
#define Xrestpos(xs, xp, n) ((xs).beg + (n))

/* close, return string */
#define Xclose(xs, xp) (char*) aresize((void*)(xs).beg, (size_t)((xp) - (xs).beg), (xs).areap)

/* free string */
#define Xfree(xs, xp) afree((void*) (xs).beg, (xs).areap)

char *Xcheck_grow_ ARGS((XString * xsp, char *xp, int more)); /* todo: unused */

/*
 * expandable vector of generic pointers
 */

typedef struct XPtrV {
    void **cur;             /* next avail pointer */
    void **beg, **end;     /* begin, end of vector */
} XPtrV;

#define XPinit(x, n) do { \
            void **vp__; \
            vp__ = (void**) alloc(sizeofN(void*, n), ATEMP); \
            (x).cur = (x).beg = vp__; \
            (x).end = vp__ + n; \
        } while (0)

#define XPput(x, p) do { \
            if ((x).cur >= (x).end) { \
                int n = XPsize(x); \
                (x).beg = (void**) aresize((void*) (x).beg, sizeofN(void*, n*2), ATEMP); \
                (x).cur = (x).beg + n; \
                (x).end = (x).cur + n; \
            } \
            *(x).cur++ = (p); \
        } while (0)

#define XPptrv(x) ((x).beg)
#define XPsize(x) ((x).cur - (x).beg)
#define XPclose(x) (void**) aresize((void*)(x).beg, sizeofN(void*, XPsize(x)), ATEMP)
#define XPfree(x) afree((void*) (x).beg, ATEMP)
