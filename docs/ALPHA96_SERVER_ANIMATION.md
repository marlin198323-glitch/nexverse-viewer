# Firestorm AvatarAnimation High20 – OGL Alpha.96

OpenGenesisLINK/Firestorm compatibility contract.

## Autoritative Drahtstruktur

Offizielles Firestorm 7.2.4 message_template.msg:

- Message ID: High 20; Trusted, Unencoded, simulator -> viewer
- Sender Single: ID LLUUID
- AnimationList Variable: AnimID LLUUID, AnimSequenceID S32
- AnimationSourceList Variable: ObjectID LLUUID
- PhysicalAvatarEventList Variable: TypeData Variable 1

Entgegen AgentUpdate ist weder AgentSessionID noch RegionHandle
im AvatarAnimation-Nutzdatenblock vorhanden. Sicherheits-/Regions-
zuordnung erfolgt ueber aktiven Circuit und WorldModel-Epochen;
keine erfundenen Felder lesen.

Alle Blockzaehler sind U8; Quelle darf kuerzer als AnimationList
sein. Der Decoder prueft Vollstaendigkeit, Budget und Null-Sender.
Die AnimationList ist jeweils ein vollstaendiger aktiver Satz fuer
den Absender. Ein leeres Paket stoppt vorherige Eintraege.
Doppelte AnimUUID im selben Paket: maximaler signed SequenceID-
Wert gewinnt. PhysicalEvents bleiben rohe Bytefelder;
keine Interpretation als Bewegungs-, Physik- oder Wear-Befehl.

## InWorld-Verarbeitung

SelfAnimation wird nur gespeichert, wenn Sender der aktiven
AgentID entspricht, Region verbunden und Circuit gueltig.
RemoteAnimation wird an ein existierendes AvatarState gebunden
oder bis zum ersten ObjectUpdate/CoarseSnapshot fuer max.
64 unbekannte UUIDs vorgehalten; es entsteht kein sichtbarer
GhostActor. Upsert/Coarse/Terse duerfen AnimationState nicht
versehentlich ueberschreiben. WorldModel.reset und
begin_region_transition loeschen alle Animation-Pakete.
Avatarentfernung invaldiert dessen Pending-Satz.

## GPU-Animation: Funktionsumfang und Wahrheit

Bekannte Firestorm Built-in-Anims identifiziert anhand
der offiziellen indra/llcharacter/llanimationstates.cpp UUIDs:
Walk(+New), Run(+New), Jump/PreJump/Land, Fly(+Slow),
Sit/SitFemale/SitGround, Hover. Der provisorische
Motion-Selector gibt Sit > Jump > Fly > Hover > Run > Walk
den Vorrang. Er ist KEINE vollstaendige
Firestorm Animation-Priority-Berechnung, denn der High20
Wire-Block enthaelt kein Asset-Priority-Attribut.
Unbekannte Custom-UUIDs werden gelesen, gespeichert,
aber nicht fälschlich als erkannte Standardanimation gespielt.
Locomotion-Fallback aus bestaetigten RemotePosition-
Deltas bzw. lokalen WASD-Controls bleibt getrennt.

Der Original-LLM-133-Bone-Pfad erzeugt simple
prozedurale Joint-Posen. Echte LLKeyframeMotion-Assetclips,
Source-Object-Sit-Animation, Animation-Bone-Rotation,
AnimationLayer-Mixing, Loop, Fade, Custom Asset-Fetch
und vollständige Gesichtsanimtionen folgen später.

## Regressionsumfang

tests/avatar_animation_tests.cpp: golden High20 LLUDP header,
Bloecke und Byteoffets, NullUUID, Event Bounded,
fehlende Counts, doppelte IDs, signed Sequence und UUID Mapping.
tests/world_model_tests.cpp: Self/Remote/Unknown, invisible Pending,
bounded 64, Stop, ServerPoseMetadata, Epochenreset.
tests/renderer_region_transition_tests.cpp:
3 originale Firestorm LLM GPU-Weight-Meshes fuer
Run/Sit/Jump/Fly/Hover in einem OpenGL3.3-Lauf.
Keine Live-Abnahme ohne authentifizierten NexVerse-Login.

Quellen: FirestormViewer/phoenix-firestorm,
Firestorm_7.2.4 message_template.msg und
indra/llcharacter/llanimationstates.cpp. License
Firestorm Komponenten pro Modul pruefen; aktuelle
OGL-Implementierung ist separate MPL-2.0.
