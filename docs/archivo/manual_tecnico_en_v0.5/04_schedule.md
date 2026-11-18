# 4. Schedule

> Status as of **Nov 12, 2026 (Mexico City time)**. Real dates are those recorded in the STATE document per task; **there are no real hours** in the repository (§4.5).

## 4.1 Original plan (proposal v2.0) ✅

| Wk | Dates | Phase | Milestone |
|---|---|---|---|
| 1 | Sep 28–Oct 2 | Kick-off and infrastructure | Repo, base project building, room documented |
| 2 | Oct 5–9 | Base scene and navigation | Beach and sea walkable in first person |
| 3 | Oct 12–16 | Cemetery room and graveyard | Facade, arch, interior and wall modeled and textured |
| 4 | Oct 19–23 | Offering objects | 3-level altar and 7 objects modeled |
| 5 | Oct 26–30 | Texturing, integration, lighting | Complete scene loaded, textured and lit |
| 6 | Nov 2–6 | Simple animations | Waves, candles and flowers integrated |
| 7 | Nov 9–13 | Complex animations, interaction, audio | 2 KeyFrame animations, cards and audio |
| 8 | Nov 16–18 | Testing, packaging, delivery | Executable, videos and manuals delivered |

Planned effort: 60 activities, 173 h execution + 30 h verification = 203 h. ✅

## 4.2 Re-plan of Oct 18 ✅

Five blocks with task codes (`A-1`…`E-7`, plus `X-1…X-3`): A (Oct 19–25) base scene with light · B (Oct 26–Nov 1) cemetery and altar with offering · C (Nov 2–8) 3 simple animations and cards · D (Nov 9–15) 2 KeyFrame animations and audio · E (Nov 16–18) delivery.

## 4.3 Real progress per task (STATE §2) ✅

Legend: ✅ closed with its criterion · 🔄 done in code/files with something to confirm (mostly "not yet compiled in Release x64 on D2's PC") · ⏳ not started.

| Block | Task | Date | Status | PR |
|---|---|---|---|---|
| A | A-1 Remove skull; provisional chapel | Oct 19 | ✅ APPROVE | #9 |
| A | A-2 Real scales (facade ×1.31) | Oct 20 | ✅ APPROVE | #11 |
| A | A-3, A-4 1280×720 window; 300 m far plane | Oct 20 | ✅ | #12 |
| A | A-5 Beach 100×50 m · A-6 1K sand | Oct 20 | ✅ APPROVE | #13, #14 |
| A | A-7 Sea 100×100 m | Oct 25 | ✅ APPROVE WITH REMARKS | #15 |
| A | A-8 Blinn-Phong lighting (D1's lights) | Nov 4 | ✅ APPROVE (per D2) | #16 |
| A | A-9 Collisions (Nov 4) · A-10 Arch with voussoirs (Nov 3) | Nov 3–4 | ✅ | #18, #30 |
| A | A-11 Floor and roof · A-12 Stone textures | Nov 3–4 | ✅ (A-12: still to compile and verify) | #31, #32 |
| B | B-9 Tombs (4) · B-10 Wall and rock | Nov 4 | ✅ APPROVE | #40, #41 |
| B | B-1, B-2, B-5, B-6, B-8, B-11, B-14 Own Blender models | Nov 4 | 🔄 | #36 |
| B | B-3 Marigold · B-4 Pan de muerto · B-7 Papel picado | Nov 11 | 🔄 | #49, #50, #51 |
| B | B-12 Own textures and bump map | Nov 12 | 🔄 | #66 |
| B | B-13 Full offering in the room | Nov 12 | 🔄 | #52, #59 |
| C | C-1 Waves · C-4 Floating flowers | Nov 4 | 🔄 | #28, #26 |
| C | C-2 Flame (noise) · C-3 Floating candles | Nov 11 | 🔄 | #54, #53 |
| C | C-5 Integration of the 3 simple · C-6 Alpha discard | Nov 12 | 🔄 (C-5: FPS not measured) | #64, #63 |
| C | C-7 Cards (C-7a texts, C-7b ImGui) | Nov 4 | 🔄 not compiled in VS; texts unverified | #20, #24 |
| C | C-8 Justification of the 3 simple | Nov 11 | 🔄 | #55 |
| D | D-1 Silhouette hierarchy · D-2 Slerp walk | Nov 4 | D-2 ✅ · D-1 ⏳ (D1's model integrated, not compiled) | #29, #39 |
| D | D-3 Smoothstep justification | Nov 11 | ✅ | #56 |
| D | D-4 Leaving flowers (Bézier) | Nov 11 | 🔄 | #57 |
| D | D-5 Integrate the 2 complex · D-6 Audio · D-7 Justification | Nov 12 | 🔄 | #60, #62, #65 |
| E | E-1a `cleanup()` and FPS counter | Nov 4 | 🔄 to compile and verify | #19 |
| E | E-2 Wall and collisions | Nov 12 | 🔄 | #61 |
| E | E-3 Packaging (`dist/`) | Nov 4 | 🟡 test on another PC pending | #21, #43 |
| E | E-4 Videos · E-7 Release | — | ⏳ | — |
| E | E-5 Technical manual · E-6 User manual | Nov 12–13 | 🔄 (this document) | #67, #68 + current PR |
| X | X-3 D1's extras (bread, flower, skull) | Nov 4 | ✅ APPROVE (per D2) | #17 |

PRs #59–#68 were merged without an APPROVE verdict (Nov 12 exception, STATE §4; [§1.5](01_introduction.md)).

## 4.4 Plan vs. real

| Block | Planned | Real on Nov 12 |
|---|---|---|
| A (Oct 19–25) | A-1…A-12 | ✅ 12 of 12 (last closures, A-8 and A-12, on Nov 4) |
| B (Oct 26–Nov 1) | 14 tasks | 2 ✅ (B-9, B-10); the other 12 🔄: **done 1–2 weeks late** (Nov 4 and Nov 11–12) |
| C (Nov 2–8) | C-1…C-8 | All 🔄 (code done between Nov 4 and Nov 12) |
| D (Nov 9–15) | D-1…D-7 | D-2 and D-3 ✅; the rest 🔄; D-1 ⏳ to confirm |
| E (Nov 16–18) | E-1…E-7 | E-1a, E-2, E-3, E-5, E-6 under way; E-4 and E-7 ⏳ |

**Honest reading:** the recorded closures cluster between Nov 3 and Nov 12 (STATE dates), leaving **little testing margin**: almost everything marked 🔄 depends on a single compile and visual check by D2. The repository does not record the causes of the delay against the original plan; what is said about them is for D1 and D2 to decide before delivery. ❓

## 4.5 Real hours

❓ **UNDECIDED.** The repo does not store real hours; the proposal's 173 + 30 h are estimates. This section therefore delivers closing dates, not hours.

## 4.6 Remaining schedule (until Nov 18)

Proposed order, **no dates fixed by D1/D2**: (1) D2 compiles `Release | x64` and reviews all 🔄 items; (2) measure FPS (§8.2.1); (3) E-1: error list and `cleanup()` check; (4) E-3: test the `.zip` on a PC without Visual Studio; (5) E-4: three videos; (6) E-5/E-6: final screenshots on D2's PC and PDF; (7) E-7: publish the Release. ⏳
