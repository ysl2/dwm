void
alwayscenter(Client *c)
{
	// c->x = c->mon->mx + (c->mon->mw - WIDTH(c)) / 2;
	// c->y = c->mon->my + (c->mon->mh - HEIGHT(c)) / 2;

    c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
    c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
}
