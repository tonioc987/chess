/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef LOGGER_H
#define LOGGER_H

namespace acortes {
namespace chess {

void Log(const char *file, int line, const char *message);

}
}

#define LOG(MESSAGE) Log(__FILE__, __LINE__, MESSAGE)

#endif
