/* MPL-2.0: deterministic successful GPU mesh upload log cadence. */
#include "nexverse/render/mesh_upload_log_checkpoint.hpp"
#include <iostream>
using nexverse::render::mesh_upload_log_checkpoint;
int main()
{
    if(mesh_upload_log_checkpoint(0U) ||
       !mesh_upload_log_checkpoint(1U) ||
       mesh_upload_log_checkpoint(2U) ||
       mesh_upload_log_checkpoint(127U) ||
       !mesh_upload_log_checkpoint(128U) ||
       !mesh_upload_log_checkpoint(256U) ||
       mesh_upload_log_checkpoint(257U))
       return 1;
    std::uint64_t emits=0;
    for(std::uint64_t upload=1;upload<=102762U;++upload)
        if(mesh_upload_log_checkpoint(upload))++emits;
    // 102,762 historical messages reduce to 803 aggregates; actual
    // GL calls, VRAM usage and asset identities remain unchanged.
    if(emits!=803U)return 2;
    std::cout<<"OGL_MESH_UPLOAD_LOG_CHECKPOINT_PASS\n";
}
