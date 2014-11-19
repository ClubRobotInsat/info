#ifndef _COMMUN_H_
#define _COMMUN_H_

#include <memory>
#include <type_traits>
#include <thread>
#include "Clock.h"
#include "Enum.h"

ENUM_CLASS(ResultatAction,
		   REUSSI, RATE, BLOQUE_PAR_ADV, TIMEOUT, BLOQUE
		   )

inline void setThreadName(char const *name) {
#if __LINUX__
	pthread_setname_np(pthread_self(), name);
#elif __APPLE__
	pthread_setname_np(name);
#endif
}

#endif
