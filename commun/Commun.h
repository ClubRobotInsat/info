#ifndef _COMMUN_H_
#define _COMMUN_H_

#include "Enum/Enum.h"
#include "Units/Units.h"
#include "MathToolbox/MathToolbox.h"
#include "Trame.h"
#include "Clock.h"
#include "Utils.h"
#include "ResultatAction.h"

using namespace std::string_literals;

inline void setThreadName(char const *name) {
#if __LINUX__
	pthread_setname_np(pthread_self(), name);
#elif __APPLE__
	pthread_setname_np(name);
#endif
}

inline void setThreadName(std::string const &name) {
	setThreadName(name.c_str());
}

using std::uint8_t;
using std::int8_t;
using std::uint16_t;
using std::int16_t;
using std::uint32_t;
using std::int32_t;
using std::uint64_t;
using std::int64_t;

#endif
