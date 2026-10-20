/* MPL-2.0: 64-byte compressed packet must never invent attachment assets. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/world/attachment_state.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <bit>
#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
using namespace nexverse;
int main()
{
    world::WorldModel model;
    model.apply_region({
        0x0000010000000200ULL,"NexVerse",{128.0f,128.0f,25.0f},
        "127.0.0.1",9000U,"https://caps.example/seed",
        "aaaaaaaa-1111-2222-3333-bbbbbbbbbbbb",0x10U,0U,0U,
        13U,20.0f,"OpenGenesisLINK Test",256U,256U,true});
    world::ObjectState worn;
    worn.id="11111111-2222-3333-4444-555555555555";
    worn.local_id=42U;
    worn.pcode=protocol::pcode_prim;
    worn.state=0x10U; worn.parent_id=51U;
    worn.position={1.0f,2.0f,3.0f};
    worn.rotation={0.0f,0.0f,0.0f,1.0f};
    worn.scale={0.4f,0.5f,0.6f};
    worn.sculpt_asset_id="real-avatar-attachment-mesh";
    worn.sculpt_type=0x05U;
    worn.texture_entry={10U,11U,12U};
    worn.parsed_texture_entry=protocol::TextureEntry{};
    worn.default_texture=protocol::TextureEntryDefaultFace{};
    model.upsert_object(worn);
    auto header=protocol::ObjectUpdateCompressedEntry{};
    header.id=worn.id;
    header.local_id=42U;
    header.pcode=protocol::pcode_prim;
    header.state=0x20U; // Avatar attachment moved from point 1 to point 2
    header.position={2.0f,3.0f,4.0f};
    header.rotation={0.0f,0.0f,0.25f,0.96824583f};
    if(!model.apply_object_compressed_partial(header)||
       model.object_revision()!=2U)return 1;
    const auto& after=model.objects().at(42U);
    if(after.id!=worn.id||after.state!=0x20U||
       after.parent_id!=51U||after.position.x!=2.0f||
       after.scale.z!=0.6f||after.sculpt_asset_id!=worn.sculpt_asset_id||
       !after.is_mesh()||after.texture_entry!=worn.texture_entry||
       !after.parsed_texture_entry||!after.default_texture||
       world::infer_avatar_attachment_points(model.objects(),51U)
           .at(worn.id)!=2U)return 2;
    if(model.apply_object_compressed_partial(header)||
       model.object_revision()!=2U)return 3;
    auto recycled=header;
    recycled.id="99999999-2222-3333-4444-555555555555";
    recycled.state=0xF1U;
    recycled.position={8.0f,8.0f,8.0f};
    if(model.apply_object_compressed_partial(recycled)||
       model.objects().at(42U).id!=worn.id||
       model.objects().at(42U).state!=0x20U||
       model.objects().at(42U).position.x!=2.0f||
       model.object_revision()!=2U)return 4;
    auto unknown=header;
    unknown.local_id=99U;
    if(model.apply_object_compressed_partial(unknown)||
       model.objects().size()!=1U)return 5;
    auto invalid=header;
    invalid.position.x=std::numeric_limits<float>::quiet_NaN();
    if(model.apply_object_compressed_partial(invalid))return 6;
    invalid=header;
    invalid.rotation={0.0f,0.0f,0.0f,0.0f};
    if(model.apply_object_compressed_partial(invalid))return 7;
    invalid=header;
    invalid.has_complete_prim_data=true;
    if(model.apply_object_compressed_partial(invalid))return 8;
    // State=0 is meaningful: a server-authoritative detach. Do not
    // reuse the previous joint binding after that.
    header.state=0U;
    if(!model.apply_object_compressed_partial(header)||
       !world::infer_avatar_attachment_points(model.objects(),51U).empty())
        return 9;
    // Actual 64-byte OpenGenesisLINK compressed wire packet, including
    // 8-byte region header, 2-byte dilation, 1 object, 4 flags, 2 length,
    // object UUID, LocalID, State and position/rotation floats.
    std::vector<std::uint8_t> raw(8U+2U+1U+4U+2U+64U,0U);
    for(std::size_t k=0;k<8U;++k)
        raw[k]=static_cast<std::uint8_t>(
            (0x0000010000000200ULL>>(k*8U))&0xffU);
    raw[10U]=1U;
    raw[15U]=64U;
    const auto uuid=protocol::uuid_to_bytes(worn.id);
    if(!uuid)return 12;
    std::copy(uuid->begin(),uuid->end(),raw.begin()+17);
    raw[33U]=42U;
    raw[37U]=protocol::pcode_prim;
    raw[38U]=0x30U; // point 3, valid new server state
    const auto put_float=[&](std::size_t offset,float value)
    {
        const std::uint32_t bits=std::bit_cast<std::uint32_t>(value);
        for(std::size_t k=0U;k<4U;++k)
            raw[17U+offset+k]=static_cast<std::uint8_t>(
                (bits>>(k*8U))&0xffU);
    };
    put_float(40U,9.0f);put_float(44U,8.0f);put_float(48U,7.0f);
    const auto received=protocol::decode_object_update_compressed(raw);
    if(!received||received->objects.size()!=1U||
       received->objects[0].has_complete_prim_data||
       received->objects[0].state!=0x30U||
       received->objects[0].id!=worn.id||
       received->region_handle!=0x0000010000000200ULL||
       !model.apply_object_compressed_partial(received->objects[0])||
       world::infer_avatar_attachment_points(model.objects(),51U)
           .at(worn.id)!=3U||
       model.objects().at(42U).position.x!=9.0f)return 13;
    raw[17U]=0x99U; // recycled UUID but same LocalID
    const auto recycled_wire=protocol::decode_object_update_compressed(raw);
    if(!recycled_wire||model.apply_object_compressed_partial(
         recycled_wire->objects[0])||
       model.objects().at(42U).state!=0x30U)return 14;
    model.remove_object(42U);
    if(model.apply_object_compressed_partial(header)||
       !model.objects().empty())return 10;
    model.upsert_object(worn);
    model.begin_region_transition({
        0x0000010000000300ULL,"Other Region",{20.0f,20.0f,25.0f},
        "127.0.0.1",9001U,"https://caps.example/other",
        "bbbbbbbb-1111-2222-3333-cccccccccccc",0x10U,0U,0U,
        13U,20.0f,"OpenGenesisLINK Test",256U,256U,true});
    if(model.apply_object_compressed_partial(header)||
       !model.objects().empty())return 11;
    std::cout<<"COMPRESSED_PARTIAL_UUID_SCOPED_PARENT_MESH_TE_PRESERVED_PASS\n";
}
