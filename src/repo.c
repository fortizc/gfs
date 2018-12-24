#include <git2.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <git/repo.h>


#define PRIVATE_KEY_PATH "/.ssh/id_rsa"
#define PUBLIC_KEY_PATH "/.ssh/id_rsa.pub"


struct _repo
{
    char *error;
    int errno;
    git_repository *repo;
};

void repo_set_error(repo *rp)
{
    const git_error *e = giterr_last();

    if (!e)
        return;

    rp->errno = e->klass;

    if (rp->error)
        free(rp->error);
    rp->error = strdup(e->message);
}

int repo_credentials_cb(git_cred **out,
                        const char *url,
                        const char *username_from_url,
                        unsigned allowed_types,
                        void *payload)
{
    if (!(GIT_CREDTYPE_SSH_KEY & allowed_types))
        return -1;

    uid_t uid = geteuid();
    struct passwd *p = getpwuid(uid);

    size_t pw_dir_sz = strlen(p->pw_dir);
    size_t pub_key_path_sz = strlen(PUBLIC_KEY_PATH);
    size_t priv_key_path_sz = strlen(PRIVATE_KEY_PATH);

    size_t pub_key_sz = pw_dir_sz + pub_key_path_sz + 1;
    size_t priv_key_sz = pw_dir_sz + priv_key_path_sz + 1;

    // public key path
    char pub_key[pub_key_sz];
    memcpy(&pub_key, p->pw_dir, pw_dir_sz);
    memcpy(&(pub_key[pw_dir_sz]), PUBLIC_KEY_PATH, pub_key_path_sz);
    pub_key[pub_key_sz - 1] = 0;

    // private key path
    char priv_key[priv_key_sz];
    memcpy(&priv_key, p->pw_dir, priv_key_sz);
    memcpy(&(priv_key[pw_dir_sz]), PRIVATE_KEY_PATH, priv_key_path_sz);
    priv_key[priv_key_sz - 1] = 0;

    char *passphrase = (char *)payload;

    if (!passphrase)
        return git_cred_ssh_key_new(out, username_from_url, pub_key, priv_key, "");
    return git_cred_ssh_key_new(out, username_from_url, pub_key, priv_key, passphrase);
}

// public API

void repo_free(repo *rp)
{
    if (rp->repo)
        git_repository_free(rp->repo);

    if (rp->error)
        free(rp->error);

    git_libgit2_shutdown();
    free(rp);
}

repo *repo_new()
{
    repo *rp = (repo *) malloc(sizeof(repo));

    if (!rp)
        return NULL;

    rp->repo = NULL;
    rp->error = NULL;
    rp->errno = 0;

    git_libgit2_init();

    return rp;
}


void repo_clone(repo *rp, char *url, char *path, char *passphrase)
{

    int errno = 0;
    git_clone_options clone_opts;
    errno = git_clone_init_options(&clone_opts, GIT_CLONE_OPTIONS_VERSION);

    if (errno != 0)
        goto clone_end;

    errno = git_checkout_init_options(&clone_opts.checkout_opts,
                                      GIT_CHECKOUT_OPTIONS_VERSION);
    if (errno != 0)
        goto clone_end;

    clone_opts.fetch_opts.callbacks.credentials = repo_credentials_cb;
    clone_opts.fetch_opts.callbacks.payload = passphrase;

    errno = git_clone(&rp->repo, url, path, &clone_opts);
    if (errno != 0)
        goto clone_end;

clone_end:
    repo_set_error(rp);
}


int repo_get_errno(repo *rp)
{
    return rp->errno;
}

const char *repo_get_error(repo *rp)
{
    return rp->error;
}
