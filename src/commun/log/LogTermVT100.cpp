// LogTermVT100.cpp

#ifndef WIN32

#include "Log.h"

enum TermAttr {
	TERM_RESET = 0,  // "normal" mode
	TERM_BRIGHT = 1, // more luminosity for the foreground
	TERM_DIM = 2,    // less luminosity for the foreground
	TERM_UNDERLINE = 4,
	TERM_BLINK = 5,   // no difference...
	TERM_REVERSE = 7, // reverse front and back color
};

enum TermColor {
	TERM_BLACK = 0,
	TERM_RED,
	TERM_GREEN,
	TERM_YELLOW,
	TERM_BLUE,
	TERM_MAGENTA,
	TERM_CYAN,
	TERM_WHITE,
	TERM_NONE // not really standard, but it works with xterm...^^
};

struct TermFormat {
	// Declaring this constructor prevents a silent failure when a LogType enumarator is added, and the array below is
	// not update accordingly.
	TermFormat(TermAttr a, TermColor fc, TermColor bc) : attr(a), front_color(fc), back_color(bc) {}

	TermAttr attr;
	TermColor front_color;
	TermColor back_color;
};

static const TermFormat term_formats[static_cast<int>(LogType::ALL)] = {
    {TERM_BRIGHT, TERM_WHITE, TERM_RED},    // error
    {TERM_BRIGHT, TERM_WHITE, TERM_YELLOW}, // warn

    {TERM_BRIGHT, TERM_RED, TERM_NONE},   // failed
    {TERM_BRIGHT, TERM_GREEN, TERM_NONE}, // success

    {TERM_RESET, TERM_NONE, TERM_NONE}, // info

    {TERM_BRIGHT, TERM_WHITE, TERM_GREEN},    // debug0
    {TERM_BRIGHT, TERM_WHITE, TERM_BLUE},     // debug1
    {TERM_BRIGHT, TERM_WHITE, TERM_MAGENTA},  // debug2
    {TERM_BRIGHT, TERM_WHITE, TERM_CYAN},     // debug3
    {TERM_BRIGHT, TERM_WHITE, TERM_BLACK},    // debug4
    {TERM_BRIGHT, TERM_YELLOW, TERM_GREEN},   // debug5
    {TERM_BRIGHT, TERM_YELLOW, TERM_BLUE},    // debug6
    {TERM_BRIGHT, TERM_YELLOW, TERM_MAGENTA}, // debug7
    {TERM_BRIGHT, TERM_YELLOW, TERM_CYAN},    // debug8
    {TERM_BRIGHT, TERM_YELLOW, TERM_BLACK}    // debug9
};

// Useless in the vt100/xterm implementation, but necessary for the Windows version.
void Log::writeTermFormattedString(std::ostream& p_stream, const std::string& str) {
	p_stream << str;
}

void Log::resetTerm(std::ostream& p_stream) {
	p_stream << static_cast<char>(0x1B) << "[0;;m";
}

void Log::doTermFormatting(std::string& msg, LogType type) {
	const TermFormat& format = term_formats[static_cast<int>(type)];

	char str_beginning[64] = "";
	sprintf(str_beginning, "%c[%d;%d;%dm", 0x1B, int(format.attr), int(format.front_color) + 30, int(format.back_color) + 40);
	msg = std::string(str_beginning) + msg;
}
#endif // !defined WIN32
