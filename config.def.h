/* See LICENSE file for copyright and license details. */

/*
 * appearance
 *
 * font: see http://freedesktop.org/software/fontconfig/fontconfig-user.html
 */
static char *font = "Liberation Mono:pixelsize=12:antialias=true:autohint=true";

/*
 * background image
 * expects farbfeld format
 * pseudo transparency fixes coordinates to the screen origin
 */
static const char *bgfile = "~/.cache/st-wallpaper.ff";
static const int use_bgimage = 1;
static const int pseudotransparency = 0;

/* How to align the content in the window when the size of the terminal
 * doesn't perfectly match the size of the window. The values are percentages.
 * 50 means center, 0 means flush left/top, 100 means flush right/bottom.
 */
static int anysize_halign = 50;
static int anysize_valign = 50;

static int borderpx = 2;

/*
 * What program is execed by st depends of these precedence rules:
 * 1: program passed with -e
 * 2: scroll and/or utmp
 * 3: SHELL environment variable
 * 4: value of shell in /etc/passwd
 * 5: value of shell in config.h
 */
#ifdef __ANDROID__
static char *shell = "/data/data/com.termux/files/usr/bin/sh";
#else
static char *shell = "/bin/sh";
#endif
char *utmp = NULL;
/* scroll program: to enable use a string like "scroll" */
char *scroll = NULL;
char *stty_args = "stty raw pass8 nl -echo -iexten -cstopb 38400";

/* identification sequence returned in DA and DECID */
/* By default, use the same one as kitty. */
char *vtiden = "\033[?62;4c"; /* VT200 family (62) with sixel (4) */

/* sixel rgb byte order: LSBFirst or MSBFirst */
int const sixelbyteorder = LSBFirst;

/* Kerning / character bounding-box multipliers */
static float cwscale = 1.0;
static float chscale = 1.0;

/*
 * word delimiter string
 *
 * More advanced example: L" `'\"()[]{}"
 */
wchar_t *worddelimiters = L" ";

/* selection timeouts (in milliseconds) */
static unsigned int doubleclicktimeout = 300;
static unsigned int tripleclicktimeout = 600;

/* alt screens */
int allowaltscreen = 1;

/* allow certain non-interactive (insecure) window operations such as:
   setting the clipboard text */
int allowwindowops = 0;

/*
 * draw latency range in ms - from new content/keypress/etc until drawing.
 * within this range, st draws when content stops arriving (idle). mostly it's
 * near minlatency, but it waits longer for slow updates to avoid partial draw.
 * low minlatency will tear/flicker more, as it can "detect" idle too early.
 */
static double minlatency = 2;
static double maxlatency = 33;

/*
 * Synchronized-Update timeout in ms
 * https://gitlab.com/gnachman/iterm2/-/wikis/synchronized-updates-spec
 */
static uint su_timeout = 200;

/*
 * blinking timeout (set to 0 to disable blinking) for the terminal blinking
 * attribute.
 */
static unsigned int blinktimeout = 800;

/*
 * thickness of underline and bar cursors
 */
static unsigned int cursorthickness = 2;

/* Hide the X cursor whenever a key is pressed. 0: off, 1: on */
int hidecursor = 1;

/*
 * 1: render most of the lines/blocks characters without using the font for
 *    perfect alignment between cells (U2500 - U259F except dashes/diagonals).
 *    Bold affects lines thickness if boxdraw_bold is not 0. Italic is ignored.
 * 0: disable (render all U25XX glyphs normally from the font).
 */
const int boxdraw = 0;
const int boxdraw_bold = 0;

/* braille (U28XX):  1: render as adjacent "pixels",  0: use font */
const int boxdraw_braille = 0;

/*
 * bell volume. It must be a value between -100 and 100. Use 0 for disabling
 * it
 */
static int bellvolume = 0;

/* default TERM value */
char *termname = "st-256color";

/*
 * spaces per tab
 *
 * When you are changing this value, don't forget to adapt the »it« value in
 * the st.info and appropriately install the st.info in the environment where
 * you use this st version.
 *
 *	it#$tabspaces,
 *
 * Secondly make sure your kernel is not expanding tabs. When running `stty
 * -a` »tab0« should appear. You can tell the terminal to not expand tabs by
 *  running following command:
 *
 *	stty tabs
 */
unsigned int tabspaces = 8;

/*
 * drag and drop escape characters
 *
 * this will add a '\' before any characters specified in the string.
 */
char *xdndescchar = " !\"#$&'()*;<>?[\\]^`{|}~";

/* Terminal colors (16 first used in escape sequence) */
static const char *colorname[] = {
	/* 8 normal colors */
	"black",
	"red3",
	"green3",
	"yellow3",
	"blue2",
	"magenta3",
	"cyan3",
	"gray90",

	/* 8 bright colors */
	"gray50",
	"red",
	"green",
	"yellow",
	"#5c5cff",
	"magenta",
	"cyan",
	"white",

	[255] = 0,

	/* more colors can be added after 255 to use with DefaultXX */
	"#add8e6", /* 256 -> cursor */
	"#555555", /* 257 -> rev cursor*/
	"#000000", /* 258 -> bg */
	"#e5e5e5", /* 259 -> fg */
};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultbg = 258;
unsigned int defaultfg = 259;
unsigned int defaultcs = 256;
unsigned int defaultrcs = 257;

/*
 * https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h4-Functions-using-CSI-_-ordered-by-the-final-character-lparen-s-rparen:CSI-Ps-SP-q.1D81
 * Default style of cursor
 * 0: Blinking block
 * 1: Blinking block (default)
 * 2: Steady block ("â–ˆ")
 * 3: Blinking underline
 * 4: Steady underline ("_")
 * 5: Blinking bar
 * 6: Steady bar ("|")
 * 7: Blinking st cursor
 * 8: Steady st cursor
 */
static unsigned int cursorstyle = 1;
static Rune stcursor = 0x2603; /* snowman (U+2603) */

/*
 * Default columns and rows numbers
 */

static unsigned int cols = 80;
static unsigned int rows = 24;

/*
 * Default colour and shape of the mouse cursor
 */
static unsigned int mouseshape = XC_xterm;
static unsigned int mousefg = 7;
static unsigned int mousebg = 0;

/*
 * Color used to display font attributes when fontconfig selected a font which
 * doesn't match the ones requested.
 */
static unsigned int defaultattr = 11;

/*
 * Graphics configuration
 */

/// The template for the cache directory.
#ifdef __ANDROID__
const char graphics_cache_dir_template[] = "/data/data/com.termux/files/usr/tmp/st-images-XXXXXX";
#else
const char graphics_cache_dir_template[] = "/tmp/st-images-XXXXXX";
#endif
/// The max size of a single image file, in bytes.
unsigned graphics_max_single_image_file_size = 20 * 1024 * 1024;
/// The max size of the cache, in bytes.
unsigned graphics_total_file_cache_size = 300 * 1024 * 1024;
/// The max ram size of an image or placement, in bytes.
unsigned graphics_max_single_image_ram_size = 100 * 1024 * 1024;
/// The max total size of all images loaded into RAM.
unsigned graphics_max_total_ram_size = 300 * 1024 * 1024;
/// The max total number of image placements and images.
unsigned graphics_max_total_placements = 4096;
/// The ratio by which limits can be exceeded. This is to reduce the frequency
/// of image removal.
double graphics_excess_tolerance_ratio = 0.05;
/// The minimum delay between redraws caused by animations, in milliseconds.
unsigned graphics_animation_min_delay = 20;

/*
 * Force mouse select/shortcuts while mask is active (when MODE_MOUSE is set).
 * Note that if you want to use ShiftMask with selmasks, set this to an other
 * modifier, set to 0 to not use it.
 */
static uint forcemousemod = ShiftMask;

/*
 * Internal mouse shortcuts.
 * Beware that overloading Button1 will disable the selection.
 */
static MouseShortcut mshortcuts[] = {
	/* mask                 button   function        argument       release  screen */
	{ TERMMOD,              Button3, previewimage,   {.s = "feh"} },
	{ TERMMOD,              Button2, showimageinfo,  {},            1 },
	{ XK_ANY_MOD,           Button2, selpaste,       {.i = 0},      1 },
	{ ShiftMask,            Button4, ttysend,        {.s = "\033[5;2~"} },
	{ ShiftMask,            Button5, ttysend,        {.s = "\033[6;2~"} },
	{ XK_ANY_MOD,           Button4, kscrollup,      {.i = 1},      0, S_PRI },
	{ XK_ANY_MOD,           Button5, kscrolldown,    {.i = 1},      0, S_PRI },
	{ XK_ANY_MOD,           Button4, ttysend,        {.s = "\031"}, 0, S_ALT },
	{ XK_ANY_MOD,           Button5, ttysend,        {.s = "\005"}, 0, S_ALT },
};

/* Internal keyboard shortcuts. */
#define MODKEY Mod1Mask
#define TERMMOD (ControlMask|ShiftMask)

static char *openurlcmd[] = { "/bin/sh", "-c",
	"xurls | dmenu -l 10 -w $WINDOWID | xargs -r open",
	"externalpipe", NULL };

static char *setbgcolorcmd[] = { "/bin/sh", "-c",
	"printf '\033]11;#008000\007'",
	"externalpipein", NULL };

static Shortcut shortcuts[] = {
	/* mask                 keysym          function         argument   screen */
	{ XK_ANY_MOD,           XK_Break,       sendbreak,       {.i =  0} },
	{ ControlMask,          XK_Print,       toggleprinter,   {.i =  0} },
	{ ShiftMask,            XK_Print,       printscreen,     {.i =  0} },
	{ XK_ANY_MOD,           XK_Print,       printsel,        {.i =  0} },
	{ TERMMOD,              XK_Prior,       zoom,            {.f = +1} },
	{ TERMMOD,              XK_Next,        zoom,            {.f = -1} },
	{ TERMMOD,              XK_Home,        zoomreset,       {.f =  0} },
	{ TERMMOD,              XK_C,           clipcopy,        {.i =  0} },
	{ TERMMOD,              XK_V,           clippaste,       {.i =  0} },
	{ ShiftMask,            XK_Page_Up,     kscrollup,       {.i = -1}, S_PRI },
	{ ShiftMask,            XK_Page_Down,   kscrolldown,     {.i = -1}, S_PRI },
	{ TERMMOD,              XK_Y,           selpaste,        {.i =  0} },
	{ ShiftMask,            XK_Insert,      selpaste,        {.i =  0} },
	{ TERMMOD,              XK_Num_Lock,    numlock,         {.i =  0} },
	{ TERMMOD,              XK_F1,          togglegrdebug,  {.i =  0} },
	{ TERMMOD,              XK_F6,          dumpgrstate,    {.i =  0} },
	{ TERMMOD,              XK_F7,          unloadimages,   {.i =  0} },
	{ TERMMOD,              XK_F8,          toggleimages,   {.i =  0} },
};

/*
 * Special keys (change & recompile st.info accordingly)
 *
 * Mask value:
 * * Use XK_ANY_MOD to match the key no matter modifiers state
 * * Use XK_NO_MOD to match the key alone (no modifiers)
 * appkey value:
 * * 0: no value
 * * > 0: keypad application mode enabled
 * *   = 2: term.numlock = 1
 * * < 0: keypad application mode disabled
 * appcursor value:
 * * 0: no value
 * * > 0: cursor application mode enabled
 * * < 0: cursor application mode disabled
 *
 * Be careful with the order of the definitions because st searches in
 * this table sequentially, so any XK_ANY_MOD must be in the last
 * position for a key.
 */

/*
 * State bits to ignore when matching key or button events.  By default,
 * numlock (Mod2Mask) and keyboard layout (XK_SWITCH_MOD) are ignored.
 */
static uint ignoremod = Mod2Mask|XK_SWITCH_MOD;

/*
 * Selection types' masks.
 * Use the same masks as usual.
 * Button1Mask is always unset, to make masks match between ButtonPress.
 * ButtonRelease and MotionNotify.
 * If no match is found, regular selection is used.
 */
static uint selmasks[] = {
	[SEL_RECTANGULAR] = Mod1Mask,
};

/*
 * Printable characters in ASCII, used to estimate the advance width
 * of single wide characters.
 */
static char ascii_printable[] =
	" !\"#$%&'()*+,-./0123456789:;<=>?"
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
	"`abcdefghijklmnopqrstuvwxyz{|}~";

