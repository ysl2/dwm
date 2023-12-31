/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int savefloats         = 1;        /* 0 means default behaviour, 1 = savefloats patch */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "monospace:size=13" };
static const char dmenufont[]       = "monospace:size=13";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_cyan  },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
// const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
// const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd1[] = {"alacritty", "--class", "spterm", NULL };
const char *spcmd2[] = {"alacritty", "--class", "spfm", "-e", "lf", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spfm",        spcmd2},
	{"keepassxc",   spcmd3},
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance       title       tags mask   isfloating  isterminal  noswallow     monitor */
	{ "Gimp",	  NULL,			NULL,		0,				1,		   0,           0,        	-1 },
	{ "Firefox",  NULL,			NULL,		1 << 8,			0,		   0,           -1,        	-1 },
	{ "St",       NULL,         NULL,       0,              0,         1,           0,          -1 },
	{ NULL,       NULL,     "Event Tester", 0,              0,         0,           1,          -1 }, /* xev */
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,		   1,	        0,          -1 },
	{ NULL,		  "spfm",		NULL,		SPTAG(1),		1,		   1,           0,          -1 },
	{ NULL,		  "keepassxc",	NULL,		SPTAG(2),		0,		   1,           0,          -1 },
	{ "Alacritty", NULL,        NULL,       0,              0,         1,           0,          -1 },
	{ "Clash for Windows", NULL, NULL,      0,              1,         0,           0,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ Mod1Mask,                     KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ Mod1Mask|ShiftMask,           KEY,      toggletag,      {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask|ControlMask, KEY,      swaptags,       {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

#include "movestack.c"
#include "shift-tools-scratchpads.c"
#include "bulkill.c"
#include "exitdwm.c"
#include "inplacerotate.c"
#include "cyclelayouts.c"
#include "fullscreen.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_o,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_bracketleft,  setmfact, {.f = -0.05} },
	{ MODKEY,                       XK_bracketright, setmfact, {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

    // movestack
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

    // cyclelayouts
	{ MODKEY|ControlMask,		    XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },

    // cfacts-vanitygaps
	{ MODKEY,                       XK_minus,     setcfact,    {.f = -0.25} },
	{ MODKEY,                       XK_equal,     setcfact,    {.f = +0.25} },
	// { MODKEY,                       XK_backslash, setcfact,    {.f =  0.00} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_b,      setlayout,      {.v = &layouts[6]} },

    // scratch
	{ MODKEY,            			XK_y,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_u,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            			XK_x,	   togglescratch,  {.ui = 2 } },

    // shif-tools
	{ MODKEY,	                    XK_p,      shiftviewclients, { .i = -1 } },
	{ MODKEY,                       XK_n,      shiftviewclients, { .i = +1 } },
	{ MODKEY,                       XK_h,	   shiftview,        { .i = -1 } },
	{ MODKEY,                       XK_l,	   shiftview,        { .i = +1 } },
	{ MODKEY|ShiftMask,		        XK_p,      shiftbothclients, { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_n,      shiftbothclients, { .i = +1 } },
	{ MODKEY|ShiftMask,		        XK_h,      shiftboth,        { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      shiftboth,        { .i = +1 } },
	{ MODKEY|ShiftMask|ControlMask,	XK_h,      shiftswaptags,    { .i = -1 } },
	{ MODKEY|ShiftMask|ControlMask,	XK_l,      shiftswaptags,    { .i = +1 } },

    // awesomebar
	{ MODKEY,                       XK_j,      focusstackvis,  {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstackvis,  {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,      focusstackhid,  {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,      focusstackhid,  {.i = -1 } },
	{ MODKEY|ControlMask,           XK_s,      show,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_s,      showall,        {0} },
	{ MODKEY|ControlMask,           XK_h,      hide,           {0} },

    // fullscreen
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },

    // sticky
    { MODKEY,                       XK_s,      togglesticky,    {0} },

    //bulkill-systray-safe
    { MODKEY|ControlMask,           XK_c,      bulkill,        {.ui = 1} },  // kill unselect (except sticky)
    { MODKEY|ShiftMask|ControlMask, XK_c,      bulkill,        {.ui = 2} },  // killall (except sticky)
    { MODKEY|ControlMask,           XK_z,      bulkill,        {.ui = 3} },  // kill unselect
    { MODKEY|ShiftMask|ControlMask, XK_z,      bulkill,        {.ui = 4} },  // killall

    // exitmenu
	{ MODKEY|ShiftMask,             XK_q,      exitdwm,        {0} },

    // inplacerotate
	{ Mod1Mask|ShiftMask|ControlMask, XK_j,    inplacerotate,  {.i = +1} },
	{ Mod1Mask|ShiftMask|ControlMask, XK_k,    inplacerotate,  {.i = -1} },
	{ MODKEY|ShiftMask|ControlMask, XK_j,      inplacerotate,  {.i = +2} },
	{ MODKEY|ShiftMask|ControlMask, XK_k,      inplacerotate,  {.i = -2} },

    // resetlayout
	{ MODKEY,                       XK_backslash, resetlayout,    {.ui = 0} },
	{ MODKEY|ControlMask,           XK_backslash, resetlayout,    {.ui = 1} },
	{ MODKEY|ShiftMask,             XK_backslash, resetlayout,    {.ui = 2} },
	{ MODKEY|ShiftMask|ControlMask, XK_backslash, resetlayout,    {.ui = 3} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
#include "tapresize.c"
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	// { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,         Button4,        resizemousescroll, {.v = &scrollargs[0]} },
	{ ClkClientWin,         MODKEY,         Button5,        resizemousescroll, {.v = &scrollargs[1]} },
	{ ClkClientWin,         MODKEY,         Button6,        resizemousescroll, {.v = &scrollargs[2]} },
	{ ClkClientWin,         MODKEY,         Button7,        resizemousescroll, {.v = &scrollargs[3]} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
