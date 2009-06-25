/*
 * This file is part of uxlaunch
 *
 * (C) Copyright 2009 Intel Corporation
 * Authors: 
 *     Auke Kok <auke@linux.intel.com>
 *     Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uxlaunch.h"

/*
 * ssh-agent prints a bunch of env variables to its stdout that we need to put in 
 * the environment. 
 */
void start_ssh_agent(void)
{
	FILE *file;
	char line[4096];
	log_string("Entering start_ssh_agent");

	memset(line, 0, 4096);

	file = popen("/usr/bin/ssh-agent", "r");
	if (!file) {
		log_string("!! Failed to start ssh-agent");
		return;
	}
	/*
	 * ssh-agent output looks like this:
	 *
	 * SSH_AUTH_SOCK=/tmp/ssh-ccZMs16230/agent.16230; export SSH_AUTH_SOCK;
 	 * SSH_AGENT_PID=16231; export SSH_AGENT_PID;
 	 * echo Agent pid 16231;
	 * 
	 * so search for "; export", cut that off. Then split at the = for env var name
	 * and value.
	 */
	while (!feof(file)) {
		char *c;
		if (fgets(line, 4095, file)==NULL)
			break;
		c = strstr(line, "; export");
		if (c) {
			char *c2;
			*c = 0;
			c2 = strchr(line, '=');
			if (c2) {
				*c2 = 0;
				c2++;
				setenv(line, c2, 1);
			}
		}
	}
	pclose(file);
	log_environment();
}

/*
 * helper function to make debug easier
 */
void start_bash(void)
{
	int ret;

	log_string("Entering start_bash");

	fprintf(stderr, "Starting bash shell -- type exit to continue\n");
	ret = system("/bin/bash");
	if (ret != EXIT_SUCCESS)
		log_string("bash returned an error");
}

/*
 * We want to start gconf early, by hand, so that it can start processing the
 * XML well before someone needs it to cut down the total time
 */
void start_gconf(void)
{
	int ret;
	log_string("Entering start_gconf");
	ret = system("gconftool-2 --spawn");
	if (!ret)
		log_string("failure to start gconftool-2");
}


void maybe_start_screensaver(void)
{
	/* FIXME: to do */
	log_string("** Entering maybe_start_screensaver");
}

