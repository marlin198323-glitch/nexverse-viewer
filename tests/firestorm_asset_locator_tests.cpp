/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
void env_set(const char* key,const char* value)
{
#ifdef _WIN32
    _putenv_s(key,value);
#else
    setenv(key,value,1);
#endif
}
void env_clear(const char* key)
{
#ifdef _WIN32
    _putenv_s(key,"");
#else
    unsetenv(key);
#endif
}
}
int main()
{
    namespace fs=std::filesystem;
    const auto home_before=std::getenv("HOME");
    const auto assets_before=std::getenv("NEXVERSE_AVATAR_ASSETS");
    const std::string old_home=home_before?home_before:"";
    const std::string old_assets=assets_before?assets_before:"";
    const auto base=fs::temp_directory_path()/
        ("ogl-firestorm-discovery-"+std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    const auto root=base/"Schreibtisch";
    const auto stable=root/"Phoenix-FirestormOS-Releasex64-test"/"character";
    const auto nightly=root/"Phoenix-FirestormOS-Nightlyx64-test"/"character";
    fs::create_directories(stable);
    fs::create_directories(nightly);
    const auto create=[&](const fs::path& folder)
    {
        for(auto name:{"avatar_head.llm","avatar_upper_body.llm",
                       "avatar_lower_body.llm","avatar_skeleton.xml"})
            std::ofstream{folder/name}<<"local-only fixture";
    };
    create(stable);
    create(nightly);
    const auto cache=base/".cache"/"nexverse-viewer"/
        "firestorm-nightly-7-2-5-81754"/"character";
    fs::create_directories(cache);
    create(cache);
    env_set("HOME",base.string().c_str());
    env_clear("NEXVERSE_AVATAR_ASSETS");
    const auto discovered=nexverse::assets::find_firestorm_avatar_character_directory();
    int error=0;
    if(!discovered||*discovered!=cache)error=1;
    env_set("NEXVERSE_AVATAR_ASSETS",(stable.parent_path()).string().c_str());
    const auto explicit_selected=
        nexverse::assets::find_firestorm_avatar_character_directory();
    if(!explicit_selected||*explicit_selected!=stable)error=2;
    fs::remove(cache/"avatar_lower_body.llm");
    env_clear("NEXVERSE_AVATAR_ASSETS");
    const auto desktop_nightly=nexverse::assets::
        find_firestorm_avatar_character_directory();
    if(!desktop_nightly||*desktop_nightly!=nightly)error=3;
    fs::remove(nightly/"avatar_lower_body.llm");
    const auto fallback=nexverse::assets::
        find_firestorm_avatar_character_directory();
    if(!fallback||*fallback!=stable)error=4;
    if(home_before)env_set("HOME",old_home.c_str());else env_clear("HOME");
    if(assets_before)env_set("NEXVERSE_AVATAR_ASSETS",old_assets.c_str());
    else env_clear("NEXVERSE_AVATAR_ASSETS");
    fs::remove_all(base);
    if(error)
    {
        std::cerr<<"Local Firestorm asset resolver failed stage "<<error<<"\n";
        return error;
    }
    std::cout<<"Firestorm extracted Desktop/Schreibtisch Nightly, stable and explicit override PASS\n";
    return 0;
}
