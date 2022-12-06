#ifndef __SGLOG_H
#define __SGLOG_H

#include "Measure_Types.h"
#include "Measure_Constants.h"

void log_init(void);

#define SG_LOG(severity, message) do \
{ \
	logMessage(severity, __FILE__, __LINE__, message); \
} while (0)

void logMessage(
			EventLogSeverity_t severity,
			char *file_name,
			uint16_t code_line,
			char *inputStream);

#endif

