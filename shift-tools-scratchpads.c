static Arg shift(const Arg *arg, int clients);
static void shiftboth(const Arg *arg);
static void shiftbothclients(const Arg *arg);
static void shiftswaptags(const Arg *arg);
// static void shifttag(const Arg *arg);
// static void shifttagclients(const Arg *arg);
static void shiftview(const Arg *arg);
static void shiftviewclients(const Arg *arg);
static void swaptags(const Arg *arg);


static Arg
shift(const Arg *arg, int clients)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	shifted.ui = selmon->tagset[selmon->seltags];

	for (c = selmon->clients; c && clients; c = c->next) {
		if (c == selmon->sel)
			continue;
		if (!(c->tags & SPTAGMASK))
			tagmask |= c->tags;
	}

	do {
		if (arg->i > 0) // left circular shift
			shifted.ui = (shifted.ui << arg->i) | (shifted.ui >> (LENGTH(tags) - arg->i));
		else // right circular shift
			shifted.ui = (shifted.ui >> -arg->i) | (shifted.ui << (LENGTH(tags) + arg->i));
		shifted.ui &= ~SPTAGMASK;
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

void
shiftbothclients(const Arg *arg)
{
	Arg shifted = shift(arg, 1);
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

// /* Sends a window to the next/prev tag */
// void
// shifttag(const Arg *arg)
// {
// 	Arg shifted = shift(arg, 0);
// 	tag(&shifted);
// }
//
// /* Sends a window to the next/prev tag that has a client, else it moves it to the next/prev one. */
// void
// shifttagclients(const Arg *arg)
// {
// 	Arg shifted = shift(arg, 1);
// 	tag(&shifted);
// }

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

void
swaptags(const Arg *arg)
{
	Client *c;
	unsigned int newtag = arg->ui & TAGMASK;
	unsigned int curtag = selmon->tagset[selmon->seltags];

	if (newtag == curtag || !curtag || (curtag & (curtag-1)))
		return;

	for (c = selmon->clients; c != NULL; c = c->next) {
		if ((c->tags & newtag) || (c->tags & curtag))
			c->tags ^= curtag ^ newtag;

		if (!c->tags)
            c->tags = newtag;
	}

	view(&((Arg) { .ui = newtag }));
}
