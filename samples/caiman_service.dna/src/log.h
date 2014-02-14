#ifndef __LOG_H__
#define __LOG_H__ 1

#include <sdlog.h>
#undef log_debug // Defined in sdlog.h.

#define log_full(level, fmt...) \
	log_generic(level, __FILE__, __LINE__, __func__, fmt)

// As log level, the syslog defines are used.
#define log_debug(fmt...) \
	log_generic(LOG_DEBUG, __FILE__, __LINE__, __func__, fmt)
#define log_info(fmt...) \
	log_generic(LOG_INFO, __FILE__, __LINE__, __func__, fmt)
#define log_warn(fmt...) \
	log_generic(LOG_WARNING, __FILE__, __LINE__, __func__, fmt)
#define log_error(fmt...) \
	log_generic(LOG_ERR, __FILE__, __LINE__, __func__, fmt)

namespace caiman
{

namespace system_daemon
{

void log_enable_console();
void log_generic(int, const char *, int, const char *, const char *, ...);

}; // namespace system_daemon

}; // namespace caiman

#endif // __LOG_H__

