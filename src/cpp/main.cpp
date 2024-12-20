#include <Mlib/Args.h>
#include <Mlib/Sys.h>
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

namespace Bash_Helpers {
  // #!/bin/bash
  // # Get the architecture of the machine
  // ARCH=$(uname -m)
  // # Print the architecture
  // echo "Architecture: $ARCH"

  // # Perform actions based on the architecture
  // case "$ARCH" in
  //     x86_64)
  //         echo "This is a 64-bit x86 architecture."
  //         # Add your commands for x86_64 architecture here
  //         ;;
  //     aarch64)
  //         echo "This is a 64-bit ARM architecture."
  //         # Add your commands for ARM64 architecture here
  //         ;;
  //     armv7l)
  //         echo "This is a 32-bit ARM architecture."
  //         # Add your commands for ARM32 architecture here
  //         ;;
  //     *)
  //         	echo "Unknown architecture: $ARCH"
  //         	# Add your commands for other architectures here
  // 			;;
  // esac
}

vector<string> cleanObjVec(const vector<string> &vec) {
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
#if defined(__x86_64__) || defined(_M_X64)
  if (mode & BUILDARGS) {
    args = {"-c",
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
            output.data()};
  }
  else if (mode & LINKARGS) {
    args = {"-stdlib=libc++",
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
            "-l:libz.a"};
  }

#elif defined(__aarch64__) || defined(_M_ARM64)
  if (mode & BUILDARGS) {
    args = {"-funroll-loops",
            "-Rpass=loop-vectorize",
            "-Wall",
            "-Werror",
            "-O3",
            "-stdlib=libc++",
            "-std=c++20",
            "-c",
            file.data(),
            "-o",
            output.data()};
  }
  else if (mode & LINKARGS) {
    args = {"-stdlib=libc++",
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
            "-l:libz.a"};
  }

#elif defined(__arm__) || defined(_M_ARM)
#else
  printC("Unknown architecture.", ESC_CODE_YELLOW);
#endif
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
      CONF           = (1 << 2),
      VER            = (1 << 3),
      BUILD          = (1 << 4),
      CLEAN          = (1 << 5),
      INSTALL        = (1 << 6),
      LIB            = (1 << 7),
      TEST           = (1 << 8),
      LINK           = (1 << 9)
    };

    /* Convert string to Option */
		Option optionFromArg(const string &arg) {
      const static std::unordered_map<string, Option> optionMap = {
        {     "--help",    HELP},
        {"--configure",    CONF},
        {  "--version",     VER},
        {    "--build",   BUILD},
        {    "--clean",   CLEAN},
        {  "--install", INSTALL},
        {      "--lib",     LIB},
        {     "--test",    TEST},
        {     "--link",    LINK}
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
    ConfigureOption configureOptionFromArg(const string &arg) {
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
    void create_project_dir(const string &path, unsigned char mode = FileSys::NONE) {
      try {
        printC("Creating Dir -> " + path, ESC_CODE_GREEN);
        FileSys::mkdir(path, mode);
      }
      catch (exception const &e) {
        printC(e.what(), ESC_CODE_RED);
      }
    }

    void config_Amake_dir(void) {
      if (!FileSys::exists(AMAKE_CONF_DIR)) {
        create_project_dir(AMAKE_CONF_DIR, FileSys::NO_THROW);
      }
      if (!FileSys::exists(AMAKE_CPP_SIZES)) {
        FileSys::touch(AMAKE_CPP_SIZES);
      }
    }

    /* Compile (.cpp in src/cpp -> .o in build/obj) */
    void compile_cpp(void) {
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
    void link_binary(const vector<string> &strVec = {}) {
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

    void configure_project_dirs(void) {
      create_project_dir(SRC_DIR);
      create_project_dir(INCLUDE_DIR);
      create_project_dir(AS_DIR);
      create_project_dir(CPP_DIR);
      create_project_dir(C_DIR);
      create_project_dir(LIB_SRC_DIR);
      create_project_dir(BUILD_DIR);
      create_project_dir(OBJ_DIR);
      create_project_dir(BIN_DIR);
      create_project_dir(LIB_BUILD_DIR);
    }

    void configure_clang_format(void) {
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
                if (!FileSys::exists(cwd + "/.vscode")) {
                  create_project_dir(cwd + "/.vscode");
                }
                FileSys::writeStrVecToFile(cwd + "/.vscode/settings.json", vsCodeSettings);
                printC("Format on save configured", ESC_CODE_GREEN);
                break;
              }
              else {
                printC("Invalid answer, must be (Y/n)", ESC_CODE_RED);
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
      inline namespace Options {
        enum LibOption : Uint { UNKNOWN_LIB = (1 << 0), NCURSESW_STATIC = (1 << 1) };

        Uint get_lib_option(const string &lib) {
          const static unordered_map<string, LibOption> libOptionMap = {
              {"ncursesw-static", NCURSESW_STATIC},
              {             "ss", NCURSESW_STATIC}
          };
          const auto it = libOptionMap.find(lib);
          if (it != libOptionMap.end()) {
            return it->second;
          }
          return UNKNOWN_LIB;
        }
      }

      inline namespace Tools {
        /** @name printIL ( printInstallLib ). */
        void printIL(const string &libName) {
          printC("Installing Static Lib " + libName + " -> " + LIB_BUILD_DIR + "/" + libName, ESC_CODE_GREEN);
        }
      }

      vector<string> getLibInstallArgs(void) {
        vector<string> args;
#if defined(__x86_64__) || defined(_M_X64)
        args = {"--prefix=/usr/local", "--with-shared",   "--with-normal",
                "--enable-widec", "--enable-ext-colors", "--enable-static", "--disable-shared"};
#elif defined(__aarch64__) || defined(_M_ARM64)
        args = {"--prefix=/usr/local", "--with-shared",   "--with-normal",
                "--enable-widec", "--enable-ext-colors", "--enable-static", "--disable-shared"};
#elif defined(__arm__) || defined(_M_ARM)
        args = {"CC=clang", "CXX=clang++", "CFLAGS=-O3 --target=aarch64-linux-gnu -march=armv8-a",
                "CXXFLAGS=-O3 -march=armv8-a", "LDFLAGS=-O3 -march=armv8-a -flto"};
#endif
        return args;
      }

      vector<string> getLibInstallEnvArgs(void) {
        vector<string> args;
#if defined(__x86_64__) || defined(_M_X64)
        args = {"CC=clang", "CXX=clang++", "CFLAGS=-O3 -march=native", "CXXFLAGS=-O3 -march=native", "LDFLAGS=-O3 -flto"};
#elif defined(__aarch64__) || defined(_M_ARM64)
        args = {/* "CC=clang", "CXX=clang++", "CFLAGS=-O3 --target=aarch64-linux-gnu -march=armv8-a",
                "CXXFLAGS=-O3 --target=aarch64-linux-gnu -march=armv8-a", "LDFLAGS=-O3 -march=armv8-a -flto" */};
#elif defined(__arm__) || defined(_M_ARM)
        args = {"CC=clang", "CXX=clang++", "CFLAGS=-O3 --target=aarch64-linux-gnu -march=armv8-a",
                "CXXFLAGS=-O3 --target=aarch64-linux-gnu -march=armv8-a", "LDFLAGS=-O3 -march=armv8-a -flto"};
#endif
        return args;
      }

      int install_ncursesw_part(const string &libName) {
        printIL(libName);
        if (FileSys::exists(LIB_SRC_DIR + "/ncurses-6.5")) {
          printC("Folder lib/ncurses-6.5 Already Exists", ESC_CODE_YELLOW);
        }
        else {
          printC("Downloading ncurses-6.5.tar.gz", ESC_CODE_GREEN);
          if (FileSys::exists("/usr/bin/wget")) {
            try {
              Sys::run_binary("/usr/bin/wget", {"https://ftp.gnu.org/gnu/ncurses/ncurses-6.5.tar.gz", "-O",
                                                LIB_SRC_DIR + "/ncurses-6.5.tar.gz"});
            }
            catch (const exception &e) {
              printC(e.what(), ESC_CODE_RED);
              return 1;
            }
          }
          else if (FileSys::exists("/usr/bin/axel")) {
            try {
              Sys::run_binary("/usr/bin/axel", {"https://ftp.gnu.org/gnu/ncurses/ncurses-6.5.tar.gz", "-o",
                                                LIB_SRC_DIR + "/ncurses-6.5.tar.gz"});
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

      int install_ncursesw_static() {
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
    }
  }

  /* Show help message. */
  void Help(void) {
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
  void Configure(const string &subOption = "") {
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
  void Build(void) {
    compile_cpp();
  }

  /* Clean project, meaning remove build directory. */
  void Clean(void) {
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

  void Install(const vector<string> &strVec = {}) {
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

  void Lib(const string &str = "") {
    if (str.empty()) {
      printC("No library specified", ESC_CODE_RED);
      return;
    }
    const Uint option = get_lib_option(str);
    if (option & NCURSESW_STATIC) {
      install_ncursesw_static();
    }
    if (option & UNKNOWN_LIB) {
      printC("Unknown library ('Or Installer Not Implemented'): " + str, ESC_CODE_RED);
    }
  }
}

int main(int argc, char **argv) {
  const auto sArgv = Args::argvToStrVec(argc, argv);
  for (int i = 1; i < sArgv.size(); ++i) {
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
    if (option & CONF) {
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
          Lib(sArgv[i + 1]);
          ++i;
          continue;
        }
      }
      Lib();
    }
    if (option & UNKNOWN_OPTION) {
      printC("Error: Unknown option '" + sArgv[i] + "'. Run: " + PROJECT_NAME + " '--help' to display help msg",
             ESC_CODE_RED);
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
