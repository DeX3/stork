//
// log.cc - This file is part of System Daemon.
// Copyright (c) 2013 Theobroma Systems Design and Consulting GmbH
//

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

#include "log.h"

namespace caiman
{

namespace system_daemon
{

using namespace std;

// console is always the fallback, however it is not used by default if we're
// logging to syslog.
static bool enable_console = false;
// The file descriptor will be changed to /dev/console if we're init.
static int fd_console = STDERR_FILENO;

static bool open_syslog()
{
	static bool currently_open = false;

	struct stat info;
	if (stat("/dev/log", &info) < 0) {
		if (currently_open) {
			closelogp();
			currently_open = false;
		}
		return false;
	}

	openlogp("systemd", LOG_NDELAY, LOG_DAEMON);
	currently_open = true;
	return true;
}

static bool open_console()
{
	if (getpid() == 1) { // If we are really init, we use /dev/console.
		int fd = open("/dev/console", O_WRONLY | O_NOCTTY | O_CLOEXEC);
		if (fd < 0) {
			return false;
		}
		fd_console = fd;
	}

	return true;
}

static void close_console()
{
	if (fd_console != STDERR_FILENO) {
		close(fd_console);
	}
}

static void log_console(int, const char *, int, const char *, const char *,
		va_list);

static void log_syslog(int level, const char *file, int line, const char *func,
		const char *format, va_list arg)
{
	if (!open_syslog()) {
		if (!enable_console) {
			// syslog does not seem to work, console is not enabled, but we use
			// it as fallback.
			log_console(level, file, line, func, format, arg);
		}
		return;
	}

	char message_id[256];
	memset(message_id, 0, sizeof(message_id));

	snprintf(message_id, 255, "[%s:%d - %s]", file, line, func);
	vsyslogp(level, message_id, NULL, format, arg);
}

static void log_console(int level, const char *file, int line, const char *func,
		const char *format, va_list arg)
{
	open_console();

	char header[256], message[1024];
	int n = 0;
	struct iovec iov[2];

	memset(header, 0, sizeof(header));
	memset(message, 0, sizeof(message));

	// Log header: level, source file, line number, function name.
	switch (level) {
	case LOG_DEBUG:
		n += sprintf(header, "DEBUG ");
		break;
	case LOG_INFO:
		n += sprintf(header, "INFO  ");
		break;
	case LOG_WARNING:
		n += sprintf(header, "WARN  ");
		break;
	case LOG_ERR:
		n += sprintf(header, "ERROR ");
		break;
	}
	n += sprintf(header+n, "[%s:%d - %s] ", file, line, func);

	// Log message: format and arg.
	n = vsnprintf(message, sizeof(message)-1, format, arg);
	if (message[n-1] != '\n') {
		message[n] = '\n';
	}

	iov[0].iov_base = header;
	iov[0].iov_len = strlen(header);
	iov[1].iov_base = message;
	iov[1].iov_len = strlen(message);
	writev(fd_console, iov, 2);

	close_console();
}

void log_enable_console()
{
	enable_console = true;
	log_info("Logging to console (stderr) activated in addition to syslog");
}

void log_generic(int level, const char *file, int line, const char *func,
		const char *format, ...)
{
	va_list arg;

	va_start(arg, format);

	log_syslog(level, file, line, func, format, arg);

	if (enable_console) {
		log_console(level, file, line, func, format, arg);
	}

	va_end(arg);
}

}; // namespace system_daemon

}; // namespace caiman

