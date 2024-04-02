#include "compile_and_run.h"

void compile_project(const std::string& project_name,const std::string& base_scene)
{
    //Copy files from engine/game_src to build folder
    //Ensure directory exists
    if (!std::filesystem::exists("build"))
    {
        std::filesystem::create_directory("build");
        std::filesystem::copy("engine/game_src", "build/", std::filesystem::copy_options::recursive);
    }
    //Check if build/src exists
    if (std::filesystem::exists("build/src"))
    {
        //Delete build/src
        std::filesystem::remove_all("build/src");
    }

    //Remove build/bin/Debug/res
    if (std::filesystem::exists("build/bin/Debug/res"))
    {
        std::filesystem::remove_all("build/bin/Debug/res");
    }
    //Remove build/bin/Debug/engine
    if (std::filesystem::exists("build/bin/Debug/engine"))
    {
        std::filesystem::remove_all("build/bin/Debug/engine");
    }

    //Copy files from engine/game_src to build folder
    std::filesystem::copy("engine/game_src/src", "build/src", std::filesystem::copy_options::recursive);

    //Modify build/src/main.cpp
    std::ifstream main_file("build/src/main.cpp");
    std::string main_file_contents;
    std::string line;
    while (std::getline(main_file, line))
    {
        main_file_contents += line + "\n";
    }
    main_file.close();
    //Replace the [/GAME_NAME] and [/SCENE_PATH] with the project name and base scene
    main_file_contents = std::regex_replace(main_file_contents, std::regex(R"(\[\/GAME_NAME\])"), project_name);
    main_file_contents = std::regex_replace(main_file_contents, std::regex(R"(\[\/SCENE_PATH\])"), base_scene);

    //Write the modified contents to the file
    std::ofstream modified_main_file("build/src/main.cpp");
    modified_main_file << main_file_contents;
    modified_main_file.close();

    //Setup project with cmake, use vs compiler on windows
    std::string command = "cmake -S build -B build -G \"Visual Studio 16 2019\"";
    system(command.c_str());
    //Compile the project
    command = "cmake --build build --config Debug";
    system(command.c_str());

    //Copy engine/ and res/
    std::filesystem::copy("res/", "build/bin/Debug/res", std::filesystem::copy_options::recursive);
    std::filesystem::copy("engine/", "build/bin/Debug/engine", std::filesystem::copy_options::recursive);
}

void run_project()
{
    //Run the project in build/bin/Debug, make sure its running from it's own directory
    std::string command = "cd build/bin/Debug && ls && start Pomegranate_Engine.exe";
    system(command.c_str());
}