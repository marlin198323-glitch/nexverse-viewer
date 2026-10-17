/* MPL-2.0 */
#include "nexverse/render/avatar_skin_weight_pair.hpp"
#include <cmath>
#include <iostream>
#include <limits>
using nexverse::render::resolve_avatar_skin_weight;
int main()
{
  for(std::size_t n=2;n<=32;++n)
    for(float w=0;w<float(n);w+=0.125f)
    {
      auto p=resolve_avatar_skin_weight(w,n);
      if(!p.valid||p.second>=n||p.second!=p.first+1||
         p.blend<0||p.blend>1)return 1;
    }
  auto a=resolve_avatar_skin_weight(1.25f,4);
  if(!a.valid||a.first!=1||a.second!=2||
     std::abs(a.blend-0.25f)>0.0001f)return 2;
  auto b=resolve_avatar_skin_weight(31.9f,32);
  if(!b.valid||b.first!=30||b.second!=31||b.blend!=1)return 3;
  auto c=resolve_avatar_skin_weight(1.5f,2);
  if(!c.valid||c.first!=0||c.second!=1||c.blend!=1)return 4;
  if(resolve_avatar_skin_weight(-1,4).valid ||
     resolve_avatar_skin_weight(4,4).valid ||
     resolve_avatar_skin_weight(0,1).valid ||
     resolve_avatar_skin_weight(0,33).valid ||
     resolve_avatar_skin_weight(std::numeric_limits<float>::quiet_NaN(),4).valid)
     return 5;
  std::cout<<"Firestorm last joint, interpolated pair and GPU palette bounds PASS\n";
}
