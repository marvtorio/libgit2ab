//
//  ab_util.h
//  libgit2-demo1
//
//  Created by Marvin Torio on 4/12/14.
//  Copyright (c) 2014 Martian Madness. All rights reserved.
//

#ifndef libgit2_demo1_ab_util_h
#define libgit2_demo1_ab_util_h

#define FATAL_MSG_PREFIX "ERROR:"

void logit(const char *name, const char *value);

void fatal(const char *errmsg, const char *errmsg2);

void get_repopath(char **out, const char *repopath_env);

void get_envvar(char **out, const char *env);

void check_error(int error_code, const char *action);

#endif
