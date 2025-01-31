#include <Mlib/Args.h>
#include <Mlib/Sys.h>
#include <Mlib/Socket.h>
#include <string>
#include <unordered_map>
#include "../include/prototypes.h"

#define VERSION      "0.1"
#define PROJECT_NAME "AmakeCpp"
bool arm = false;

/**
  Print string with color.
  @param s: The string to print.
  @param color: The color code.
  Color codes available:
    ESC_CODE_RED
    ESC_CODE_GREEN
    ESC_CODE_YELLOW
    ESC_CODE_BLUE
    ESC_CODE_MAGENTA
    ESC_CODE_CYAN
    ESC_CODE_WHITE
    ESC_CODE_RESET
 */
void printC(const string &str, const char *color) {
  cout << PROJECT_NAME << " - [ " << color << str << ESC_CODE_RESET << " ]" << '\n';
}

static void print_msg(const char *color, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("%s - [ %s", PROJECT_NAME, color);
  vprintf(format, args);
  printf("%s ]\n", ESC_CODE_RESET);
  va_end(args);
}

namespace Bash_Helpers {
  /* #!/bin/bash
  # Get the architecture of the machine
  ARCH=$(uname -m)
  # Print the architecture
  echo "Architecture: $ARCH"

  # Perform actions based on the architecture
  case "$ARCH" in
      x86_64)
          echo "This is a 64-bit x86 architecture."
          # Add your commands for x86_64 architecture here
          ;;
      aarch64)
          echo "This is a 64-bit ARM architecture."
          # Add your commands for ARM64 architecture here
          ;;
      armv7l)
          echo "This is a 32-bit ARM architecture."
          # Add your commands for ARM32 architecture here
          ;;
      *)
            echo "Unknown architecture: $ARCH"
            # Add your commands for other architectures here
            ;;
  esac */
}

_UNUSED static vector<string> cleanObjVec(const vector<string> &vec) {
  vector<string> cleanVec;
#if defined(__x86_64__) || defined(_M_X64)
  for (const string &str : vec) {
    if (str.find(".o") != string::npos) {
      cleanVec.push_back(str);
    }
  }
#elif defined(__aarch64__) || defined(_M_ARM64)
  for (const string &str : vec) {
    if (str.find(".arm.o") != string::npos) {
      cleanVec.push_back(str);
    }
  }
#elif defined(__arm__) || defined(_M_ARM)
  for (const string &str : vec) {
    if (str.find(".arm.o") != string::npos) {
      cleanVec.push_back(str);
    }
  }
#endif
  return cleanVec;
}

vector<string> getArgsBasedOnArch(const Uchar mode, string_view output, string_view file) {
  vector<string> args;
  if (mode & BUILDARGS) {
    args = {
#if defined(__x86_64__) || defined(_M_X64)
        "-c",
        "-O3",
        "-march=native",
        "-funroll-loops",
        "-Rpass=loop-vectorize",
        "-flto",
        "-m64",
        "-Wall",
        "-Werror",
        "-static",
        "-stdlib=libc++",
        "-std=c++23",
        "-Wno-vla",
        file.data(),
        "-o",
        output.data()
#elif defined(__aarch64__) || defined(_M_ARM64)
        "-funroll-loops",
        "-Rpass=loop-vectorize",
        "-Wall",
        "-Werror",
        "-O3",
        "-stdlib=libc++",
        "-std=c++20",
        "-c",
        file.data(),
        "-o",
        output.data()
#endif
    };
  }
  else if (mode & LINKARGS) {
    args = {
#if defined(__x86_64__) || defined(_M_X64)
        "-stdlib=libc++",
        "-std=c++23",
        "-s",
        "-flto",
        "-O3",
        "-march=native",
        "-o",
        output.data(),
        "/usr/lib/Mlib.a",
        "-L/usr/lib",
        "-l:libc++.a",
        "-l:libc++abi.a",
        "-l:libz.a"
#elif defined(__aarch64__) || defined(_M_ARM64)
        "-stdlib=libc++",
        "-std=c++20",
        "-s",
        "-flto",
        "-O3",
        "-march=armv8-a",
        "-o",
        output.data(),
        "/usr/lib/Mlib.a",
        "-L/usr/lib",
        "-l:libc++.a",
        "-l:libc++abi.a",
        "-l:libz.a"
#endif
    };
  }
  return args;
}

inline namespace ConfigStrVecS {
  /* clang-format off */
  const vector<string> clang_format = {
    "Language: Cpp",
    "BasedOnStyle: LLVM",
    "AccessModifierOffset: -4",
    "AlignConsecutiveAssignments: true",
    "AlignConsecutiveDeclarations: true",
    "AlignOperands: AlignAfterOperator",
    "AlignTrailingComments:",
    "  Kind: Always",
    "  OverEmptyLines: 2",
    "AllowShortBlocksOnASingleLine: Empty",
    "AllowShortCaseLabelsOnASingleLine: false",
    "AllowShortFunctionsOnASingleLine: None",
    "AllowShortIfStatementsOnASingleLine: false",
    "AllowShortLoopsOnASingleLine: false",
    "BreakConstructorInitializers: BeforeComma",
    "BreakBeforeBinaryOperators: None",
    "AlwaysBreakAfterDefinitionReturnType: All",
    "AlwaysBreakAfterReturnType: None",
    "AlwaysBreakTemplateDeclarations: Yes",
    "BinPackArguments: true",
    "BinPackParameters: true",
    "BraceWrapping:",
    "  AfterCaseLabel: true",
    "  AfterClass: true",
    "  AfterControlStatement: true",
    "  AfterEnum: true",
    "  AfterFunction: true",
    "  AfterNamespace: True",
    "  AfterStruct: true",
    "  AfterUnion: true",
    "  AfterExternBlock: true",
    "  AfterObjCDeclaration: false",
    "  BeforeCatch: true",
    "  BeforeElse: true",
    "  BeforeLambdaBody: true",
    "  BeforeWhile: true",
    "  IndentBraces: false",
    "  SplitEmptyFunction: false",
    "  SplitEmptyRecord: false",
    "  SplitEmptyNamespace: false",
    "BreakBeforeBraces: Custom",
    "BreakBeforeTernaryOperators: true",
    "ColumnLimit: 120",
    "CompactNamespaces: false",
    "ContinuationIndentWidth: 4",
    "EmptyLineAfterAccessModifier: Never",
    "EmptyLineBeforeAccessModifier: LogicalBlock",
    "IncludeBlocks: Preserve",
    "IncludeCategories:",
    "  - Regex: '^<.*'",
    "    Priority: 1",
    "  - Regex: '^\".*'",
    "    Priority: 2",
    "  - Regex: '.*'",
    "    Priority: 3",
    "IncludeIsMainRegex: '([-_](test|unittest))?$'",
    "IndentCaseBlocks: false",
    "IndentCaseLabels: true",
    "IndentGotoLabels: true",
    "IndentPPDirectives: AfterHash",
    "IndentRequiresClause: True",
    "IndentWidth: 4",
    "IndentWrappedFunctionNames: false",
    "InsertBraces: True",
    "InsertNewlineAtEOF: True",
    "MacroBlockBegin: ''",
    "MacroBlockEnd: ''",
    "MaxEmptyLinesToKeep: 1",
    "NamespaceIndentation: All",
    "SpaceAfterCStyleCast: false",
    "SpaceAfterLogicalNot: false",
    "SpaceAfterTemplateKeyword: true",
    "SpaceBeforeAssignmentOperators: True",
    "SpaceBeforeCaseColon: true",
    "SpaceBeforeCpp11BracedList: True",
    "SpaceBeforeInheritanceColon: true",
    "SpaceBeforeParens: ControlStatements",
    "SpaceBeforeRangeBasedForLoopColon: true",
    "SpaceBeforeSquareBrackets: false",
    "SpaceInEmptyParentheses: false",
    "SpacesBeforeTrailingComments: 1",
    "SpacesInAngles: false",
    "SpacesInContainerLiterals: true",
    "SpacesInConditionalStatement: False",
    "BracedInitializerIndentWidth: 4",
    "SpacesInCStyleCastParentheses: false",
    "SpacesInParentheses: false",
    "SpacesInSquareBrackets: false",
    "TabWidth: 4",
    "UseTab: Never",
    "PointerAlignment: Right",
    "Standard: Latest",
    "AlignAfterOpenBracket: Align",
    "AlignArrayOfStructures: Right",
    "AllowAllParametersOfDeclarationOnNextLine: false",
    "AllowAllArgumentsOnNextLine: false",
    "AllowShortLambdasOnASingleLine: None",
    "LambdaBodyIndentation: Signature",
    "AlignConsecutiveMacros:",
    "  Enabled: true",
    "  AcrossEmptyLines: true",
    "  AcrossComments: true",
    "  AlignCompound: true",
    "  PadOperators: true",
    "AlignConsecutiveShortCaseStatements:",
    "  Enabled: true",
    "  AcrossEmptyLines: true",
    "  AcrossComments: true",
    "  AlignCaseColons: true",
    "AlignEscapedNewlines: Left",
    "BreakAfterAttributes: Always",
    "PenaltyBreakBeforeFirstCallParameter: 0"
  };

  const vector<string> vsCodeSettings = {
    "{",
    "    \"editor.formatOnSave\": true,",
    "    \"editor.defaultFormatter\": \"xaver.clang-format\",",
    "    \"clang-format.executable\": \"/usr/bin/clang-format\",",
    "    \"clang-format.style\": \"file\",",
    "    \"editor.foldingHighlight\": false,",
    "    \"C_Cpp.intelliSenseEngine\": \"disabled\",",
    "    \"breadcrumbs.showTypeParameters\": false,",
    "    \"editor.suggest.showTypeParameters\": false,",
    "    \"clangd.arguments\": [",
    "        \"--header-insertion=never\",",
    "        \"--header-insertion-decorators\"",
    "    ],",
    "    \"clangd.enableCodeCompletion\": true,",
    "    \"editor.semanticHighlighting.enabled\": true,",
    "    \"editor.inlayHints.enabled\": \"offUnlessPressed\",",
    "    \"accessibility.signals.noInlayHints\": {",
    "        \"sound\": \"off\",",
    "        \"announcement\": \"off\"",
    "    },",
    "    \"clangd.path\": \"/usr/bin/clangd\",",
    "    \"editor.folding\": true,",
    "    \"editor.foldingImportsByDefault\": true,",
    "    \"editor.unfoldOnClickAfterEndOfLine\": false,",
    "    \"[cpp]\": {",
    "        \"editor.foldingStrategy\": \"auto\",",
    "        \"editor.showFoldingControls\": \"always\"",
    "    },",
    "    \"editor.foldingStrategy\": \"auto\",",
    "    \"editor.showFoldingControls\": \"always\",",
    "    \"editor.defaultFoldingRangeProvider\": \"llvm-vs-code-extensions.vscode-clangd\",",
    "}"
  };

  const static vector<string> compileArgs {
    "-O3",   "-march=native", "-funroll-loops", "-Rpass=loop-vectorize", "-flto", "-m64",
    "-Wall", "-Werror",       "-static",        "-stdlib=libc++",
  };
}

inline namespace AmakeCpp {
  inline namespace Options {
    /* Enum representing cli options */
    enum Option {
      UNKNOWN_OPTION = (1 << 0),
      HELP           = (1 << 1),
      CONFIG           = (1 << 2),
      VER            = (1 << 3),
      BUILD          = (1 << 4),
      CLEAN          = (1 << 5),
      INSTALL        = (1 << 6),
      LIB            = (1 << 7),
      TEST           = (1 << 8),
      LINK           = (1 << 9),
      CONFIG_CHECK   = (1 << 10)
    };

    /* Convert string to Option */
		Option optionFromArg(const string &arg) {
      const static std::unordered_map<string, Option> optionMap = {
        {     "--help",         HELP},
        {"--configure",       CONFIG},
        {  "--version",          VER},
        {    "--build",        BUILD},
        {    "--clean",        CLEAN},
        {  "--install",      INSTALL},
        {      "--lib",          LIB},
        {     "--test",         TEST},
        {     "--link",         LINK},
        {    "--check", CONFIG_CHECK}
      };
      const auto it = optionMap.find(arg);
      if (it != optionMap.end()) {
        return it->second;
      }
      return UNKNOWN_OPTION;
    }

    /* Enum representing configure sub options */
    enum ConfigureOption { UNKNOWN_CONFIGURE_OPTION = (1 << 0), CLANG_FORMAT = (1 << 1) };

    /* Convert string to ConfigureOption */
    static ConfigureOption configureOptionFromArg(const string &arg) {
      const static unordered_map<string, ConfigureOption> configureOptionMap = {
        {"--clang-format", CLANG_FORMAT}
      };
      const auto it = configureOptionMap.find(arg);
      if (it != configureOptionMap.end()) {
        return it->second;
      }
      return CLANG_FORMAT;
    }
  }

  inline namespace Tools {
    /* Creates a directory */
    static void create_project_dir(const char *relativepath) {
      char *fullpath = concatenate_path(get_pwd(), relativepath);
      if (!dir_exists(fullpath)) {
        print_msg(ESC_CODE_GREEN, "Creating Dir -> %s", fullpath);
        if (make_directory(fullpath) == -1) {
          perror("make_directory");
        }
      }
      free(fullpath);
    }

    static void config_Amake_dir(void) {
      create_project_dir(".amake");
      // if (!FileSys::exists(AMAKE_CONF_DIR)) {
      //   create_project_dir(AMAKE_CONF_DIR, FileSys::NO_THROW);
      // }
      if (!FileSys::exists(AMAKE_CPP_SIZES)) {
        FileSys::touch(AMAKE_CPP_SIZES);
      }
    }

    /* Compile (.cpp in src/cpp -> .o in build/obj) */
    static void compile_cpp(void) {
      config_Amake_dir();
      printC("Compiling .cpp -> .o", ESC_CODE_GREEN);
      try {
        auto           cppSizes        = FileSys::fileContentToStrVec(AMAKE_CPP_SIZES);
        auto           cppSizesToPrint = cppSizes;
        vector<string> files           = FileSys::dirContentToStrVec(CPP_DIR);
        for (const auto &file : files) {
          const string fileName   = file.substr(file.find_last_of("/") + 1);
          bool         hasChanged = true;
          if (!cppSizes.empty()) {
            for (const auto &cppSize : cppSizes) {
              const string fileId = cppSize.substr(0, cppSize.find_first_of(':'));
              if (fileId == fileName) {
                const string size = cppSize.substr(cppSize.find_first_of(':') + 1);
                if (size == to_string(FileSys::fileSize(file))) {
                  bool alreadyInVec = false;
                  for (const auto &cppSize : cppSizes) {
                    if (cppSize == fileName + ":" + size) {
                      alreadyInVec = true;
                      break;
                    }
                  }
                  if (!alreadyInVec) {
                    cppSizesToPrint.push_back(fileName + ":" + to_string(FileSys::fileSize(file)));
                  }
                  hasChanged = false;
                }
                else {
                  printC("Recompiling " + file + " -> " + OBJ_DIR + "/" +
                             fileName.substr(0, fileName.find_last_of(".")) + ".o",
                         ESC_CODE_YELLOW);
                }
              }
            }
          }
          if (!hasChanged) {
            continue;
          }
          const string         objName = OBJ_DIR + "/" + fileName.substr(0, fileName.find_last_of(".")) + ".o";
          const vector<string> args    = getArgsBasedOnArch(BUILDARGS, objName, file);
          try {
            Sys::run_binary("/usr/bin/clang++", args);

            printC(file + " -> " + objName, ESC_CODE_BLUE);
            printC(".cpp File Size: " + to_string(FileSys::fileSize(file)) + " Bytes" +
                       " .o File Size: " + to_string(FileSys::fileSize(objName)) + " Bytes",
                   ESC_CODE_GRAY);

            for (unsigned int i = 0; i < cppSizesToPrint.size(); ++i) {
              if (cppSizesToPrint[i].find(fileName) != string::npos) {
                Args::eraseFromVector(cppSizesToPrint, cppSizesToPrint[i]);
              }
            }
            cppSizesToPrint.push_back(fileName + ":" + to_string(FileSys::fileSize(file)));
          }
          catch (const exception &e) {
            printC(e.what(), ESC_CODE_RED);
            exit(EXIT_FAILURE);
          }
          FileSys::rmFile(AMAKE_CPP_SIZES);
          FileSys::writeStrVecToFile(AMAKE_CPP_SIZES, cppSizesToPrint);
        }
      }
      catch (const exception &e) {
        printC(e.what(), ESC_CODE_RED);
        exit(EXIT_FAILURE);
      }
    }

    /* Link .o files in build/obj to binary in build/bin */
    static void link_binary(const vector<string> &strVec = {}) {
      const string output = cwd + "/build/bin/" + projectName;
      printC("Linking Obj Files -> " + output, ESC_CODE_GREEN);
      vector<string> objVec      = FileSys::dirContentToStrVec(OBJ_DIR);
      vector<string> linkArgsVec = getArgsBasedOnArch(LINKARGS, output);
      vector<string> libVec      = FileSys::dirContentToStrVec(LIB_BUILD_DIR);
      for (const auto &obj : objVec) {
        linkArgsVec.push_back(obj);
      }
      for (const auto &arg : strVec) {
        linkArgsVec.push_back(arg);
      }
      for (const string &lib : libVec) {
        linkArgsVec.push_back(lib);
      }
      try {
        Sys::run_binary("/usr/bin/clang++", linkArgsVec);
      }
      catch (exception const &e) {
        printC(e.what(), ESC_CODE_RED);
      }
    }

    static void configure_project_dirs(void) {
      create_project_dir("src");
      create_project_dir("src/include");
      create_project_dir("src/as");
      create_project_dir("src/cpp");
      create_project_dir("src/c");
      create_project_dir("src/lib");
      create_project_dir("build");
      create_project_dir("build/obj");
      create_project_dir("build/bin");
      create_project_dir("build/lib");
    }

    static void configure_clang_format(void) {
      printC("Configuring Clang-Format", ESC_CODE_GREEN);
      try {
        while (true) {
          string answer = Sys::Prompt("Do you want to add standard .clang-format file? (y/n): ");
          if (answer == "n" || answer == "N" || answer == "no" || answer == "NO" || answer == "No") {
            break;
          }
          else if (answer == "y" || answer == "Y" || answer == "yes" || answer == "YES" || answer == "Yes") {
            FileSys::writeStrVecToFile(cwd + "/.clang-format", clang_format);
            printC("Clang-Format configured", ESC_CODE_GREEN);
            while (true) {
              string answer = Sys::Prompt("Do you want format on save in vscode? (y/n): ");
              if (answer == "n" || answer == "N" || answer == "no" || answer == "NO" || answer == "No") {
                break;
              }
              else if (answer == "y" || answer == "Y" || answer == "yes" || answer == "YES" || answer == "Yes") {
                create_project_dir("/.vscode");
                FileSys::writeStrVecToFile(cwd + "/.vscode/settings.json", vsCodeSettings);
                printC("Format on save configured", ESC_CODE_GREEN);
                break;
              }
              else {
                print_msg(ESC_CODE_RED, "Invalid answer, must be (Y/n)");
              }
            }
            break;
          }
          else {
            printC("Invalid answer, must be (Y/n)", ESC_CODE_RED);
          }
        }
      }
      catch (const exception &e) {
        printC(e.what(), ESC_CODE_RED);
      }
    }

    inline namespace Libs {
      static vector<string> getLibInstallArgs(void) {
        return {
        #if defined(__x86_64__) || defined(_M_X64)
          "--prefix=/usr/local", "--with-shared",   "--with-normal", "--enable-widec", "--enable-ext-colors", "--enable-static", "--disable-shared"
        #elif defined(__aarch64__) || defined(_M_ARM64)
          "--prefix=/usr/local", "--with-shared",   "--with-normal", "--enable-widec", "--enable-ext-colors", "--enable-static", "--disable-shared"
        #endif
        };
      }

      static vector<string> getLibInstallEnvArgs(void) {
        return {
        #if defined(__x86_64__) || defined(_M_X64)
          "CC=clang", "CXX=clang++", "CFLAGS=-O3 -march=native", "CXXFLAGS=-O3 -march=native", "LDFLAGS=-O3 -flto"
        #elif defined(__aarch64__) || defined(_M_ARM64)
        #endif
        };
      }

      static int install_ncursesw_part(const string &libName) {
        print_msg(ESC_CODE_GREEN, "Installing Lib %s -> %s/build/lib/%s", libName.c_str(), get_pwd(), libName.c_str());
        if (FileSys::exists(LIB_SRC_DIR + "/ncurses-6.5")) {
          print_msg(ESC_CODE_YELLOW, "Folder lib/ncurses-6.5 Already Exists");
        }
        else {
          print_msg(ESC_CODE_YELLOW, "Downloading ncurses-6.5.tar.gz");
          char *fullpath;
          if (exec_exists("wget", &fullpath)) {
            try {
              Sys::run_binary(fullpath, {"https://ftp.gnu.org/gnu/ncurses/ncurses-6.5.tar.gz", "-O", LIB_SRC_DIR + "/ncurses-6.5.tar.gz"});
            }
            catch (const exception &e) {
              printC(e.what(), ESC_CODE_RED);
              return 1;
            }
            free(fullpath);
          }
          else if (FileSys::exists("/usr/bin/axel")) {
            try {
              Sys::run_binary("/usr/bin/axel", {"https://ftp.gnu.org/gnu/ncurses/ncurses-6.5.tar.gz", "-o", LIB_SRC_DIR + "/ncurses-6.5.tar.gz"});
            }
            catch (const exception &e) {
              printC(e.what(), ESC_CODE_RED);
              return 1;
            }
            if (FileSys::exists(LIB_SRC_DIR + "/ncurses-6.5.tar.gz")) {
              printC("Successfully Downloaded ncurses-6.5.tar.gz", ESC_CODE_GREEN);
            }
            else {
              printC("Failed to download ncurses-6.5.tar.gz", ESC_CODE_RED);
              return 1;
            }
          }
          else {
            if (FileSys::exists("/usr/bin/pacman")) {
              try {
                Sys::run_binary("/usr/bin/sudo", {"/usr/bin/pacman", "-S", "--needed", "--noconfirm", "wget"});
              }
              catch (const exception &e) {
                printC(e.what(), ESC_CODE_RED);
                return 1;
              }
            }
            else if (FileSys::exists("/usr/bin/apt")) {
              try {
                Sys::run_binary("/usr/bin/sudo", {"/usr/bin/apt", "install", "-y", "wget"});
              }
              catch (const exception &e) {
                printC(e.what(), ESC_CODE_RED);
                return 1;
              }
            }
            else {
              printC("No download manager found", ESC_CODE_RED);
              return 1;
            }
          }
          if (FileSys::exists("/usr/bin/tar")) {
            printC(
                "Extracting " + LIB_SRC_DIR + "/ncurses-6.5.tar.gz -> " + LIB_SRC_DIR + "/ncurses-6.5", ESC_CODE_GREEN);
            try {
              Sys::run_binary("/usr/bin/tar", {"-xzf", LIB_SRC_DIR + "/ncurses-6.5.tar.gz", "-C", LIB_SRC_DIR + "/"});
            }
            catch (const exception &e) {
              printC(e.what(), ESC_CODE_RED);
              return 1;
            }
            if (!FileSys::exists(LIB_SRC_DIR + "/ncurses-6.5")) {
              printC("Failed to extract ncurses-6.5.tar.gz", ESC_CODE_RED);
              return 1;
            }
            printC("Successfully Extracted ncurses-6.5.tar.gz", ESC_CODE_GREEN);
            FileSys::rmFile(LIB_SRC_DIR + "/ncurses-6.5.tar.gz");
          }
          else {
            printC("tar Not found", ESC_CODE_RED);
            return 1;
          }
        }
        try {
          FileSys::cd(LIB_SRC_DIR + "/ncurses-6.5");
          string binary_path = "./configure";

          vector<string> args = getLibInstallArgs();

          vector<string> env_vars = getLibInstallEnvArgs();

          if (!FileSys::exists("lib/" + libName)) {
            Sys::run_binary(binary_path, args, env_vars);
            Sys::run_binary("/usr/bin/make", {"-j", "4"});
          }
          if (FileSys::exists(LIB_BUILD_DIR + "/" + libName)) {
            FileSys::rmFile(LIB_BUILD_DIR + "/" + libName);
          }
          FileSys::fileContentToFile(LIB_SRC_DIR + "/ncurses-6.5/lib/" + libName, LIB_BUILD_DIR + "/" + libName);
          FileSys::cd(cwd);
        }
        catch (const exception &e) {
          printC(e.what(), ESC_CODE_RED);
          return 1;
        }
        if (FileSys::exists(LIB_BUILD_DIR + "/" + libName)) {
          printC("Sizes " + to_string(FileSys::fileSize(LIB_SRC_DIR + "/ncurses-6.5/lib/" + libName)) + " -> " +
                     to_string(FileSys::fileSize(LIB_BUILD_DIR + "/" + libName)),
                 ESC_CODE_GRAY);
        }
        return EXIT_SUCCESS;
      }

      static int install_ncursesw_static(void) {
        install_ncursesw_part("libncurses++w.a");
        install_ncursesw_part("libncursesw.a");
        install_ncursesw_part("libformw.a");
        install_ncursesw_part("libmenuw.a");
        install_ncursesw_part("libpanelw.a");
        FileSys::cd(LIB_SRC_DIR + "/ncurses-6.5");
        Sys::run_binary("sudo", {"make", "install"});
        FileSys::rmdir(LIB_SRC_DIR + "/ncurses-6.5");
        return EXIT_SUCCESS;
      }

      static int download_glfw_lib(void) {
        int ret = 0;
        create_project_dir("src");
        create_project_dir("src/lib");
        create_project_dir("src/lib/glfw");
        char *glfwdir = concatenate_path(get_lib_src_dir(), "glfw");
        char *output_file = concatenate_path(glfwdir, "source.zip");
        char *bin_path;
        if (exec_exists("wget", &bin_path) || exec_exists("axel", &bin_path)) {
          launch_bin(bin_path, ARGV(bin_path, "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip", "-O", output_file), ARGV(NULL));
          sync();
          extract_zip(output_file, glfwdir);
        }
        else {
          print_msg(ESC_CODE_RED, "Could not find wget or axel to download source.");
          ret = -1;
        }
        free(glfwdir);
        free(output_file);
        free(bin_path);
        return ret;
      }

      static int install_glfw_lib(void) {
        int ret = 0;
        char *was_pwd = copy_of(get_pwd());
        char *glfw_dir = concatenate_path(get_lib_src_dir(), "glfw/glfw-3.4");
        chdir(glfw_dir);
        char *build_dir = concatenate_path(glfw_dir, "build");
        make_directory(build_dir);
        chdir(build_dir);
        char *bin;
        if (exec_exists("cmake", &bin)) {
          launch_bin(bin, ARGV(bin, "-DBUILD_SHARED_LIBS=OFF", "-DCMAKE_MAKE_PROGRAM=make", "-DCMAKE_C_COMPILER=clang", ".."), PARENT_ENV);
          launch_bin(bin, ARGV(bin, "--build", ".", "--parallel", "16"), PARENT_ENV);
          char *lib_path = concatenate_path(glfw_dir, "build/src/libglfw3.a");
          char *lib_bin_path = concatenate_path(get_lib_src_dir(), "bin");
          make_directory(lib_bin_path);
          launch_bin("/bin/cp", ARGV("/bin/cp", lib_path, lib_bin_path), PARENT_ENV);
          free(lib_path);
          free(lib_bin_path);
          char *include_path = concatenate_path(get_lib_src_dir(), "include");
          make_directory(include_path);
          char *lib_include_path = concatenate_path(get_lib_src_dir(), "glfw/glfw-3.4/include/GLFW/");
          launch_bin("/bin/cp", ARGV("/bin/cp", "-r", lib_include_path, include_path), PARENT_ENV);
          free(include_path);
          free(lib_include_path);
        }
        else {
          print_msg(ESC_CODE_RED, "Please install cmake.");
          ret = -1;
        }
        free(glfw_dir);
        free(build_dir);
        free(pwd);
        pwd = was_pwd;
        chdir(pwd);
        return ret;
      }

      static int download_glew_lib(void) {
        int ret = 0;
        create_project_dir("src");
        create_project_dir("src/lib");
        create_project_dir("src/lib/glew");
        char *glewdir = concatenate_path(get_lib_src_dir(), "glew");
        char *output_file = concatenate_path(glewdir, "source.zip");
        char *bin_path;
        if (exec_exists("wget", &bin_path) || exec_exists("axel", &bin_path)) {
          launch_bin(bin_path, ARGV(bin_path, "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip", "-O", output_file), ARGV(NULL));
          sync();
          extract_zip(output_file, glewdir);
        }
        else {
          print_msg(ESC_CODE_RED, "Could not find wget or axel to download source.");
          ret = -1;
        }
        free(glewdir);
        free(output_file);
        free(bin_path);
        return ret;
      }
      
      static int install_glew_lib(void) {
        int ret = 0;
        char *was_pwd = copy_of(get_pwd());
        char *glew_dir = concatenate_path(get_lib_src_dir(), "glew/glew-2.2.0");
        chdir(glew_dir);
        char *bin;
        if (exec_exists("make", &bin)) {
          launch_bin(bin, ARGV(bin, "-j36"), PARENT_ENV);
          char *lib_path = concatenate_path(glew_dir, "lib/libGLEW.a");
          char *lib_bin_path = concatenate_path(get_lib_src_dir(), "bin");
          make_directory(lib_bin_path);
          launch_bin("/bin/cp", ARGV("/bin/cp", lib_path, lib_bin_path), PARENT_ENV);
          free(lib_path);
          free(lib_bin_path);
          char *include_path = concatenate_path(get_lib_src_dir(), "include");
          make_directory(include_path);
          char *lib_include_path = concatenate_path(glew_dir, "include/GL");
          launch_bin("/bin/cp", ARGV("/bin/cp", "-r", lib_include_path, include_path), PARENT_ENV);
          free(include_path);
          free(lib_include_path);
        }
        else {
          print_msg(ESC_CODE_RED, "Please install cmake.");
          ret = -1;
        }
        free(glew_dir);
        free(pwd);
        pwd = was_pwd;
        chdir(pwd);
        return ret;
      }
    }
  }

  /* Show help message. */
  static void Help(void) {
    cout << "Usage: " << PROJECT_NAME << " [options]\n"
         << "Options:\n"
         << "   --help                      Show this help message\n"
         << "   --version                   Show version information\n"
         << "   --configure                 Configure current dir as project\n"
         << "   configure options:\n"
         << "       none                    (default) Create Project in current directory\n"
         << "       --clang-format          Configure .clang-format file for project\n"
         << "   --build                     Build project\n"
         << "   --clean                     Clean project\n"
         << "   --install                   Install project\n";
  }

  /* Configure current directory as project. */
  static void Configure(const string &subOption = "") {
    if (subOption.empty()) {
      configure_project_dirs();
    }
    else {
      const ConfigureOption option = configureOptionFromArg(subOption);
      if (option & CLANG_FORMAT) {
        try {
          configure_clang_format();
        }
        catch (exception const &e) {
          printC(e.what(), ESC_CODE_RED);
        }
      }
      if (option & UNKNOWN_CONFIGURE_OPTION) {
        printC("Unknown sub option '" + subOption + "', run 'Amake --help'", ESC_CODE_RED);
      }
    }
  }

  /* Build project. */
  static void Build(void) {
    compile_cpp();
  }

  /* Clean project, meaning remove build directory. */
  static void Clean(void) {
    printC("Cleaning project", ESC_CODE_GREEN);
    if (fs::exists(BUILD_DIR) && fs::is_directory(BUILD_DIR)) {
      fs::remove_all(BUILD_DIR);
    }
    if (fs::exists(BUILD_DIR) && fs::is_directory(BUILD_DIR)) {
      printC("Failed to clean project", ESC_CODE_RED);
    }
    else {
      printC("Project cleaned", ESC_CODE_GREEN);
    }
  }

  static void Install(const vector<string> &strVec = {}) {
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
    link_binary(args);
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

  static void Lib(const char *name) {
    if (strcmp(name, "ncursesw-static") == 0) {
      install_ncursesw_static();
    }
    else if (strcmp(name, "glfw-static") == 0) {
      if (download_glfw_lib() == -1) {
        return;
      }
      install_glfw_lib();
    }
    else if (strcmp(name, "glew-static") == 0) {
      if (download_glew_lib() == -1) {
        return;
      }
      install_glew_lib();
    }
    else {
      print_msg(ESC_CODE_RED, "Unknown library ('Or Installer Not Implemented'): %s", name);
    }
  }
}

int main(int argc, char **argv) {
  const auto sArgv = Args::argvToStrVec(argc, argv);
  for (Ulong i = 1; i < sArgv.size(); ++i) {
    const Option option = optionFromArg(sArgv[i]);
    if (option & TEST) {
      if (i + 1 < sArgv.size()) {
        vector<string> args;
        while (optionFromArg(sArgv[i + 1]) == UNKNOWN_OPTION && i + 1 < sArgv.size()) {
          ++i;
          args.push_back(sArgv[i]);
        }
        if (!args.empty()) {
          do_link(args);
          continue;
        }
      }
      do_compile();
      exit(0);
    }
    if (option & LINK) {
      if (i + 1 < sArgv.size()) {
        vector<string> args;
        while (optionFromArg(sArgv[i + 1]) == UNKNOWN_OPTION && i + 1 < sArgv.size()) {
          ++i;
          args.push_back(sArgv[i]);
        }
        if (!args.empty()) {
          do_link(args);
          continue;
        }
      }
      do_link();
      exit(0);
    }
    if (option & HELP) {
      Help();
    }
    if (option & CONFIG) {
      if (i + 1 < sArgv.size()) {
        if (optionFromArg(sArgv[i + 1]) & UNKNOWN_OPTION) {
          Configure(sArgv[i + 1]);
          exit(EXIT_SUCCESS);
        }
      }
      Configure();
    }
    if (option & VER) {
      cout << "Version: " << VERSION << '\n';
    }
    if (option & BUILD) {
      Build();
    }
    if (option & CLEAN) {
      Clean();
    }
    if (option & INSTALL) {
      if (i + 1 < sArgv.size()) {
        vector<string> args;
        while (optionFromArg(sArgv[i + 1]) == UNKNOWN_OPTION && i + 1 < sArgv.size()) {
          ++i;
          args.push_back(sArgv[i]);
        }
        if (!args.empty()) {
          Install(args);
          continue;
        }
      }
      Install();
    }
    if (option & LIB) {
      if (i + 1 < sArgv.size()) {
        if (optionFromArg(sArgv[i + 1]) & UNKNOWN_OPTION) {
          Lib(sArgv[i + 1].c_str());
          ++i;
          continue;
        }
      }
      Lib("");
    }
    if (option & CONFIG_CHECK) {
      check_config_part(CONFIG_HAVE___THREAD);
      exit(0);
    }
    if (option & UNKNOWN_OPTION) {
      printC("Error: Unknown option '" + sArgv[i] + "'. Run: " + PROJECT_NAME + " '--help' to display help msg",
             ESC_CODE_RED);
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
