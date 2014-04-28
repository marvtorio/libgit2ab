//
//  main.c
//  libgit2-demo1
//
//  Created by Marvin Torio on 4/6/14.
//  Copyright (c) 2014 Martian Madness. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "git2.h"

void fatal(const char *errmsg, const char *errmsg2)
{
    exit(printf("ERROR: %s - [%s]\n", errmsg, errmsg2));
}

int ab_util_signature_default(git_signature **sig, git_repository *repo)
{
    int retval = 0;
    retval = git_signature_default(sig, repo);
    
    if (retval < 0)
    {
        fatal("Not able to create commit signature", NULL);
    }
    return retval;
}

int ab_util_repository_index(git_index **idx, git_repository *repo)
{
    int retval = 0;
    retval = git_repository_index(idx, repo);
    
    if (retval < 0)
    {
        fatal("Not able to open repository index", NULL);
    }
    return retval;
}


int ab_git_init(git_repository **gitrepo, const char *repo_path)
{
    int retval;
    
    retval = git_repository_init(gitrepo, repo_path, 0);
    return retval;
}

void ab_create_initial_commit(git_repository *gitrepo)
{
    git_signature *gsig;
    git_index *gidx;
    git_oid tree_id, commit_id;
    git_tree *gtree;
    
    if (git_signature_default(&gsig, gitrepo) < 0)
    {
        fatal("Unable to create commit signature.", "Perhaps user.name and user.email are NOT set.");
    }
    
    if (git_repository_index(&gidx, gitrepo) < 0)
    {
        fatal("Could not open repository index", NULL);
    }
    
    if (git_index_write_tree(&tree_id, gidx) < 0)
    {
        fatal("Unable to write initial tree from index", NULL);
    }
    
    git_index_free(gidx);
    
    if (git_tree_lookup(&gtree, gitrepo, &tree_id) < 0 )
    {
        fatal("Could not lookup initial tree", NULL);
    }
    
    if (git_commit_create_v(&commit_id, gitrepo, "HEAD", gsig, gsig, NULL, "Initial test commit", gtree, 0) < 0)
    {
        fatal("Could not create initial commit", NULL);
    }
    
    git_tree_free(gtree);
    git_signature_free(gsig);
    git_index_free(gidx);
    git_tree_free(gtree);

}

/*
void ab_git_add_bypath(git_repository *repo, const char *file)
{
    git_signature *sig;
    git_index *idx;
    git_tree *tree;
    
//    git_tree_lookup(&tree, repo, <#const git_oid *id#>)
    ab_util_signature_default(&sig, repo);
    ab_util_repository_index(&idx, repo);
    
    if (git_index_add_bypath(idx, file) < 0)
    {
        fatal("Unable to add a file", file);
    }
    
    git_index_free(idx);
    git_signature_free(sig);

}
*/

int main(int argc, const char * argv[])
{
    git_repository *gr;
    gr = NULL;

    char *repopath = NULL;
    int retval=0;
    int repopathLen=0;
    
    retval = 99;
    
    repopathLen=(int) strlen(argv[1]) + 1;
    
    repopath = malloc(repopathLen);
    
    strcpy(repopath, argv[1]);
    
    // insert code here...
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("Starting...\n");

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("Initializing GIT\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    git_threads_init();
    
    
/*
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("01 Repo Initialize\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    retval = ab_git_init(&gr, repopath);
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("01 Initial Commit\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    ab_create_initial_commit(gr);
*/
    
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("Cleaning up GIT\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    
    git_repository_free(gr);
    git_threads_shutdown();
    
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("Clearing memory\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    
    free(repopath);

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("End.\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

    
    return 0;
}

