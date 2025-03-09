#include <Mlib/Sys.h>
#include "../include/prototypes.h"

/* Link .o files in build/obj to binary in build/bin */
static void link_binary(const vector<string> &obj_vec, const vector<string> &strVec = {}) {
  const string output = cwd + "/build/bin/" + projectName;
  printC("Linking Obj Files -> " + output, ESC_CODE_GREEN);

  vector<string> linkArgsVec = getArgsBasedOnArch(LINKARGS, output);
  vector<string> libVec      = FileSys::dirContentToStrVec(LIB_BUILD_DIR);

  for (const auto &obj : obj_vec) {
    linkArgsVec.push_back(obj);
  }
  for (const auto &arg : strVec) {
    linkArgsVec.push_back(arg);
  }
  for (const string &lib : libVec) {
    linkArgsVec.push_back(lib);
  }
  if (dir_exists(string(LIB_SRC_DIR + "/bin").c_str())) {
    vector<string> lib_bins = FileSys::dirContentToStrVec(LIB_SRC_DIR + "/bin");
    for (const string &lib : lib_bins) {
      linkArgsVec.push_back(lib);
    }
  }
  try {
    Sys::run_binary("/usr/bin/clang++", linkArgsVec);
  }
  catch (exception const &e) {
    printC(e.what(), ESC_CODE_RED);
  }
}

void do_link(const vector<string> &strVec) {
  vector<string> in_files;
  Ulong          n;
  DirEntry      *files = files_in_dir(OBJ_DIR.c_str(), &n);
  for (Ulong i = 0; i < n; ++i) {
    DirEntry *e = &files[i];
    /* File. */
    if (e->type == 8) {
      extract_name_and_ext(e);
      char input[PATH_MAX];
      snprintf(input, PATH_MAX, "%s/%s%s", OBJ_DIR.c_str(), e->name, e->ext);
      in_files.push_back(input);
    }
    /* Dir. */
    else if (e->type == 4) {
      if (strcmp(e->file, ".") == 0 || strcmp(e->file, "..") == 0) {
        continue;
      }
      Ulong     sn;
      DirEntry *sub_files = files_in_dir(string(OBJ_DIR + "/" + e->file).c_str(), &sn);
      for (Ulong si = 0; si < sn; ++si) {
        DirEntry *se = &sub_files[si];
        if (se->type == 8) {
          printf("file: %s\n", se->file);
          extract_name_and_ext(se);
          char input[PATH_MAX];
          snprintf(input, PATH_MAX, "%s/%s/%s%s", OBJ_DIR.c_str(), e->file, se->name, se->ext);
          in_files.push_back(input);
        }
      }
    }
  }
  free(files);
  vector<string> args;
  bool           installBin = false;
  for (const string &arg : strVec) {
    if (arg == "--bin") {
      installBin = true;
    }
    else {
      args.push_back(arg);
    }
  }
  link_binary(in_files, args);
  if (installBin) {
    try {
      FileSys::fileContentToFile(BIN_DIR + "/" + projectName, "/usr/bin/" + projectName);
      printC(to_string(FileSys::fileSize("/usr/bin/" + projectName)) + " Bytes", ESC_CODE_GRAY);
    }
    catch (const exception &e) {
      printC(e.what(), ESC_CODE_RED);
    }
  }
}
