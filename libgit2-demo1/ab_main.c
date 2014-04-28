//
//  ab_git_add.c
//  libgit2-demo1
//
//  Created by Marvin Torio on 4/11/14.
//  Copyright (c) 2014 Martian Madness. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "git2.h"
#include "ab_util.h"

static int cred_acquire(git_cred **out, const char *url,
                        const char *username_from_url, unsigned int allowed_types,
                        void *payload)
{
	char username[128] = { 0 };
	char password[128] = { 0 };
    
	printf("Username: ");
	scanf("%s", username);
    
	/* Yup. Right there on your terminal. Careful where you copy/paste output. */
	printf("Password: ");
	scanf("%s", password);
    
	return git_cred_userpass_plaintext_new(out, username, password);
}

void write_target_file(const char *filename)
{
    time_t rawtime;
    struct tm *info;
    
    FILE *fp;
    
    time(&rawtime);
    info = localtime(&rawtime);
    
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        exit(printf("Unable to open file %s for writing", filename));
    }
    fprintf(fp, "TIMESTAMP: %s", asctime(info));
    
    
    fclose(fp);
}


int main_addfile(int argc, const char * argv[])
{
    git_repository *gr;
    char *repopath = NULL;
    int r=0;
    int pid;
    char spid[50];
    char filename[500];
    char full_filename[1024];
    char commit_comment[1024];
    
    git_signature *gsig;
    git_index *gidx;
    git_oid tree_id, commit_id;
    git_tree *gtree;
    git_commit *old_head;
    
    get_repopath(&repopath, "REPO_HOME");

    logit("REPO_LOCATION",repopath);
    
    pid = getpid();
    sprintf(spid,"08%d",pid);
    sprintf(filename,"%s.txt",spid);
    sprintf(full_filename, "%s/%s", repopath, filename);
    
    logit("PID",spid);
    logit("WRITING FULL FILE",full_filename);
    
    write_target_file(full_filename);
    
    r = git_repository_open(&gr, repopath);
    logit("OPENING REPO", repopath);
    
    check_error(r, "opening repository");
    

    logit("Generating signature", "");
    r=git_signature_default(&gsig, gr);
    check_error(r, "Unable to generate a signature");
    logit("SIGNATURE", gsig->name);
    
    r=git_repository_index(&gidx, gr);
    check_error(r, "Unable to generate an index");
    
    r=git_index_add_bypath(gidx, filename);
    check_error(r, "Unable to add to index");
    
    r=git_index_write(gidx);
    check_error(r, "Unable to write index");
    
    r=git_index_write_tree(&tree_id, gidx);
    check_error(r, "Unable to write tree");
    
    r=git_tree_lookup(&gtree, gr, &tree_id);
    check_error(r, "Unable to lookup tree");
    
    r=git_revparse_single((git_object**) &old_head, gr, "HEAD");
    check_error(r, "Unable to determine soon to be old HEAD");
    
    sprintf(commit_comment, "TEST COMMIT pid=%s", spid);
    
    r=git_commit_create_v(&commit_id, gr, "HEAD", gsig, gsig, NULL, commit_comment, gtree, 1, old_head);
    check_error(r, "Unable to commit");
    
    logit("FREEING MEMORY", "");
    free(gsig);
    free(gidx);
    free(gtree);
    free(repopath);
    
    logit("END","PROCESS");
    return 0;
}


int main_fetch(int argc, char *argv[])
{
    git_repository *gr;
    char *repopath = NULL;
    int r=0;
    
    git_remote *grmt;
    const git_transfer_progress *stats;
    
    
    get_repopath(&repopath, "REPO_HOME");
    
    logit("REPO_LOCATION",repopath);

    r = git_repository_open(&gr, repopath);
    logit("OPENING REPO", repopath);

    
    r = git_remote_load(&grmt, gr, "github-origin");
    check_error(r, "Unable to load remote");
    logit("LOADED REMOTE","github-origin");

    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    callbacks.credentials = cred_acquire;
    git_remote_set_callbacks(grmt, &callbacks);
    logit("SET CALLBACKS", NULL);
    
//    r = git_cred_ssh_key_new(&gcred, "git", "/Users/marvintorio/.ssh/id_rsa.pub", "/Users/marvintorio/.ssh/id_rsa", "2sik4u");
//    check_error(r, "Unable to create new ssh key");
    
    r = git_remote_connect(grmt, GIT_DIRECTION_FETCH);
    check_error(r, "Unable to conduct remote connect");
    logit("CONNECTED TO REMOTE", NULL);
    
    r = git_remote_download(grmt);
    check_error(r, "Unable to download");
    
    stats = git_remote_stats(grmt);

    printf("================================================\n");
    printf("total objects   : %d\n", stats->total_objects);
    printf("total deltas    : %d\n", stats->total_deltas);
    printf("received objects: %d\n", stats->received_objects);
    printf("received bytes  : %zu\n", stats->received_bytes);
    printf("indexed objects : %d\n", stats->indexed_objects);
    printf("indexed deltas  : %d\n", stats->indexed_deltas);
    printf("================================================\n\n");
    
    free(gr);
    free(grmt);
    
    return 0;
}

int main_fetch2(int argc, char *argv[])
{
    git_repository *gr;
    char *repopath = NULL;
    int r=0;
    char s[512];
    
    git_remote *grmt;
    const git_transfer_progress *stats;
    
    git_cred *gcred;
    
    get_repopath(&repopath, "REPO_HOME");
    
    logit("REPO_LOCATION",repopath);
    
    r = git_repository_open(&gr, repopath);
    logit("OPENING REPO", repopath);

    
//    git_remote_create_inmemory(&grmt, gr, "+refs/heads/*:refs/custom/namespace/*", "https://github.com/libgit2/libgit2");

    r = git_remote_create_anonymous(&grmt, gr, "ssh://git@github.com:marvtorio/mylibgit2demo.git", "+refs/heads/*:refs/custom/namespace/*");
    
    check_error(r, "Unable to create anonymous remote");
    
/*
    strcpy(s, "origin");
    r = git_remote_load(&grmt, gr, s);
    check_error(r, "Unable to load remote");
    logit("LOADED REMOTE", s);
    
    r = git_remote_connect(grmt, GIT_DIRECTION_FETCH);
    check_error(r, "Unable to conduct remote connect");
    logit("CONNECTED TO REMOTE", NULL);
    
    r = git_remote_download(grmt);
    check_error(r, "Unable to download");
*/
    
    free(gr);
    free(grmt);
    
    return 0;
}


int main(int argc, char *argv[])
{
    main_fetch2(argc, argv);
}
