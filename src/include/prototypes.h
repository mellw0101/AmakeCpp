#include "defenitions.h"

using std::string;
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

/* 'utils.cpp' */
void run(const char *bin, const char *const *argv, const char *const *envv) __NOT_NULL(1, 2);
void copy_stack_nstr(char *stack_dst, const char *stack_src, Uint n) noexcept;
void extract_name_and_ext(compile_entry *e) noexcept;
compile_entry *files_in_dir(const char *path, Ulong *n);
void           free_files(compile_entry *files, Ulong n);

/* 'compile.cpp'. */
void do_compile(void);
