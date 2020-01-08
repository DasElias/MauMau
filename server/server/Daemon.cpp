#include "Daemon.h"

#if defined(__unix__) && defined(BUILD_SERVER_AS_DAEMON) 
#pragma message "BUILD SERVER AS DAEMON"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include <signal.h>

typedef void (*sighandler_t)(int);
static sighandler_t
handle_signal(int sig_nr, sighandler_t signalhandler) {
	struct sigaction neu_sig, alt_sig;
	neu_sig.sa_handler = signalhandler;
	sigemptyset(&neu_sig.sa_mask);
	neu_sig.sa_flags = SA_RESTART;
	if(sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
		return SIG_ERR;
	return alt_sig.sa_handler;
}

namespace card {
	void initDaemon() {
		pid_t pid, sid;

		// Fork off the parent process
		pid = fork();

		// if we got a good PID, then we can exit the parent process
		if(pid > 0) exit(EXIT_SUCCESS);
		else if (pid < 0) exit(EXIT_FAILURE);

		// create new SID for child process
		sid = setsid();
		if(sid < 0) exit(EXIT_FAILURE);

		handle_signal(SIGHUP, SIG_IGN);

		// change the current working directory
		if(chdir("/") < 0) {
			exit(EXIT_FAILURE);
		}

		// change the file mode mask
		umask(0);

		// close out the  standard file descriptors
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

	}
}
#else
#pragma message "NOT BUILD AS DAEMON"
namespace card {
	void initDaemon() {
		// empty method body
	}
}
#endif
