#include <Mlib/Args.h>
#include <Mlib/FileSys.h>
#include <Mlib/def.h>
#include <string>
#include <unordered_map>
#include <vector>

#define VERSION "0.1"
#define NAME "AmakeCpp"

using namespace std;
using namespace Mlib;

string const cwd         = FileSys::currentWorkingDir();
string const projectName = cwd.substr(cwd.find_last_of("/") + 1);
string const SRC_DIR     = cwd + "/src";
string const INCLUDE_DIR = SRC_DIR + "/include";
string const AS_DIR      = SRC_DIR + "/as";
string const CPP_DIR     = SRC_DIR + "/cpp";
string const C_DIR       = SRC_DIR + "/c";
string const BUILD_DIR   = cwd + "/build";
string const OBJ_DIR     = BUILD_DIR + "/obj";
string const BIN_DIR     = BUILD_DIR + "/bin";

namespace AmakeCpp {
    namespace Options {
        /// @enum Option
        /// @brief
        /// - Enum representing cli options
        enum Option
        {
            H,
            C,
            V,
            B,
            CL,
            I,
            UNKNOWN
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
        optionFromArg(const string& arg)
        {
            static const unordered_map<string, Option> optionMap = {
                {     "--help",  H},
                {"--configure",  C},
                {  "--version",  V},
                {    "--build",  B},
                {    "--clean", CL},
                {  "--install",  I}
            };
            auto const it = optionMap.find(arg);
            if (it != optionMap.end())
            {
                return it->second;
            }
            return UNKNOWN;
        }

        /// @enum ConfigureOption
        /// @brief
        /// - Enum representing configure sub options
        enum ConfigureOption
        {
            CLANG_FORMAT
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
        configureOptionFromArg(const string& arg)
        {
            static const unordered_map<string, ConfigureOption> configureOptionMap = {
                {"--clang-format", CLANG_FORMAT}
            };
            auto const it = configureOptionMap.find(arg);
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
        /// @returns void
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
        void
        printC(string const& str, const char* color)
        {
            cout << NAME << " - [ " << color << str << ESC_CODE_RESET << " ]" << '\n';
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
        createProjectDir(const string& path)
        {
            try
            {
                printC("Creating Dir -> " + path, ESC_CODE_GREEN);
                FileSys::mkdir(path);
            }
            catch (exception const& e)
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
            vector<string> files;
            try
            {
                files = FileSys::dirContentToStrVec(cwd + "/src/cpp");
                for (auto const& file : files)
                {
                    string fileName = file.substr(file.find_last_of("/") + 1);
                    string objName  = cwd + "/build/obj/" + fileName.substr(0, fileName.find_last_of(".")) + ".o";
                    printC(file + " -> " + objName, ESC_CODE_BLUE);
                    system(("clang++ -O3 -march=native -c " + file + " -o " + objName).c_str());
                }
            }
            catch (exception const& e)
            {
                printC(e.what(), ESC_CODE_RED);
            }
        }

        /// @name linkBinary
        /// @brief
        /// - Link .o files in build/obj to binary in build/bin
        /// @returns void
        void
        linkBinary()
        {
            printC("Linking Obj Files -> " + cwd + "/build/bin/" + projectName, ESC_CODE_GREEN);
            vector<string> objVec = FileSys::dirContentToStrVec(cwd + "/build/obj");
            string         objStr = Args::strFromStrVec(objVec);
            system(("clang++ -O3 -march=native " + objStr + " -o " + cwd + "/build/bin/" + projectName).c_str());
        }

        void
        configureProjectDirs()
        {
            createProjectDir(SRC_DIR);
            createProjectDir(INCLUDE_DIR);
            createProjectDir(AS_DIR);
            createProjectDir(CPP_DIR);
            createProjectDir(C_DIR);
            createProjectDir(BUILD_DIR);
            createProjectDir(OBJ_DIR);
            createProjectDir(BIN_DIR);
        }

        void
        configureClangFormat()
        {
            cout << "Configuring .clang-format\n";
        }
    } // namespace Tools
    using namespace Tools;

    /// @name Help
    /// @brief
    /// - Show help message
    /// @returns void
    void
    Help()
    {
        cout << "Usage: " << NAME << " [options]\n"
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
    Configure(const string& subOption = "")
    {
        if (subOption.empty())
        {
            configureProjectDirs();
        }
        else
        {
            ConfigureOption const option = configureOptionFromArg(subOption);
            switch (option)
            {
                case CLANG_FORMAT :
                    try
                    {
                        configureClangFormat();
                    }
                    catch (exception const& e)
                    {
                        printC(e.what(), ESC_CODE_RED);
                    }
                    break;
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
    /// - Clean project
    /// @returns void
    void
    Clean()
    {
        try
        {
            printC("Cleaning project", ESC_CODE_GREEN);
            FileSys::rmdir(cwd + "/build");
        }
        catch (exception const& e)
        {
            printC(e.what(), ESC_CODE_RED);
        }
    }

    /// @name Install
    /// @brief
    /// - Install project
    /// @returns void
    void
    Install()
    {
        linkBinary();
    }
} // namespace AmakeCpp
using namespace AmakeCpp;

int
main(int argc, char** argv)
{
    auto sArgv = Args::argvToStrVec(argc, argv);
    for (int i = 1; i < sArgv.size(); ++i)
    {
        Option const option = optionFromArg(sArgv[i]);
        switch (option)
        {
            case H :
                Help();
                break;
            case C :
                if (i + 1 < sArgv.size())
                {
                    if (optionFromArg(sArgv[i + 1]) == UNKNOWN)
                    {
                        Configure(sArgv[i + 1]);
                        ++i;
                        break;
                    }
                }
                Configure();
                break;
            case V :
                cout << "Version: " << VERSION << '\n';
                break;
            case B :
                Build();
                break;
            case CL :
                Clean();
                break;
            case I :
                Install();
                break;
            case UNKNOWN :
                cerr << "Error: Unknown option '" << sArgv[i] << "'\n";
                break;
        }
    }
    return 0;
}
