/* clang-format off */
#include "defenitions.h"

using std::string;
using std::vector;
using namespace Mlib;

const string cwd         = FileSys::currentWorkingDir();
const string projectName = cwd.substr(cwd.find_last_of("/") + 1);

const string AMAKE_CONF_DIR  = cwd + "/.amake";
const string AMAKE_CPP_SIZES = AMAKE_CONF_DIR + "/cppSizes";

const string SRC_DIR     = cwd + "/src";
const string INCLUDE_DIR = SRC_DIR + "/include";
const string AS_DIR      = SRC_DIR + "/as";
const string CPP_DIR     = SRC_DIR + "/cpp";
const string C_DIR       = SRC_DIR + "/c";
const string LIB_SRC_DIR = SRC_DIR + "/lib";

const string BUILD_DIR     = cwd + "/build";
const string OBJ_DIR       = BUILD_DIR + "/obj";
const string BIN_DIR       = BUILD_DIR + "/bin";
const string LIB_BUILD_DIR = BUILD_DIR + "/lib";

extern char *pwd;
extern char *src_dir;
extern char *lib_src_dir;

void           printC(const string &str, const char *color);
vector<string> getArgsBasedOnArch(const Uchar mode, std::string_view output, std::string_view file = "");

/* 'utils.cpp' */
void      run(const char *bin, const char *const *argv, const char *const *envv) __NOT_NULL(1, 2);
void      copy_stack_nstr(char *stack_dst, const char *stack_src, Uint n) noexcept;
void      extract_name_and_ext(DirEntry *e) noexcept;
bool      dir_exists(const char *path);
bool      exec_exists(const char *name, char **fullpath_ret);
DirEntry *files_in_dir(const char *path, Ulong *n);
void      free_files(DirEntry *files, Ulong n);
void     *amalloc(Ulong howmush);
void     *arealloc(void *ptr, Ulong howmush);
char     *measured_copy(const char *string, Ulong count);
char     *copy_of(const char *string);
char     *get_pwd(void);
char     *get_src_dir(void);
char     *get_lib_src_dir(void);
char     *concatenate_path(const char *s1, const char *s2);
int       make_directory(const char *relativepath);
int       launch_bin(const char *binary, const char *const argv[], const char *const envp[]);
int       launch_bin(const char *binary, MVector<const char *> argv, MVector<const char *> envp);
void      extract_tar_gz(const char *path, const char *output_path);
void      extract_zip(const char *path, const char *output_path);

/* 'compile.cpp'. */
void do_compile(void);

/* 'link.cpp' */
void do_link(const vector<string> &strVec = {});
