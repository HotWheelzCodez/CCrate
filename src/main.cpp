#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

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
    return 1;
  }

  const std::string project = arguments[0];
  const std::string compiler = (arguments.size() > 1) ? arguments[1] : "g++";
  const std::string standard = (arguments.size() > 2) ? arguments[2] : "11";

  if (compiler != "g++" && compiler != "clang++")
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

  std::ofstream main_src(project+"/src/main.cpp");
  main_src << "#include <iostream>\n\n";
  main_src << "#include \"../include/main.hpp\"\n\n";
  main_src << "int main(int argc, char** argv)\n";
  main_src << "{\n";
  main_src << "\tstd::cout << \"Hello, World!\\n\";\n";
  main_src << "\treturn 0;\n}";
  main_src.close();
  printf("CCRATE: INFO: Created file '%s'\n", std::string(project+"/src/main.cpp").c_str());

  std::ofstream main_include(project+"/include/main.hpp");
  main_include << "#pragma once /* MAIN_HPP */\n\n";
  main_include.close();
  printf("CCRATE: INFO: Created file '%s'\n", std::string(project+"/include/main.hpp").c_str());

  std::ofstream makefile(project+"/Makefile");
  makefile << "CC="+compiler << '\n';
  makefile << "CFLAGS=-Wall -g\n";
  makefile << "STD=c++"+standard+'\n';
  makefile << "SRC_DIR=src\n";
  makefile << "OBJ_DIR=obj\n";
  makefile << "SRC_FILES=$(shell find $(SRC_DIR) -name \"*.cpp\")\n";
  makefile << "OBJ_FILES=$(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)\n";
  makefile << "TARGET="+project << '\n';
  makefile << "all: $(TARGET)\n";
  makefile << "$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c\n";
  makefile << "\t@mkdir -p $(OBJ_DIR)\n";
  makefile << "\t$(CC) $(CFLAGS) --std=$(STD) -c $< -o $@\n";
  makefile << "$(TARGET): $(OBJ_FILES)\n";
  makefile << "\t$(CC) $(OBJ_FILES) $(LDFLAGS) -o $(TARGET)\n";
  makefile << "\trm -r $(OBJ_DIR)\n";
  makefile << "\trm -rf $(OBJ_DIR)\n";
  makefile << ".PHONY: all clean";
  makefile.close();
  printf("CCRATE: INFO: Created file '%s'\n", std::string(project+"/Makefile").c_str());

  std::ofstream proj_file(project+"/ccrate.proj");
  proj_file << "name="+project << '\n';
  proj_file << "compiler="+compiler << '\n';
  proj_file << "standard="+standard;
  proj_file.close();
  printf("CCRATE: INFO: Created file '%s'\n", std::string(project+"/ccrate.proj").c_str());

  return 0;
}

int build_project(const std::vector<std::string>& arguments)
{
  std::fstream project("ccrate.proj");
  std::string name;
  while (std::getline(project, name)) { break; }
  project.close();
  name = name.substr(name.find('=')+1, name.length());

  system("make >/dev/null");
  printf("CCRATE: INFO: Created project executable '%s'\n", name.c_str());

  return 0;
}

int run_project(const std::vector<std::string>& arguments)
{
  std::fstream project("ccrate.proj");
  std::string name;
  while (std::getline(project, name)) { break; }
  project.close();
  name = name.substr(name.find('=')+1, name.length());

  build_project(arguments);
  printf("CCRATE: INFO: Running project '%s'...\n", name.c_str());
  std::string run = "./"+name;
  system(run.c_str());

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
