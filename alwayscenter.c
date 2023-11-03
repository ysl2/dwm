void
alwayscenter(Client *c)
{
	c->x = c->mon->mx + (c->mon->mw - WIDTH(c)) / 2;
	c->y = c->mon->my + (c->mon->mh - HEIGHT(c)) / 2;
}
