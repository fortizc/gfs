#include <fs/dirpath.h>
#include <utils/vector.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define DIRPATH_DEFAULT_NFILES 100


struct _dirpath
{
    char *name;
    git_blob *blob;
    dirpath_t *prev;
    vector_t *content;
    struct stat st;
};


dirpath_t *dirpath_new(const char *name, git_blob *blob, mode_t mode)
{
    dirpath_t *dp = (dirpath_t *) malloc(sizeof(dirpath_t));

    if (!dp)
        return NULL;

    dp->name = strdup(name);
    dp->blob = blob;
    dp->prev = NULL;

    time_t tm = time(NULL);
    dp->st.st_atime = tm;
    dp->st.st_mtime = tm;
    dp->st.st_uid = getuid();
    dp->st.st_gid = getgid();
    dp->st.st_mode = mode;

    if (mode & S_IFDIR)
    {
        dp->st.st_nlink = 2;
        dp->content = vector_new(DIRPATH_DEFAULT_NFILES);
        dp->st.st_size = 0;
    }
    else if (mode & S_IFREG)
    {
        dp->st.st_nlink = 1;
        dp->content = NULL;
        dp->st.st_size = (off_t) git_blob_rawsize(blob);
    }
    else
    {
        dp->st.st_nlink = 1;
        dp->content = NULL;
        dp->st.st_size = 0;
    }

    return dp;
}


void dirpath_free(dirpath_t *dp)
{
    if (dp->content)
    {
        size_t size = vector_get_size(dp->content);
        for (size_t i = 0; i < size; ++i)
        {
            dirpath_t *elem = vector_get(dp->content, i);

            if (elem->st.st_mode & S_IFDIR)
            {
                dirpath_free(elem);
            }
            else
            {
                free(elem->name);
                git_blob_free(elem->blob);
                free(elem);
            }
        }
    }

    free(dp->name);
    free(dp);
}


void dirpath_add(dirpath_t *dp, char *path, dirpath_t *new)
{
    if (path[0] != '/')
        return;

    dirpath_t *walker = NULL;
    char *dir = strtok(path, "/");

    walker = dp;
    while (dir)
    {
        vector_t *vec = walker->content;
        size_t size = vector_get_size(vec);

        for (size_t i = 0; i < size; ++i)
        {
            walker = vector_get(vec, i);
            if (strcmp(walker->name, dir) == 0)
                break;
        }
        dir = strtok(NULL, "/");
    }

    // we only add files or folders into folders
    if (!(walker->st.st_mode & S_IFDIR))
        return;

    vector_append(walker->content, new);
}
