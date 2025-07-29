/* See LICENSE file for copyright and license details. */

/* TODO add patches:
custom-fakefullscreen (DONE)
resizecorners (DONE)
steam (DONE)
zoomswap (DONE)
keychords (Next)
scratchpads (After)
torus (DONE)
NOTE: Zoom function is literally broken, dont use it, itll brick DWM &
      I have no idea why
*/

#include <X11/XF86keysym.h>
#include "functions.h"
#include "themes/tokyonight.h"

/* Tags | Appearance */
#define ICONSIZE 14
#define ICONSPACING 8
static const unsigned int ulinepad      = 5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0; /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0; /* 1 to show underline on all tags, 0 for just the active ones */

static char *tags[]           = {
    "", "", "", "",
    "󰋶", "󰸳", "", "",
    "", "󰇞", "", ""
 };

static const int tagschemes[] = { // I use 12 Tags (check 'dwm.c' for decleration of Tags)
    SchemeTag1, SchemeTag2, SchemeTag3, SchemeTag4,
    SchemeTag5, SchemeTag6, SchemeTag7, SchemeTag8,
    SchemeTag9, SchemeTag10, SchemeTag11, SchemeTag12
 };

static const char *colors[][3]      = {
    /*                     fg       bg      border */
    [SchemeNorm]       = { gray3,   black,  gray2 },
    [SchemeSel]        = { gray4,   blue,   blue  },
    [SchemeTitle]      = { white,   black,  black }, // active window title
    [TabSel]           = { blue,    gray2,  black },
    [TabNorm]          = { gray3,   black,  black },
    [SchemeTag]        = { gray3,   black,  black }, // Tag border
    [SchemeTag1]       = { pink,    black,  black },
    [SchemeTag2]       = { orange,  black,  black },
    [SchemeTag3]       = { green,   black,  black },
    [SchemeTag4]       = { blue,    black,  black },
    [SchemeTag5]       = { white,   black,  black },
    [SchemeTag6]       = { red,     black,  black },
    [SchemeTag7]       = { orange,  black,  black },
    [SchemeTag8]       = { green,   black,  black },
    [SchemeTag9]       = { pink,    black,  black },
    [SchemeTag10]      = { yellow,  black,  black },
    [SchemeTag11]      = { blue,    black,  black },
    [SchemeTag12]      = { white,   black,  black },
    [SchemeLayout]     = { blue,    black,  black },
    [SchemeBtnPrev]    = { green,   black,  black },
    [SchemeBtnNext]    = { yellow,  black,  black },
    [SchemeBtnClose]   = { red,     black,  black },
};

static const char *fonts[] = { 
    "FiraCode:style:medium:size=12",
    "JetBrainsMono Nerd Font Mono:style:medium:size=19"
};


static const unsigned int borderpx  = 2;        /* Window border pixel*/
static const unsigned int default_border = 0;   /* TODO remove? to switch back to default border after dynamic border resizing via keybinds */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 12;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 12;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */


static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails,display systray on the 1st monitor,False: display systray on last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */

static const int showbar            = 1;        /* 0 means no bar */
static const int showtab            = showtab_auto;
static const int toptab             = 1;        /* 0 means bottom tab */
static const int floatbar           = 0;        /* 1 means the bar will float(don't have padding),0 means the bar have padding */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 5;
static const int vertpadbar         = 11;
static const int vertpadtab         = 35;
static const int horizpadtabi       = 15;
static const int horizpadtabo       = 15;

static const int scalepreview       = 4;
static const int tag_preview        = 1;        /* 1 means enable, 0 is off */
static const int colorfultag        = 1;        /* 0 means use SchemeSel for selected non vacant tag */


/* Variables | X86Key Decleration */
static const char *upvol[]                = { "/usr/bin/pamixer", "-i", "2", NULL };
static const char *downvol[]              = { "/usr/bin/pamixer", "-d", "2", NULL };
static const char *mutevol[]              = { "/usr/bin/pamixer", "-t",  NULL };
static const char *mpdplaypause[]         = { "/usr/bin/playerctl", "play-pause", NULL };
static const char *mpdnext[]              = { "/usr/bin/playerctl", "next", NULL };
static const char *mpdprev[]              = { "/usr/bin/playerctl", "previous", NULL };
static const char *screenshit[]           = { "/usr/bin/flameshot", "gui", NULL };

/* Dmenu | Commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char dmenufont[]            = "FiraCode:size=12";
static const char dmenucol[]             = "10";
static const char dmenugrid[]            = "5";
static const char *dmenucmd[] = {
  "/usr/bin/bash", "-c",
  "dmenu_run",
  "-m", dmenumon,
  "-fn", dmenufont,
  "-l", dmenucol,
  "-g", dmenugrid,
  NULL
};
static const char *roficmd[] = { "rofi", "-show", "drun", NULL };
static const Launcher launchers[] = {
    /* command     name to display */
    { roficmd,         "󰣇" },
};
static const char* termcmd[]     = { "wezterm", NULL };
static const char* termeditcmd[] = { "wezterm", "-e", "nvim", NULL };
static const char* editorcmd[]   = { "emacs", NULL };
static const char* browsrcmd[]   = { "zen-browser", NULL };

/* Window Management */
static const float mfact        = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints in tiled resizals */

static const int focusonwheel             = 0;
static int torusenabled                   = 1; // Mouse wraps around the screen (can be toggled)
static const int wormholedelta            = 1; // How close to the edge before it wormholes
static const int lockfullscreen           = 0; /* 1 = Force focus on fullscreened window */
static const int new_window_attach_on_end = 1; /* 1 = Window attach on the end; 0 = Window attach front*/

static const Layout layouts[] = {
    { "[]=",      tile },           // 0
    { "|M|",      centeredmaster }, // 1
    { "TTT",      bstack },         // 2
    { "---",      horizgrid },      // 3
    { "><>",      NULL },           // No layout function means floating
    { NULL,       NULL },
};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     iscentered   isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            0,           1,           -1 },
    { "Steam",    NULL,       NULL,       1,            0,           1,           -1 },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define CTRLKEY ControlMask
#define SHFTKEY ShiftMask

#define TAGKEYS(KEY,TAG) \
    { MODKEY,                   KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|CTRLKEY,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|SHFTKEY,           KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|CTRLKEY|SHFTKEY,   KEY,      toggletag,      {.ui = 1 << TAG} },

/* Spawn Function TODO move to functions.h? */
#define SPWN(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Define Keybindings */
static const Key keys[] = {
    // XF86Keys Support
    { 0, XF86XK_AudioRaiseVolume,   spawn, { .v = upvol } },
    { 0, XF86XK_AudioLowerVolume,   spawn, { .v = downvol } },
    { 0, XF86XK_AudioMute,          spawn, { .v = mutevol } },
    { 0, XF86XK_AudioPlay,          spawn, { .v = mpdplaypause } },
    { 0, XF86XK_AudioNext,          spawn, { .v = mpdnext } },
    { 0, XF86XK_AudioPrev,          spawn, { .v = mpdprev } },
    { 0, XF86XK_Explorer,           spawn, { .v = screenshit } },

    // Spawners
    { MODKEY,                       XK_Return,    spawn,            { .v = termcmd } },
    { MODKEY,                       XK_e,         spawn,            { .v = editorcmd } },
    { MODKEY,                       XK_space,     spawn,            { .v = roficmd } },
    { MODKEY,                       XK_s,         spawn,            SPWN("spotify-launcher") },
    { MODKEY,                       XK_p,         spawn,            SPWN("pwvucontrol") },
    { MODKEY,                       XK_g,         spawn,            SPWN("gimp") },

    { MODKEY|SHFTKEY,               XK_b,         spawn,            { .v = browsrcmd } },
    { MODKEY|SHFTKEY,               XK_n,         spawn,            { .v = termeditcmd } },
    { MODKEY|SHFTKEY,               XK_e,         spawn,            SPWN("emacsclient -c") },
    { MODKEY|SHFTKEY,               XK_t,         spawn,            SPWN("nemo") },
    { MODKEY|SHFTKEY,               XK_d,         spawn,            SPWN("vesktop") },
    { MODKEY|SHFTKEY,               XK_s,         spawn,            SPWN("steam") },
    { MODKEY|SHFTKEY,               XK_m,         spawn,            SPWN("./System/Applications/Slippi/Slippi-Launcher.AppImage") },
    { MODKEY|SHFTKEY,               XK_g,         spawn,            SPWN("godot4") },

    // Window Management | Toggle Options
    { MODKEY,                       XK_w,         killclient,       {0} },
    { MODKEY|SHFTKEY,               XK_r,         restart,          {0} },
    { MODKEY|CTRLKEY,               XK_q,         spawn,            SPWN("killall bar.sh chadwm") },

    { MODKEY,                       XK_q,         hidewin,          {0} },
    { MODKEY|SHFTKEY,               XK_q,         restorewin,       {0} },

    { MODKEY,                       XK_f,         togglefullscr,    {0} },
    { MODKEY,                       XK_b,         togglebar,        {0} },
    { MODKEY|SHFTKEY,               XK_space,     togglefloating,   {0} },
    { MODKEY|SHFTKEY,               XK_f,         togglefakefullscr,  {0} },
    { MODKEY|CTRLKEY,               XK_g,         togglegaps,       {0} },
    { MODKEY|CTRLKEY|SHFTKEY,       XK_t,         toggletorus,      {0} },

    { MODKEY,                       XK_j,          focusstack,      {.i = +1 } },
    { MODKEY,                       XK_k,          focusstack,      {.i = -1 } },
    { MODKEY,                       XK_i,          incnmaster,      {.i = +1 } },
    { MODKEY,                       XK_o,          incnmaster,      {.i = -1 } },

    // Change Window Sizing (M = Horizontal; C = Vertical)
    { MODKEY,                       XK_h,          setmfact,        {.f = -0.05} },
    { MODKEY,                       XK_l,          setmfact,        {.f = +0.05} },
    { MODKEY|SHFTKEY,               XK_h,          setcfact,        {.f = +0.25} },
    { MODKEY|SHFTKEY,               XK_l,          setcfact,        {.f = -0.25} },
    { MODKEY|SHFTKEY|CTRLKEY,       XK_r,          setcfact,        {.f =  0.00} },


    { MODKEY|SHFTKEY,               XK_j,          movestack,       {.i = +1 } },
    { MODKEY|SHFTKEY,               XK_k,          movestack,       {.i = -1 } },
    { MODKEY,                       XK_Tab,        view,            {0} },

    // Layouts
    { MODKEY|CTRLKEY,               XK_r,          setlayout,       {.v = &layouts[0]} },
    { MODKEY|CTRLKEY,               XK_t,          setlayout,       {.v = &layouts[1]} },
    { MODKEY|CTRLKEY,               XK_y,          setlayout,       {.v = &layouts[2]} },
    { MODKEY|CTRLKEY,               XK_u,          setlayout,       {.v = &layouts[3]} },
    { MODKEY|CTRLKEY,               XK_comma,      cyclelayout,     {.i = -1 } },
    { MODKEY|CTRLKEY,               XK_period,     cyclelayout,     {.i = +1 } },

    TAGKEYS(                        XK_1,                           0)
    TAGKEYS(                        XK_2,                           1)
    TAGKEYS(                        XK_3,                           2)
    TAGKEYS(                        XK_4,                           3)
    TAGKEYS(                        XK_5,                           4)
    TAGKEYS(                        XK_6,                           5)
    TAGKEYS(                        XK_7,                           6)
    TAGKEYS(                        XK_8,                           7)
    TAGKEYS(                        XK_9,                           8)
    TAGKEYS(                        XK_0,                           9)
    TAGKEYS(                        XK_minus,                      10)
    TAGKEYS(                        XK_equal,                      11)
};

/* TODO button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          SPWN("st") },

    /* Keep movemouse? */
    /* { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, */

    /* placemouse options, choose which feels more natural:
    *    0 - tiled position is relative to mouse cursor
    *    1 - tiled position is relative to window center
    *    2 - mouse pointer warps to window center
    *
    * The moveorplace uses movemouse or placemouse depending on the floating state
    * of the selected client. Set up individual keybindings for the two if you want
    * to control these separately (i.e. to retain the feature to move a tiled window
    * into a floating position).
    */
    { ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         CTRLKEY,        Button1,        dragmfact,      {0} },
    { ClkClientWin,         CTRLKEY,        Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabPrev,           0,              Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,              Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,              Button1,        killclient,     {0} },
};
