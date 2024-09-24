#include "../include/prototypes.h"

void
run(const char *bin, const char *const *argv, const char *const *envv)
{
    int pid, status;
    if ((pid = fork()) == -1)
    {
        return;
    }
    else if (pid == 0)
    {
        if (envv == NULL)
        {
            execvp(bin, (char **)argv);
        }
        else
        {
            execvpe(bin, (char **)argv, (char **)envv);
        }
        exit(1);
    }
    else
    {
        if (waitpid(pid, &status, 0); WIFEXITED(status))
        {
            if (WEXITSTATUS(status) != 0)
            {
                fprintf(stderr, "%s: Exited with status: %i", bin, status);
            }
        }
        else
        {
            fprintf(stderr, "%s: Exited with unexpexted status %i", bin, status);
        }
    }
}

bool
file_size(Ulong *size, FILE *file)
{
    int prev = ftell(file);
    if (prev == -1)
    {
        return false;
    }
    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    if (*size == -1)
    {
        return false;
    }
    fseek(file, prev, SEEK_SET);
    return true;
}

void
copy_stack_nstr(char *stack_dst, const char *stack_src, Uint n) noexcept
{
    for (Uint i = 0; i < n; ++i)
    {
        stack_dst[i] = stack_src[i];
    }
}

void
extract_name_and_ext(compile_entry *e) noexcept
{
    for (Uint i = e->file_len; i > 0; --i)
    {
        if (e->file[i] == '.')
        {
            e->ext_len = e->file_len - i;
            for (Uint ext = 0; ext < (e->file_len - i); ++ext)
            {
                e->ext[ext] = e->file[ext + i];
            }
            e->name_len = i;
            for (Uint name = 0; name < i; ++name)
            {
                e->name[name] = e->file[name];
            }
            break;
        }
    }
}

bool
dir_exists(const char *path)
{
    struct stat st;
    if (access(path, R_OK) != 0)
    {
        return false;
    }
    if (S_ISDIR(st.st_mode))
    {
        return true;
    }
    return false;
}

compile_entry *
files_in_dir(const char *path, Ulong *n)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        return nullptr;
    }
    dirent        *entry;
    Uint           size = 0, cap = 10;
    compile_entry *buf = (compile_entry *)malloc(sizeof(compile_entry) * cap);
    for (; (entry = readdir(dir)); ++size)
    {
        (size == cap)
            ? ((cap *= 2), (buf = (compile_entry *)realloc(buf, sizeof(compile_entry) * cap)))
            : 0;
        compile_entry *e   = &buf[size];
        Uint           len = strlen(entry->d_name);
        copy_stack_nstr(e->file, entry->d_name, len);
        e->file[len] = '\0';
        e->type      = entry->d_type;
        e->file_len  = len;
    }
    *n = size;
    return buf;
}

void
free_files(compile_entry *files, Ulong n)
{
    free(files);
}
