static Arg shift(const Arg *arg, int clients);
static void shiftboth(const Arg *arg);
static void shiftswaptags(const Arg *arg);
static void shifttag(const Arg *arg);
static void shifttagclients(const Arg *arg);
static void shiftview(const Arg *arg);
static void shiftviewclients(const Arg *arg);


static Arg
shift(const Arg *arg, int clients)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	shifted.ui = selmon->tagset[selmon->seltags];

	#if TAGSYNC_PATCH
	Monitor *origselmon = selmon;
	for (selmon = mons; selmon; selmon = selmon->next)
	#endif // TAGSYNC_PATCH
	for (c = selmon->clients; c && clients; c = c->next) {
		if (c == selmon->sel)
			continue;
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if (!(c->tags & SPTAGMASK))
			tagmask |= c->tags;
		#elif SCRATCHPAD_ALT_1_PATCH
		if (!(c->tags & SCRATCHPAD_MASK))
			tagmask |= c->tags;
		#else
		tagmask |= c->tags;
		#endif // SCRATCHPADS_PATCH
	}
	#if TAGSYNC_PATCH
	selmon = origselmon;
	#endif // TAGSYNC_PATCH

	do {
		if (arg->i > 0) // left circular shift
			shifted.ui = (shifted.ui << arg->i) | (shifted.ui >> (NUMTAGS - arg->i));
		else // right circular shift
			shifted.ui = (shifted.ui >> -arg->i) | (shifted.ui << (NUMTAGS + arg->i));
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		shifted.ui &= ~SPTAGMASK;
		#endif // SCRATCHPADS_PATCH
	} while (tagmask && !(shifted.ui & tagmask));

	return shifted;
}

void
shiftboth(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	tag(&shifted);
	view(&shifted);
}

/* swaps "tags" (all the clients) with the next/prev tag. */
void
shiftswaptags(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	swaptags(&shifted);
}

/* Sends a window to the next/prev tag */
void
shifttag(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	tag(&shifted);
}

/* Sends a window to the next/prev tag that has a client, else it moves it to the next/prev one. */
void
shifttagclients(const Arg *arg)
{
	Arg shifted = shift(arg, 1);
	tag(&shifted);
}

void
shiftview(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	view(&shifted);
}

void
shiftviewclients(const Arg *arg)
{
	Arg shifted = shift(arg, 1);
	view(&shifted);
}
