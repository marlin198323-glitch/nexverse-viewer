/* MPL-2.0: no false Low158/COF proof from provisional object TE. */
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include "nexverse/ui/avatar_bake_inspector.hpp"
#include <iostream>
#include <string>
#include <unordered_set>
using namespace nexverse;
int main()
{
    ui::AvatarPipelineProof proof{};
    proof.world_ready=true;
    proof.self_present=true;
    proof.provisional_object_bakes=true;
    protocol::AvatarBakedTextures bakes{};
    bakes.ids[0]="head";
    bakes.ids[1]="upper";
    bakes.ids[2]="lower";
    proof.bakes=protocol::audit_avatar_bake_evidence(bakes,{"head","lower"});
    proof.original_body_draws=3U;
    proof.body_bake_draws={1U,0U,1U};
    if(proof.appearance_received||proof.body_gpu_drawn()||
       proof.provisional_body_gpu_submitted())return 1;
    if(ui::describe_avatar_pipeline(proof).find("Obj-TE vorlaeufig")==
       std::string::npos ||
       ui::describe_avatar_body_pipeline(proof).find("ObjTE H:G U:S L:G")==
       std::string::npos)return 2;
    const auto pending=ui::inspect_avatar_bakes(proof);
    if(pending.summary.find("Low158 fehlt")==std::string::npos||
       pending.rows[0].find("Obj-TE: ID / GPU geladen")==std::string::npos||
       pending.rows[1].find("Obj-TE: ID / GPU fehlt")==std::string::npos||
       pending.rows[3].find("Server: wartet")==std::string::npos||
       pending.visual.find("vorlaeufig")==std::string::npos)return 3;
    if(proof.skin_stage().find("Low158 fehlt")==std::string::npos)return 4;
    // Actual submissions and real resident texture IDs are NOT a
    // simulator-received Low158: no authoritative body_gpu_drawn.
    proof.bakes=protocol::audit_avatar_bake_evidence(
        bakes,{"head","upper","lower"});
    proof.body_bake_draws={1U,1U,1U};
    if(proof.provisional_body_gpu_submitted())return 11;
    proof.current_body_bake_sources_drawn={true,true,true};
    if(proof.body_gpu_drawn()||!proof.provisional_body_gpu_submitted())
        return 5;
    if(ui::inspect_avatar_bakes(proof).visual.find("Depth-Test wartet")==
       std::string::npos)return 6;
    proof.fragment_query_known=true;
    if(ui::inspect_avatar_bakes(proof).visual.find(
           "keine sichtbaren Depth-Pixel")==std::string::npos)return 7;
    proof.fragment_samples_passed=true;
    if(ui::inspect_avatar_bakes(proof).visual.find(
           "Low158 nicht belegt")==std::string::npos)return 8;
    // Low158 is now the authoritative source, and the normal original
    // mesh/GPU checks again decide whether body_gpu_drawn is true.
    proof.appearance_received=true;
    proof.provisional_object_bakes=false;
    if(!proof.body_gpu_drawn()||
       ui::inspect_avatar_bakes(proof).summary.find("Server-TE")==
       std::string::npos ||
       ui::describe_avatar_body_pipeline(proof).find("ObjTE")!=
       std::string::npos)return 9;
    proof.appearance_received=false;
    if(proof.body_gpu_drawn())return 10;
    std::cout<<"OGL_PROVISIONAL_BAKE_UI_GL_SOURCE_PROVENANCE_PASS\n";
}
