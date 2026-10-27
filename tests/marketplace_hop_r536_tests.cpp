#include "nexverse/app/marketplace_hop.hpp"
#include <iostream>
#include <string_view>
#include <vector>
int main()
{
 using nexverse::app::parse_marketplace_hop;
 const auto valid=parse_marketplace_hop(
     "hop://hg.stadt-nexverse.de:80/Marina/128/127/25");
 if(!valid || valid->login_start()!="uri:Marina&128&127&25")
     return 1;
 const auto space=parse_marketplace_hop(
     "hop://hg.stadt-nexverse.de/My%20Town/0/255/4096");
 if(!space || space->login_start()!="uri:My Town&0&255&4096")
     return 2;
 const std::vector<std::string_view> invalid{
     "https://hg.stadt-nexverse.de:80/Marina/128/128/25",
     "hop://evil.example:80/Marina/128/128/25",
     "hop://hg.stadt-nexverse.de.evil:80/Marina/128/128/25",
     "hop://u@hg.stadt-nexverse.de:80/Marina/128/128/25",
     "hop://hg.stadt-nexverse.de:81/Marina/128/128/25",
     "hop://hg.stadt-nexverse.de:80/Marina/256/128/25",
     "hop://hg.stadt-nexverse.de:80/Marina/128/128/4097",
     "hop://hg.stadt-nexverse.de:80/Marina/128/128/-1",
     "hop://hg.stadt-nexverse.de:80/Marina/128/128/25?foo=bar",
     "hop://hg.stadt-nexverse.de:80/Marina/128/128/25#fragment",
     "hop://hg.stadt-nexverse.de:80/Marina%2Fadmin/128/128/25",
     "hop://hg.stadt-nexverse.de:80/Marina%26admin/128/128/25",
     "hop://hg.stadt-nexverse.de:80/Marina/128/128/25/extra",
     "hop://hg.stadt-nexverse.de:80/Marina/0/0/1;touch",
     "hop://hg.stadt-nexverse.de:80/../1/1/1",
 };
 for(const auto input:invalid)
     if(parse_marketplace_hop(input))
     {std::cerr<<"Unsafe hop accepted: "<<input<<'\n';return 3;}
 std::cout<<"R536 MARKETPLACE HOP SAFE PARSER PASS\n";
}
