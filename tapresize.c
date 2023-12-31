/* Undefined in X11/X.h buttons that are actualy exist and correspond to
 * horizontal scroll
 */
#define Button6			6
#define Button7			7

/* mouse scroll resize */
static const int scrollsensetivity = 30; /* 1 means resize window by 1 pixel for each scroll event */

/* resizemousescroll direction argument list */
static const int scrollargs[][2] = {
	/* width change         height change */
	{ +scrollsensetivity,	0 },
	{ -scrollsensetivity,	0 },
	{ 0, 				  	+scrollsensetivity },
	{ 0, 					-scrollsensetivity },
};

static void resizemousescroll(const Arg *arg);

void
resizemousescroll(const Arg *arg)
{
	int nw, nh;
	Client *c;
	Monitor *m;
	XEvent ev;
	int dw = *((int*)arg->v + 1);
	int dh = *(int*)arg->v;

	if (!(c = selmon->sel))
		return;
	if (c->isfullscreen) /* no support resizing fullscreen windows by mouse */
		return;
	restack(selmon);
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurResize]->cursor, CurrentTime) != GrabSuccess)
		return;
	nw = MAX(c->w + dw, 1);
	nh = MAX(c->h + dh, 1);
	if (c->mon->wx + nw >= selmon->wx && c->mon->wx + nw <= selmon->wx + selmon->ww
	&& c->mon->wy + nh >= selmon->wy && c->mon->wy + nh <= selmon->wy + selmon->wh)
	{
		if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
		&& (abs(nw - c->w) > snap || abs(nh - c->h) > snap))
			togglefloating(NULL);
	}
    if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
        resize(c, c->x, c->y, nw, nh, 1);
        /* save last known float dimensions */
        c->sfx = c->x;
        c->sfy = c->y;
        c->sfw = nw;
        c->sfh = nh;
    }
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
}

