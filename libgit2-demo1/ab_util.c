//
//  ab_util.c
//  libgit2-demo1
//
//  Created by Marvin Torio on 4/12/14.
//  Copyright (c) 2014 Martian Madness. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "git2.h"
#include "ab_util.h"

void logit(const char *name, const char *value)
{
    printf("=>[%ld] [%s=%s]\n", time(NULL), name, value);
}

void fatal(const char *errmsg, const char *errmsg2)
{
    exit(printf("%s %s - [%s]\n", FATAL_MSG_PREFIX, errmsg, errmsg2));
}


void get_repopath(char **out, const char *repopath_env)
{
    int envvarlen = 0;
    
    envvarlen = (int) strlen(getenv(repopath_env));
    *out = malloc(envvarlen+1);
    strcpy(*out, getenv(repopath_env));
    return;
}

void get_envvar(char **out, const char *env)
{
    int envvarlen = 0;
    
    envvarlen = (int) strlen(getenv(env));
    *out = malloc(envvarlen+1);
    strcpy(*out, getenv(env));
    return;
}

void check_error(int error_code, const char *action)
{
    const git_error *error = giterr_last();
    if (!error_code)
        return;
    
    printf("Error %d %s - %s\n", error_code, action,
           (error && error->message) ? error->message : "???");
    
    exit(1);
}
