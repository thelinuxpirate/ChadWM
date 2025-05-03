static int topbar = 1;                      /* -b  option; if 0, dmenu appears at bottom */
static int caret_width = 10;                 /* -cw option; set default caret width */
static int fuzzy = 0;                       /* -F  option; if 0, dmenu doesn't use fuzzy matching */
static int center = 1;                      /* -c  option; if 0, dmenu won't be centered on the screen */
static int min_width = 1100;                 /* minimum width when centered */
static int restrict_return = 1;             /* -1 option; if 1, disables shift-return and ctrl-return */
/* -fn option overrides fonts[0]; default X11 font or font set */
static char *fonts[] =
{
	"Comic Mono:size=12",
  "Nerd Fonts:size=11"
};
static char *prompt            = NULL;      /* -p  option; prompt to the left of input field */
static const char *dynamic     = NULL;      /* -dy option; dynamic command to run on input change */
static const char *symbol_1 = "<";
static const char *symbol_2 = ">";

static
char *colors[][2] = {
	/*               fg         bg       */
	[SchemeNorm] = { "#c5dde0", "#101419" },
	[SchemeSel]  = { "#c5dde0", "#5d869e" },
	[SchemeOut]  = { "#000000", "#00ffff" },
	[SchemeBorder] = { "#000000", "#6eaae6" },
	[SchemeMid]  = { "#c5dde0", "#202836" },
	[SchemeSelHighlight]  = { "#db8fd9", "#202836" },
	[SchemeNormHighlight] = { "#db8fd9", "#222222" },
	[SchemeHover]  = { "#c5dde0", "#353D4B" },
	[SchemeGreen]  = { "#c5dde0", "#52E067" },
	[SchemeRed]    = { "#c5dde0", "#e05252" },
	[SchemeYellow] = { "#c5dde0", "#222222" },
	[SchemeBlue]   = { "#c5dde0", "#5280e0" },
	[SchemePurple] = { "#c5dde0", "#9952e0" },
};
/* -l option; if nonzero, dmenu uses vertical list with given number of lines */
static unsigned int lines      = 1;
/* -g option; if nonzero, dmenu uses a grid comprised of columns and lines */
static unsigned int columns    = 1;
static unsigned int lineheight = 35;         /* -h option; minimum height of a menu line     */
static unsigned int min_lineheight = 40;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";

/* Size of the window border */
static unsigned int border_width = 3;
