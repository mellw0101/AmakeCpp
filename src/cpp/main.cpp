#include <Mlib/Args.h>
#include <Mlib/FileSys.h>
#include <Mlib/Sys.h>

#include <unordered_map>

#define VERSION      "0.1"
#define PROJECT_NAME "AmakeCpp"

using namespace std;
using namespace Mlib;

string const cwd            = FileSys::currentWorkingDir();
string const projectName    = cwd.substr(cwd.find_last_of("/") + 1);
string const AMAKE_CONF_DIR = cwd + "/.amake";
string const SRC_DIR        = cwd + "/src";
string const INCLUDE_DIR    = SRC_DIR + "/include";
string const AS_DIR         = SRC_DIR + "/as";
string const CPP_DIR        = SRC_DIR + "/cpp";
string const C_DIR          = SRC_DIR + "/c";
string const LIB_SRC_DIR    = SRC_DIR + "/lib";
string const BUILD_DIR      = cwd + "/build";
string const OBJ_DIR        = BUILD_DIR + "/obj";
string const BIN_DIR        = BUILD_DIR + "/bin";
string const LIB_BUILD_DIR  = BUILD_DIR + "/lib";

namespace ConfigStrVecS {
    const vector<string> clang_format = {"Language: Cpp",
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
                                         "  AfterNamespace: False",
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
                                         "MaxEmptyLinesToKeep: 2",
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
                                         "PenaltyBreakBeforeFirstCallParameter: 0"};

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
        "}"};
    // ConstructorInitializerIndentWidth: 4
    // DerivePointerAlignment: false
    // IndentExternBlock: AfterExternBlock
    // IndentFunctionDeclarationAfterType: true

    const static vector<string> compileArgs {
        "-O3",   "-march=native", "-funroll-loops", "-Rpass=loop-vectorize", "-flto", "-m64",
        "-Wall", "-Werror",       "-static",        "-stdlib=libc++",
    };
} // namespace ConfigStrVecS
using namespace ConfigStrVecS;

namespace AmakeCpp {
    namespace Options {
        /// @enum Option
        /// @brief
        /// - Enum representing cli options
        enum Option
        {
            UNKNOWN_OPTION = (1 << 0),
            HELP           = (1 << 1),
            CONF           = (1 << 2),
            VER            = (1 << 3),
            BUILD          = (1 << 4),
            CLEAN          = (1 << 5),
            INSTALL        = (1 << 6),
            LIB            = (1 << 7)
        };

        /// @name optionFromArg
        /// @brief
        /// - Convert string to Option
        /// @param arg
        /// - string to convert
        /// @returns Option
        /// @note
        /// - Option is an @enum
        Option
        optionFromArg(const string &arg)
        {
            const static unordered_map<string, Option> optionMap = {
                {     "--help",    HELP},
                {"--configure",    CONF},
                {  "--version",     VER},
                {    "--build",   BUILD},
                {    "--clean",   CLEAN},
                {  "--install", INSTALL},
                {      "--lib",     LIB}
            };
            const auto it = optionMap.find(arg);
            if (it != optionMap.end())
            {
                return it->second;
            }
            return UNKNOWN_OPTION;
        }

        /// @enum ConfigureOption
        /// @brief
        /// - Enum representing configure sub options
        enum ConfigureOption
        {
            UNKNOWN_CONFIGURE_OPTION = (1 << 0),
            CLANG_FORMAT             = (1 << 1)
        };

        /// @name configureOptionFromArg
        /// @brief
        /// - Convert string to ConfigureOption
        /// @param arg
        /// - string to convert
        /// @returns ConfigureOption
        /// @note
        /// - ConfigureOption is an @enum
        ConfigureOption
        configureOptionFromArg(const string &arg)
        {
            const static unordered_map<string, ConfigureOption> configureOptionMap = {
                {"--clang-format", CLANG_FORMAT}
            };
            const auto it = configureOptionMap.find(arg);
            if (it != configureOptionMap.end())
            {
                return it->second;
            }
            return CLANG_FORMAT;
        }
    } // namespace Options
    using namespace Options;
    namespace Tools {
        /// @name printC
        /// @brief Print string with color
        /// @param s string to print
        /// @param color color code
        /// @note
        /// - color codes available:
        ///   - ESC_CODE_RED
        ///   - ESC_CODE_GREEN
        ///   - ESC_CODE_YELLOW
        ///   - ESC_CODE_BLUE
        ///   - ESC_CODE_MAGENTA
        ///   - ESC_CODE_CYAN
        ///   - ESC_CODE_WHITE
        ///   - ESC_CODE_RESET
        /// @returns void
        void
        printC(const string &str, const s8 *color)
        {
            cout << PROJECT_NAME << " - [ " << color << str << ESC_CODE_RESET << " ]" << '\n';
        }

        /// @name createProjectDir
        /// @brief
        /// - Create project directory
        /// @param subPath
        /// - Sub path to create
        /// @note
        /// - Creates directory in current working directory ( cwd + subPath )
        /// @returns void
        void
        createProjectDir(const string &path)
        {
            try
            {
                printC("Creating Dir -> " + path, ESC_CODE_GREEN);
                FileSys::mkdir(path);
            }
            catch (exception const &e)
            {
                printC(e.what(), ESC_CODE_RED);
            }
        }

        /// @name compileCpp
        /// @brief
        /// - Compile ( .cpp -> .o ) in src/cpp
        /// @returns void
        void
        compileCpp()
        {
            printC("Compiling .cpp -> .o", ESC_CODE_GREEN);
            try
            {
                vector<string> files = FileSys::dirContentToStrVec(cwd + "/src/cpp");
                for (const auto &file : files)
                {
                    string fileName     = file.substr(file.find_last_of("/") + 1);
                    string objName      = cwd + "/build/obj/" + fileName.substr(0, fileName.find_last_of(".")) + ".o";
                    vector<string> args = {
                        "-c",   "-O3",   "-march=native", "-funroll-loops", "-Rpass=loop-vectorize", "-flto",
                        "-m64", "-Wall", "-Werror",       "-static",        "-stdlib=libc++",        "-std=c++23",
                        file,   "-o",    objName};
                    try
                    {
                        Sys::run_binary("/usr/bin/clang++", args);
                        printC(file + " -> " + objName, ESC_CODE_BLUE);
                        printC(".cpp File Size: " + to_string(FileSys::fileSize(file)) + " Bytes" +
                                   " .o File Size: " + to_string(FileSys::fileSize(objName)) + " Bytes",
                               ESC_CODE_GRAY);
                    }
                    catch (const exception &e)
                    {
                        printC(e.what(), ESC_CODE_RED);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            catch (const exception &e)
            {
                printC(e.what(), ESC_CODE_RED);
                exit(EXIT_FAILURE);
            }
        }

        /// @name linkBinary
        /// @brief
        /// - Link .o files in build/obj to binary in build/bin
        /// @returns void
        void
        linkBinary(const vector<string> &strVec = {})
        {
            printC("Linking Obj Files -> " + cwd + "/build/bin/" + projectName, ESC_CODE_GREEN);
            vector<string> objVec = FileSys::dirContentToStrVec(OBJ_DIR);
            vector<string> libVec = FileSys::dirContentToStrVec(LIB_BUILD_DIR);

            vector<string> linkArgsVec = {"-stdlib=libc++",
                                          "-std=c++23",
                                          "-s",
                                          "-flto",
                                          "-O3",
                                          "-march=native",
                                          "-o",
                                          cwd + "/build/bin/" + projectName,
                                          "/usr/lib/Mlib.a",
                                          "-L/usr/lib",
                                          "-l:libc++.a",
                                          "-l:libc++abi.a",
                                          "-l:libunwind.a",
                                          "-l:libz.a"};
            for (const auto &obj : objVec)
            {
                linkArgsVec.push_back(obj);
            }
            for (const auto &arg : strVec)
            {
                linkArgsVec.push_back(arg);
            }
            linkArgsVec.push_back("-L" + LIB_BUILD_DIR);
            for (const string &lib : libVec)
            {
                const string libName = "-l:" + lib.substr(lib.find_last_of("/") + 1);
                linkArgsVec.push_back(lib);
            }
            try
            {
                Sys::run_binary("/usr/bin/clang++", linkArgsVec);
            }
            catch (exception const &e)
            {
                printC(e.what(), ESC_CODE_RED);
            }
        }

        void
        configureProjectDirs()
        {
            createProjectDir(SRC_DIR);
            createProjectDir(INCLUDE_DIR);
            createProjectDir(AS_DIR);
            createProjectDir(CPP_DIR);
            createProjectDir(C_DIR);
            createProjectDir(LIB_SRC_DIR);
            createProjectDir(BUILD_DIR);
            createProjectDir(OBJ_DIR);
            createProjectDir(BIN_DIR);
            createProjectDir(LIB_BUILD_DIR);
        }

        void
        configureClangFormat()
        {
            printC("Configuring Clang-Format", ESC_CODE_GREEN);
            try
            {
                while (true)
                {
                    string answer = Sys::Prompt("Do you want to add standard .clang-format file? (y/n): ");
                    if (answer == "n" || answer == "N" || answer == "no" || answer == "NO" || answer == "No")
                    {
                        break;
                    }
                    else if (answer == "y" || answer == "Y" || answer == "yes" || answer == "YES" || answer == "Yes")
                    {
                        FileSys::writeStrVecToFile(cwd + "/.clang-format", clang_format);
                        printC("Clang-Format configured", ESC_CODE_GREEN);
                        while (true)
                        {
                            string answer = Sys::Prompt("Do you want format on save in vscode? (y/n): ");
                            if (answer == "n" || answer == "N" || answer == "no" || answer == "NO" || answer == "No")
                            {
                                break;
                            }
                            else if (answer == "y" || answer == "Y" || answer == "yes" || answer == "YES" ||
                                     answer == "Yes")
                            {
                                if (!FileSys::exists(cwd + "/.vscode"))
                                {
                                    createProjectDir(cwd + "/.vscode");
                                }
                                FileSys::writeStrVecToFile(cwd + "/.vscode/settings.json", vsCodeSettings);
                                printC("Format on save configured", ESC_CODE_GREEN);
                                break;
                            }
                            else
                            {
                                printC("Invalid answer, must be (Y/n)", ESC_CODE_RED);
                            }
                        }
                        break;
                    }
                    else
                    {
                        printC("Invalid answer, must be (Y/n)", ESC_CODE_RED);
                    }
                }
            }
            catch (const exception &e)
            {
                printC(e.what(), ESC_CODE_RED);
            }
        }

        namespace Libs {
            namespace Options {
                enum LibOption
                {
                    UNKNOWN_LIB     = (1 << 0),
                    NCURSESW_STATIC = (1 << 1)
                };

                LibOption
                getLibOption(const string &lib)
                {
                    const static unordered_map<string, LibOption> libOptionMap = {
                        {"ncursesw-static", NCURSESW_STATIC}
                    };
                    const auto it = libOptionMap.find(lib);
                    if (it != libOptionMap.end())
                    {
                        return it->second;
                    }
                    return UNKNOWN_LIB;
                }
            } // namespace Options
            using namespace Options;
            namespace Tools {
                /// @name printIL ( printInstallLib )
                void
                printIL(const string &libName)
                {
                    printC("Installing Static Lib " + libName + " -> " + LIB_BUILD_DIR + "/" + libName, ESC_CODE_GREEN);
                }
            } // namespace Tools
            using namespace Tools;

            int
            installNcursesPart(const string &libName)
            {
                printIL(libName);
                if (FileSys::exists(LIB_SRC_DIR + "/ncurses-6.3"))
                {
                    printC("Folder lib/ncurses-6.3 Already Exists", ESC_CODE_YELLOW);
                }
                else
                {
                    printC("Downloading ncurses-6.3.tar.gz", ESC_CODE_GREEN);
                    if (FileSys::exists("/usr/bin/wget"))
                    {
                        try
                        {
                            Sys::run_binary("/usr/bin/wget", {"https://ftp.gnu.org/gnu/ncurses/ncurses-6.3.tar.gz"});
                        }
                        catch (const exception &e)
                        {
                            printC(e.what(), ESC_CODE_RED);
                            return 1;
                        }
                    }
                    else if (FileSys::exists("/usr/bin/axel"))
                    {
                        try
                        {
                            Sys::run_binary("/usr/bin/axel", {"https://ftp.gnu.org/gnu/ncurses/ncurses-6.3.tar.gz",
                                                              "-o", LIB_SRC_DIR + "/ncurses-6.3.tar.gz"});
                        }
                        catch (const exception &e)
                        {
                            printC(e.what(), ESC_CODE_RED);
                            return 1;
                        }
                        if (FileSys::exists(LIB_SRC_DIR + "/ncurses-6.3.tar.gz"))
                        {
                            printC("Successfully Downloaded ncurses-6.3.tar.gz", ESC_CODE_GREEN);
                        }
                        else
                        {
                            printC("Failed to download ncurses-6.3.tar.gz", ESC_CODE_RED);
                            return 1;
                        }
                    }
                    else
                    {
                        if (FileSys::exists("/usr/bin/pacman"))
                        {
                            try
                            {
                                Sys::run_binary(
                                    "/usr/bin/sudo", {"/usr/bin/pacman", "-S", "--needed", "--noconfirm", "wget"});
                            }
                            catch (const exception &e)
                            {
                                printC(e.what(), ESC_CODE_RED);
                                return 1;
                            }
                        }
                        else if (FileSys::exists("/usr/bin/apt"))
                        {
                            try
                            {
                                Sys::run_binary("/usr/bin/sudo", {"/usr/bin/apt", "install", "-y", "wget"});
                            }
                            catch (const exception &e)
                            {
                                printC(e.what(), ESC_CODE_RED);
                                return 1;
                            }
                        }
                        else
                        {
                            printC("No download manager found", ESC_CODE_RED);
                            return 1;
                        }
                    }
                    if (FileSys::exists("/usr/bin/tar"))
                    {
                        printC("Extracting " + LIB_SRC_DIR + "/ncurses-6.3.tar.gz -> " + LIB_SRC_DIR + "/ncurses-6.3",
                               ESC_CODE_GREEN);
                        try
                        {
                            Sys::run_binary(
                                "/usr/bin/tar", {"-xzf", LIB_SRC_DIR + "/ncurses-6.3.tar.gz", "-C", LIB_SRC_DIR + "/"});
                        }
                        catch (const exception &e)
                        {
                            printC(e.what(), ESC_CODE_RED);
                            return 1;
                        }
                        if (!FileSys::exists(LIB_SRC_DIR + "/ncurses-6.3"))
                        {
                            printC("Failed to extract ncurses-6.3.tar.gz", ESC_CODE_RED);
                            return 1;
                        }
                        printC("Successfully Extracted ncurses-6.3.tar.gz", ESC_CODE_GREEN);
                        FileSys::rmFile(LIB_SRC_DIR + "/ncurses-6.3.tar.gz");
                    }
                    else
                    {
                        printC("tar Not found", ESC_CODE_RED);
                        return 1;
                    }
                }
                try
                {
                    FileSys::cd(LIB_SRC_DIR + "/ncurses-6.3");
                    string binary_path = "./configure";

                    vector<string> args = {"--prefix=/usr/local", "--with-shared",   "--with-normal",
                                           "--enable-widec",      "--enable-static", "--disable-shared"};

                    vector<string> env_vars = {"CC=clang", "CXX=clang++", "CFLAGS=-O3 -march=native",
                                               "CXXFLAGS=-O3 -march=native", "LDFLAGS=-O3 -march=native -flto"};

                    if (!FileSys::exists("lib/" + libName))
                    {
                        Sys::run_binary(binary_path, args, env_vars);
                        Sys::run_binary("/usr/bin/make", {"-j", "4"});
                    }
                    if (!FileSys::exists(LIB_BUILD_DIR + "/" + libName))
                    {
                        FileSys::fileContentToFile(
                            LIB_SRC_DIR + "/ncurses-6.3/lib/" + libName, LIB_BUILD_DIR + "/" + libName);
                    }
                    FileSys::cd(cwd);
                }
                catch (const exception &e)
                {
                    printC(e.what(), ESC_CODE_RED);
                    return 1;
                }
                if (FileSys::exists(LIB_BUILD_DIR + "/" + libName))
                {
                    printC("Sizes " + to_string(FileSys::fileSize(LIB_SRC_DIR + "/ncurses-6.3/lib/" + libName)) +
                               " -> " + to_string(FileSys::fileSize(LIB_BUILD_DIR + "/" + libName)),
                           ESC_CODE_GRAY);
                }
                return EXIT_SUCCESS;
            }

            int
            installNcurseswStatic()
            {
                installNcursesPart("libncurses++w.a");
                installNcursesPart("libncursesw.a");
                installNcursesPart("libformw.a");
                installNcursesPart("libmenuw.a");
                installNcursesPart("libpanelw.a");
                FileSys::cd(LIB_SRC_DIR + "/ncurses-6.3");
                Sys::run_binary("sudo", {"make", "install"});
                FileSys::rmdir(LIB_SRC_DIR + "/ncurses-6.3");
                return EXIT_SUCCESS;
            }
        } // namespace Libs
        using namespace Libs;
    }     // namespace Tools
    using namespace Tools;

    /// @name Help
    /// @brief
    /// - Show help message
    /// @returns void
    void
    Help()
    {
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

    /// @name Configure
    /// @brief
    /// - Configure current directory as project
    /// @param subOptions
    /// - Sub options for configure
    /// @returns void
    void
    Configure(const string &subOption = "")
    {
        if (subOption.empty())
        {
            configureProjectDirs();
        }
        else
        {
            const ConfigureOption option = configureOptionFromArg(subOption);
            if (option & CLANG_FORMAT)
            {
                try
                {
                    configureClangFormat();
                }
                catch (exception const &e)
                {
                    printC(e.what(), ESC_CODE_RED);
                }
            }
            if (option & UNKNOWN_CONFIGURE_OPTION)
            {
                printC("Unknown sub option '" + subOption + "', run 'Amake --help'", ESC_CODE_RED);
            }
        }
    }

    /// @name Build
    /// @brief
    /// - Build project
    void
    Build()
    {
        compileCpp();
    }

    /// @name Clean
    /// @brief
    /// - Clean project meaning remove build directory
    /// @returns void
    void
    Clean()
    {
        try
        {
            printC("Cleaning project", ESC_CODE_GREEN);
            FileSys::rmdir(cwd + "/build");
            if (FileSys::exists(cwd + "/build"))
            {
                printC("Failed to clean project", ESC_CODE_RED);
            }
            else
            {
                printC("Project cleaned", ESC_CODE_GREEN);
            }
        }
        catch (const exception &e)
        {
            printC(e.what(), ESC_CODE_RED);
        }
    }

    /// @name Install
    /// @brief
    /// - Install project
    /// @returns void
    void
    Install(const vector<string> &strVec = {})
    {
        vector<string> args;
        bool           installBin = false;
        for (const string &arg : strVec)
        {
            if (arg == "--bin")
            {
                installBin = true;
            }
            else
            {
                args.push_back(arg);
            }
        }
        linkBinary(args);
        if (installBin)
        {
            FileSys::fileContentToFile(BIN_DIR + "/" + projectName, "/usr/bin/" + projectName);
			printC(to_string(FileSys::fileSize("/usr/bin/" + projectName)) + " Bytes", ESC_CODE_GRAY);
        }
    }

    void
    Lib(const vector<string> &strVec = {})
    {
        if (strVec.empty())
        {
            printC("No library specified", ESC_CODE_RED);
            return;
        }
        for (const auto &lib : strVec)
        {
            const LibOption option = getLibOption(lib);
            if (option & NCURSESW_STATIC)
            {
                installNcurseswStatic();
            }
            if (option & UNKNOWN_LIB)
            {
                printC("Unknown library( Or Installer Not Implemented ): " + lib, ESC_CODE_RED);
            }
        }
    }
} // namespace AmakeCpp
using namespace AmakeCpp;

s32
main(s32 argc, s8 **argv)
{
    const auto sArgv = Args::argvToStrVec(argc, argv);
    for (s32 i = 1; i < sArgv.size(); ++i)
    {
        Option const option = optionFromArg(sArgv[i]);
        if (option & HELP)
        {
            Help();
        }
        if (option & CONF)
        {

            if (i + 1 < sArgv.size())
            {
                if (optionFromArg(sArgv[i + 1]) & UNKNOWN_OPTION)
                {
                    Configure(sArgv[i + 1]);
                    ++i;
                    break;
                }
            }
            Configure();
        }
        if (option & VER)
        {
            cout << "Version: " << VERSION << '\n';
        }
        if (option & BUILD)
        {
            Build();
        }
        if (option & CLEAN)
        {
            Clean();
        }
        if (option & INSTALL)
        {
            if (i + 1 < sArgv.size())
            {
                vector<string> args;
                while (optionFromArg(sArgv[i + 1]) == UNKNOWN_OPTION && i + 1 < sArgv.size())
                {
                    ++i;
                    args.push_back(sArgv[i]);
                }
                Install(args);
                continue;
            }
            Install();
        }
        if (option & LIB)
        {
            if (i + 1 < sArgv.size())
            {
                vector<string> args;
                while (optionFromArg(sArgv[i + 1]) & UNKNOWN_OPTION && i + 1 < sArgv.size())
                {
                    ++i;
                    args.push_back(sArgv[i]);
                }
                Lib(args);
                continue;
            }
            Lib();
        }
        if (option & UNKNOWN_OPTION)
        {
            printC("Error: Unknown option '" + sArgv[i] + "'. Run: " + PROJECT_NAME + " '--help' to display help msg",
                   ESC_CODE_RED);
        }
    }
    return 0;
}
