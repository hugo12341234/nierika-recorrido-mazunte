# 1. Introduction

> Data as of **Nov 12, 2026 (Mexico City time)**, `develop` branch (PRs #59–#68 merged).

## 1.1 Goal

Build an interactive, first-person, real-time 3D virtual tour of the evening and night of November 1 in Mazunte, Oaxaca: the beach, the cemetery and the cemetery room with its altar. ✅ `CONTEXTO_PROYECTO.md` §1–2.

## 1.2 Project data

| Field | Value |
|---|---|
| Company / team | Nierika Studio · Team 15 ("Seeing beyond the visible") |
| Members | D1 = 319266894 · D2 = 321332086 |
| Period | Sep 28, 2026 → Nov 18, 2026 (final delivery) |
| Proposal | v2.0, October 4, 2026 (`docs/propuesta/`) |
| Client | Course professor (Computer Graphics and Human–Computer Interaction, Faculty of Engineering, UNAM) |

## 1.3 Scope

**Included:** one continuous scene with 3 zones (beach and shore, cemetery with wall, cemetery room); first-person navigation with collisions; 5 animations (2 by KeyFrames + 3 simple); 5 cultural information cards; Blinn-Phong lighting; single-cycle ambient audio; Visual Studio project, repository, 3 videos and 2 manuals (ES + EN).

**Excluded:** town streets and buildings, Punta Cometa, sand carpet, VR/web/mobile versions, multiplayer, AI characters or dialogue, copal smoke with particles (the copal burner is modeled, without smoke). An exact reproduction of the real cemetery is also excluded: the scene is a **studio adaptation**.

## 1.4 Acceptance criteria and status

| Criterion | Target | Status on Nov 12 | What is missing |
|---|---|---|---|
| Own models | ≥ 18 (definition: STATE §4, Oct 19) | 🔄 all **23** inventory `.obj` files exist, each with its `.blend` (computed from the files, [§6](06_models_and_resources.md)); 11 are ✅ in the STATE document, 12 still 🔄 | Verification or compiled view; the repo cannot prove that each `.blend` is the team's own work (the facade one was rebuilt from the `.obj`) |
| Textures | 1K (1024 px) | 🔄 almost all 1024×1024; **exception: `laminado1.jpg` (facade and roof) is 512×512**; 8 models still use a flat provisional texture | Replace them or record the exception |
| Lighting | Interior readable without losing the night mood | ✅ A-8 (Nov 4), with D1's lights | — |
| Animation | 5 (2 KeyFrames + 3 simple), none copied from course practices, each with mathematical justification | 🔄 all 5 are in the code and all 5 are justified ([§7](07_math_justification.md)) | D2 must compile and watch them; D2 reviews the "not from the course" argument (the practices are not in the repo) |
| Interaction | 5 cards at < 2 m with APA 7 source | 🔄 code ready; texts "to be verified against the source" | Compile with ImGui in VS; verify texts and references |
| Navigation | First-person camera with collisions | ✅ A-9 (Nov 4); E-2 fixed the wall (Nov 12) | Stuck-spot test on D2's PC |
| Audio | One cycle: sea and distant music | 🔄 two own tracks in `Audio/` (D-6) | **Nobody has heard them yet** |
| Performance | ≥ 30 FPS at 1280×720 on integrated graphics | ❓ **not measured** (an FPS counter has been in the window title since E-1a) | D2 measures with the protocol of [§8.2.1](08_testing_and_performance.md) |

**KeyFrames only** for the silhouette's walk and for "leaving flowers": candles, fire, papel picado and smoke do not count. ✅

## 1.5 Work organization

- **Way of working (Oct 18):** the proposal's Resp./Verif. columns are no longer followed; whoever is free takes the next task and the other member reviews before integration. ✅
- **Version control:** `main` (deliveries), `develop` (integration), `feature/<code>-<name>`; everything enters through pull requests. ✅
- **Verification:** since Oct 20 no PR enters `develop` without `/verificar` APPROVE. **Recorded exception:** PRs #59–#68 (B-13, E-2, D-5, D-6, C-6, B-12, C-5, D-7, E-5, E-6) were merged on Nov 12 **without an APPROVE verdict**, by D2's decision (STATE §4, Nov 12); Claude's self-checks do not replace another person's review or an MSVC build. ✅
- **Management:** Trello Kanban board (To do / In progress, max. 2 per person / In review / Done). ✅
- **AI assistance:** the team uses Claude (Claude Code and a Claude Project) for planning, programming and documentation, under rules written in `CLAUDE.md`. ❓ Final wording to be decided ([§9.6](09_conclusions.md)).

📷 **FIGURE pending:** complete scene at delivery time (taken on D2's PC with the final look).
