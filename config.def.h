/* See LICENSE file for copyright and license details. */
#include "exitdwm.c"
#include "movestack.c"
/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const Gap default_gap = {.isgap = 2, .realgap = 8, .gappx = 8};
static const unsigned int snap = 32; /* snap pixel */
static const int showbar = 1;        /* 0 means no bar */
static const int topbar = 1;         /* 0 means bottom bar */
static const int user_bh = 25; /* 0 means that dwm will calculate bar height, >=
                                  1 means dwm will user_bh as bar height */
static const int vertpad = 5;  /* vertical padding of bar */
static const int sidepad = 10; /* horizontal padding of bar */
static const char *fonts[] = {
    "Iosevka Nerd Font:size=13:antialias=true:autohint=true",
    "Iosevka Nerd Font:style=Medium:size=13:antialias=true:autohint=true",
    "feather:size=13:antialias=true:autohint=true",
};
static const char dmenufont[] = "Iosevka Nerd Font:size=12:antialias=true";
static const char col_gray1[] = "#1E1E2E";
static const char col_gray3[] = "#11111b";
static const char col_white[] = "#cdd6f4";
static const char col_cyan[] = "#74c7ec";
static const char *colors[][3] = {
    /*               fg          bg         border   */
    [SchemeNorm] = {col_white, col_gray1, col_gray3},
    [SchemeSel] = {col_white, col_gray1, col_cyan},
    [SchemeTitle] = {col_cyan, col_gray1, col_white},
};
/* tagging */
static const char *tags[] = {"", "", "", "", "",
                             "", "", "", "", ""};

static const unsigned int ulinepad =
    5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke =
    2; /* thickness / height of the underline */
static const unsigned int ulinevoffset =
    0; /* how far above the bottom of the bar the line should appear */
static const int ulineall =
    0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 1, 0, -1},
    {"Zathura", NULL, NULL, 1 << 3, 0, -1},
    {"Nemo", NULL, NULL, 1 << 2, 0, -1},
    {"Soffice", NULL, NULL, 1 << 4, 0, -1},
    {"zoom", NULL, NULL, 1, 0, -1},
    {"mpv", NULL, NULL, 1 << 8, 0, -1}};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
    {"[@]", spiral},
    {"[\\]", dwindle},
    {"TTT", bstack},
    {"===", bstackhoriz},
    {"|M|", centeredmaster},
    {">M>", centeredfloatingmaster},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
    {MODKEY, KEY, view, {.ui = 1 << TAG}},                                     \
        {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},             \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                      \
        {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
    {                                                                          \
        .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                   \
    }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-c", "-F", NULL};
static const char *termcmd[] = {"kitty", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_bracketright, viewnext, {0}},
    {MODKEY, XK_bracketleft, viewprev, {0}},
    {MODKEY, XK_Tab, tagtonext, {0}},
    {MODKEY | Mod1Mask, XK_Tab, tagtoprev, {0}},
    {MODKEY | ShiftMask, XK_b, togglebar, {0}},
    {MODKEY | ShiftMask, XK_f, fullscreen, {0}},
    {MODKEY, XK_Right, focusstack, {.i = +1}},
    {MODKEY, XK_Left, focusstack, {.i = -1}},
    {MODKEY | ShiftMask, XK_Right, movestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_Left, movestack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_r, setlayout, {.v = &layouts[3]}},
    {MODKEY | ShiftMask, XK_r, setlayout, {.v = &layouts[4]}},
    {MODKEY, XK_u, setlayout, {.v = &layouts[5]}},
    {MODKEY, XK_o, setlayout, {.v = &layouts[6]}},
    {MODKEY | ShiftMask, XK_u, setlayout, {.v = &layouts[7]}},
    {MODKEY | ShiftMask, XK_o, setlayout, {.v = &layouts[8]}},
    {0 | Mod1Mask, XK_l, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_minus, setgaps, {.i = -5}},
    {MODKEY, XK_equal, setgaps, {.i = +5}},
    {MODKEY | ShiftMask, XK_minus, setgaps, {.i = GAP_RESET}},
    {MODKEY | ShiftMask, XK_equal, setgaps, {.i = GAP_TOGGLE}},
    {MODKEY | ControlMask | ShiftMask, XK_q, quit, {1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)
                TAGKEYS(XK_0, 9){MODKEY | ShiftMask, XK_e, exitdwm, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
