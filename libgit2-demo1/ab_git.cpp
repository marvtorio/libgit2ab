//
//  ab_git.cpp
//  libgit2-demo1
//
//  Created by Marvin Torio on 4/20/14.
//  Copyright (c) 2014 Martian Madness. All rights reserved.
//
#include "ab_git.h"

AbGit::AbGit()
{
}

void AbGit::initialize()
{
    repoHome = getenv("REPO_HOME");
    if (repoHome.empty())
    {
        std::cerr << "Unable to get REPO_HOME" << std::endl;
        exit(1);
    }
}
