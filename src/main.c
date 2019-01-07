#include <git/repo.h>
#include <stdio.h>

int main(int argc, char **argv)
{

    repo *rp = repo_new();

    char url[] = "git@github.com:uBiome/ubiome-pipeline-orchestrator.git";
                 // "git@github.com:uBiome/ubiome-pipeline-orchestrator.git"

    repo_clone(rp, url, "/home/fortiz/orco", 0);

    if (repo_get_errno(rp) != 0)
        printf("ERROR: %s\n", repo_get_error(rp));
    repo_free(rp);
    return 0;
}
