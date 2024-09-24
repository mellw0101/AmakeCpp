#include <cstdio>
#include "../include/prototypes.h"

static Uint           tsize = 0;
static Uint           tcap  = 10;
static thread_data_t *td    = nullptr;
static pthread_t     *t     = nullptr;

void
init_threads(void)
{
    t  = (pthread_t *)malloc(sizeof(pthread_t) * tcap);
    td = (thread_data_t *)malloc(sizeof(thread_data_t) * tcap);
}

/* clang-format off */
void
add_thread_data(const char *input, const char *output)
{
    (tsize == tcap) ?
        ((tcap *= 2),
         (td = (thread_data_t *)realloc(td, sizeof(thread_data_t) * tcap)),
         (t  = (pthread_t *)realloc(t, sizeof(pthread_t) * tcap))) :
        0;
    thread_data_t *data = &td[tsize];
    Uint in_len  = strlen(input);
    Uint out_len = strlen(output);
    copy_stack_nstr(data->input, input, in_len);
    copy_stack_nstr(data->output, output, out_len);
    data->input[in_len]   = '\0';
    data->output[out_len] = '\0';
    ++tsize;
}

static void *
thread_cc_work(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    char cmd[PATH_MAX];
    snprintf(cmd, PATH_MAX, CC_COMPILER " -c %s " CC_DEFAULT_ARGS " -o %s", data->input, data->output);
    printf("%s\n", cmd);
    system(cmd);
    return nullptr;
}

static void *
thread_c_work(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    char cmd[PATH_MAX];
    snprintf(cmd, PATH_MAX, C_COMPILER " -c %s " C_DEFAULT_ARGS " -o %s", data->input, data->output);
    printf("%s\n", cmd);
    system(cmd);
    return nullptr;
}

void
do_cc(void)
{
    init_threads();
    Ulong n;
    compile_entry *files = files_in_dir(CPP_DIR.c_str(), &n);
    for (Ulong i = 0; i < n; ++i)
    {
        compile_entry *e = &files[i];
        if (e->type == 8)
        {
            extract_name_and_ext(e);
            char input[PATH_MAX];
            char output[PATH_MAX];
            snprintf(input, PATH_MAX, "%s/%s%s", CPP_DIR.c_str(), e->name, e->ext);
            snprintf(output, PATH_MAX, "%s/%s%s", OBJ_DIR.c_str(), e->name, ".o");
            add_thread_data(input, output);
        }
    }
    free(files);
    for (Uint i = 0; i < tsize; ++i)
    {
        pthread_create(&t[i], nullptr, thread_cc_work, &td[i]);
    }
    for (Uint i = 0; i < tsize; ++i)
    {
        pthread_join(t[i], nullptr);
    }
    free(t);
    free(td);
    printf("Total cc threads: %u\n", tsize);
}

void
do_c(void)
{
    init_threads();
    Ulong n;
    compile_entry *files = files_in_dir(C_DIR.c_str(), &n);
    for (Ulong i = 0; i < n; ++i)
    {
        compile_entry *e = &files[i];
        if (e->type == 8)
        {
            extract_name_and_ext(e);
            char input[PATH_MAX];
            char output[PATH_MAX];
            snprintf(input, PATH_MAX, "%s/%s%s", C_DIR.c_str(), e->name, e->ext);
            snprintf(output, PATH_MAX, "%s/%s%s", OBJ_DIR.c_str(), e->name, ".o");
            add_thread_data(input, output);
        }
    }
    free(files);
    for (Uint i = 0; i < tsize; ++i)
    {
        pthread_create(&t[i], nullptr, thread_c_work, &td[i]);
    }
    for (Uint i = 0; i < tsize; ++i)
    {
        pthread_join(t[i], nullptr);
    }
    free(t);
    free(td);
    printf("Total c threads: %u\n", tsize);
}

void
do_compile(void)
{
    do_cc();
    do_c();
    /* init_threads();
    Ulong          n;
    compile_entry *files = files_in_dir(CPP_DIR.c_str(), &n);
    for (Ulong i = 0; i < n; ++i)
    {
        compile_entry *e = &files[i];
        if (e->type == 8)
        {
            extract_name_and_ext(e);
            char input[PATH_MAX];
            char output[PATH_MAX];
            snprintf(input, PATH_MAX, "%s/%s%s", CPP_DIR.c_str(), e->name, e->ext);
            snprintf(output, PATH_MAX, "%s/%s%s", OBJ_DIR.c_str(), e->name, ".o");
            add_thread_data(input, output);
        }
    }
    free(files);
    for (Uint i = 0; i < tsize; ++i)
    {
        pthread_create(&t[i], nullptr, thread_cc_work, &td[i]);
    }
    for (Uint i = 0; i < tsize; ++i)
    {
        pthread_join(t[i], nullptr);
    }
    free(t);
    free(td);
    printf("Total threads: %u\n", tsize); */
}
/* clang-format on */
