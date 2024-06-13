#include <Mlib/Math.h>
#include <Mlib/FileSys.h>
#include <Mlib/Args.h>
#include <Mlib/def.h>

#define VERSION "0.1"
#define NAME "AmakeCpp"

using namespace std;
using namespace Mlib;

namespace AmakeCpp
{
    /// @name printC
    /// @brief Print string with color
    /// @param s string to print
    /// @param color color code
    void
    printC(string const &s, const char *color)
    {
        cout << NAME << " - [ " << color  << s << ESC_CODE_RESET << " ]" << '\n';
    }

    /// Print Help message
	void
    Help()
	{
		cout
            << "Usage: " << NAME << " [options]\n"
            << "Options:\n"
            << "    -h,     --help          Show this help message\n"
            << "    -v,     --version       Show version information\n"
            << "    -c,     --configure     Configure current dir as project" << endl;
	}

	void
    Configure()
	{
		string const cwd = FileSys::currentWorkingDir();
		try
        {
            printC("Creating Dir -> " + cwd + "/src", ESC_CODE_GREEN);
            FileSys::mkdir(cwd + "/src");
            printC("Creating Dir -> " + cwd + "/include", ESC_CODE_GREEN);
            FileSys::mkdir(cwd + "/include");
            printC("Creating Dir -> " + cwd + "/build", ESC_CODE_GREEN);
            FileSys::mkdir(cwd + "/build");
            printC("Creating Dir -> " + cwd + "/bin", ESC_CODE_GREEN);
            FileSys::mkdir(cwd + "/bin");
        }
        catch (exception const& e)
        {
            printC(e.what(), ESC_CODE_RED);
        }    
	}
}

int
main(int argc, char** argv)
{
	auto sArgv = Args::argvToStrVec(argc, argv);
	for (int i=1; i<sArgv.size(); ++i)
	{
		if (sArgv[i] == "--help")
		{
			AmakeCpp::Help();
		}
		else if (sArgv[i] == "--version")
		{
			cout << "Version: " << VERSION << endl;
		}
		else if (sArgv[i] == "--configure")
		{
			AmakeCpp::Configure();
		}
		else
		{
			cerr << "Error: Unknown option " << sArgv[i] << '\n';
		}
	}
	return 0;
}
