/* See LICENSE file for copyright and license details. */

enum { ColFg, ColBg, ColBorder }; /* Clr scheme index */
typedef XftColor Clr;

/* Colorscheme abstraction */
void drw_clr_create(Drw *drw, Clr *dest, const char *clrname);
Clr *drw_scm_create(Drw *drw, const char *clrnames[], size_t clrcount);
