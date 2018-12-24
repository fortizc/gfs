#ifndef GFS_REPO_H
#define GFS_REPO_H

struct _repo;
typedef struct _repo repo;

repo *repo_new();
void repo_free(repo *rp);
void repo_open(repo *rp, char *path);
void repo_clone(repo *rp, char *url, char *path, char *passphrase);
const char *repo_get_error(repo *rp);
int repo_get_errno(repo *rp);

#endif
