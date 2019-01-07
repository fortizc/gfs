#ifndef GFS_FS_DIRPATH_H
#define GFS_FS_DIRPATH_H

#include <sys/stat.h>
#include <stddef.h>
#include <git2.h>

struct _dirpath;
typedef struct _dirpath dirpath_t;

dirpath_t *dirpath_new(const char *name, git_blob *blob, mode_t mode);

void dirpath_free(dirpath_t *dp);
void dirpath_get_stat(dirpath_t *dp, struct stat *st);
void dirpath_add(dirpath_t *dp, char *path, dirpath_t *new);


#endif
