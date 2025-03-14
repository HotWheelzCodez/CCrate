#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

void help_menu(void)
{
  printf("Command options:\n");
  printf("\t'init' - Takes in a directory name (to be created) and an optional compiler, and C/C++ standard. Then initializes a project structure.\n");
  printf("\t'build' - Takes in an optional build directory, then will compile and build the project, either in the given directory or the project root directory.\n");
  printf("\t'run' - Acts the same as build, then runs the project.\n");
  printf("\t'install' - Takes in a single, or several, packages which will be installed, then it updates the project to include/link the new packages/libraries.\n");
}

int init_project(const std::vector<std::string>& arguments)
{
  if (!arguments.size())
  {
    fprintf(stderr, "CCRATE: ERROR: 'init' needs at least one argument, the project name.\n");
    help_menu();
    return 1;
  }

  const std::string project = arguments[0];
  const std::string compiler = (arguments.size() > 1) ? arguments[1] : "g++";
  const std::string standard = (arguments.size() > 2) ? arguments[2] : "11";

  if (compiler != "g++" && compiler != "clang++" && compiler != "cc")
  {
    fprintf(stderr, "CCRATE: ERROR: Unknown compiler!\n");
    return 1;
  }

  if (standard != "11" && standard != "17")
  {
    fprintf(stderr, "CCRATE: ERROR: Unknown standard!\n");
    return 1;
  }

  std::filesystem::create_directory(project);
  printf("CCRATE: INFO: Created directory '%s'\n", project.c_str());

  std::filesystem::create_directory(project+"/src");
  printf("CCRATE: INFO: Created directory '%s'\n", std::string(project+"/src").c_str());

  std::filesystem::create_directory(project+"/include");
  printf("CCRATE: INFO: Created directory '%s'\n", std::string(project+"/include").c_str());

  return 0;
}

int build_project(const std::vector<std::string>& arguments)
{


  return 0;
}

int run_project(const std::vector<std::string>& arguments)
{

  return 0;
}

int install_package(const std::vector<std::string>& arguments)
{

  return 0;
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "CCRATE: ERROR: No enough arguments provided!\n");
    help_menu();
    return 1;
  }

  const std::string command(argv[1]);
  std::vector<std::string> arguments(argc-2);

  for (size_t i = 2; i < argc; i++)
    arguments[i-2] = std::string(argv[i]);

  if (command == "init")
    return init_project(arguments);
  if (command == "build")
    return build_project(arguments);
  else if (command == "run")
    return run_project(arguments);
  else if (command == "install")
    return install_package(arguments);

  fprintf(stderr, "CCRATE: ERROR: Unknown command!\n");
  help_menu();

  return 1;
}
