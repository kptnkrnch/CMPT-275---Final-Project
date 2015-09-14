#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

// Debug control
#define DEBUG_ON        1 // Toggle all debug statements on or off
#define DEBUG_LEVEL     1 // Print only debugs of this level or higher
// 1, 1 will print all debug messages

// No more edits
#define DEBUG_INFO 1
#define DEBUG_WARN 2
#define DEBUG_ALRM 3

#define DEBUG(lvl, msg) if(DEBUG_ON && lvl >= DEBUG_LEVEL){ qDebug() << "DEBUG: " << lvl << ": " << msg; }

#endif // DEBUG
