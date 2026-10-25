/* Mozilla Public License, v. 2.0. */
#include "nexverse/assets/texture_format_preference.hpp"
#include <iostream>
using nexverse::assets::TextureFormatPreference;
int main()
{
    TextureFormatPreference n;
    n.enter(4U);
    if(n.skip_png(4U))return 1;
    n.native_succeeded(4U,true);
    n.native_succeeded(4U,true);
    if(n.skip_png(4U))return 2;
    n.native_succeeded(4U,true);
    // 15 successive transfers can bypass a provably broken converter;
    // on the 16th transfer, ALWAYS probe PNG again.
    for(int i=0;i<15;++i)
        if(!n.skip_png(4U))return 3;
    if(n.statistics().png_gets_avoided!=15U||
       n.statistics().png_converter_reprobes!=0U||
       !n.statistics().native_preferred)return 12;
    if(n.skip_png(4U))return 4;
    if(n.statistics().png_converter_reprobes!=1U||
       n.statistics().png_gets_avoided!=15U)return 13;
    if(!n.skip_png(4U))return 5;
    n.native_failed(4U);
    if(n.skip_png(4U))return 6;
    n.native_succeeded(4U,true);
    n.native_succeeded(4U,true);
    n.native_succeeded(4U,true);
    if(!n.skip_png(4U))return 7;
    n.png_succeeded(4U);
    if(n.skip_png(4U))return 8;
    for(int i=0;i<3;++i)n.native_succeeded(4U,true);
    n.enter(5U);
    if(n.skip_png(5U)||
       n.statistics().png_gets_avoided!=0U||
       n.statistics().png_converter_reprobes!=0U)return 9;
    // An in-flight worker from the old region cannot poison the new one.
    for(int i=0;i<4;++i)n.native_succeeded(4U,true);
    if(n.skip_png(5U))return 10;
    if(n.skip_png(0U))return 11;
    std::cout<<"OGL_TEXTURE_FORMAT_NEGOTIATION_PASS\n";
}
