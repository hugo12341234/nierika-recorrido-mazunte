# 5. RACI matrix

R = Responsible · A = Accountable (approves) · C = Consulted · I = Informed. D1 = 319266894, D2 = 321332086. Status on Nov 12, 2026.

## 5.1 Planned RACI (proposal v2.0, Table 8) ✅

Number of activities in parentheses. This is **what was proposed**, not what happened.

| Work area | Activities | D1 | D2 | Client |
|---|---|---|---|---|
| Research and proposal | 1–2 | R (1) / A (1) | R (1) / A (1) | A |
| Infrastructure and tools | 3–6 | R (1) / A (3) | R (3) / A (1) | I |
| Cemetery room design | 7 | R (1) | A (1) | C |
| Beach, sea and graveyard | 8, 10, 13, 14 | R (1) / A (3) | R (3) / A (1) | I |
| Camera, render loop, collisions | 9, 11, 12, 15 | R (3) / A (1) | R (1) / A (3) | I |
| Cemetery room | 16–22 | R (3) / A (4) | R (4) / A (3) | I |
| Offering objects | 23–30 | R (4) / A (4) | R (4) / A (4) | I |
| Texturing | 31–34 | R (2) / A (2) | R (2) / A (2) | I |
| Integration and lighting | 35–38 | R (2) / A (2) | R (2) / A (2) | I |
| Simple animations | 39–45 | R (3) / A (4) | R (4) / A (3) | I |
| Complex animations | 46–50, 53 | R (3) / A (3) | R (3) / A (3) | I |
| Information points and audio | 51–52 | R (1) / A (1) | R (1) / A (1) | C |
| Testing, videos, manuals, delivery | 54–60 | R (4) / A (3) | R (3) / A (4) | A |

## 5.2 Change of way of working ✅ (STATE §4, Oct 18)

Since Oct 18 the Resp./Verif. columns are not followed: whoever is free takes the next task and **the other member reviews** before integration. Table 8 does not describe real execution.

## 5.3 Real record (what can be stated today)

Only what the STATE document and the repository say; nothing is inferred from commits.

| What | Who, according to the repository | Source |
|---|---|---|
| Code, Blender models, script-generated textures and audio, documentation | **D2** (`Hugo Chacón`) is the author of **189 + 8 commits**; 1 commit comes from the platform (`Claude`) | `git shortlog -sne origin/develop`, Nov 12 |
| Blinn-Phong lighting with 4 point lights, spot lights and their animation | **D1** (Practice 8 adapted), integrated by D2 in A-8 | STATE §4, Nov 4 |
| Bread, flower and skull (Maya); 10-piece silhouette model | **D1**, integrated by D2 (X-3, D-1) | STATE X-3, D-1 |
| Beach and sea | D2 (Blender 5.2.2) | STATE A-5, A-7 |
| Models B-3, B-4, B-7, B-9, B-10, B-12, E-2, audio D-6 | Generated with `bpy` and scripts by Claude at D2's request (reviewed by D2 in each PR; no `/verificar` by another person for PRs #59–#68) | STATE §4, Nov 11 and 12 |
| Verifications | `/verificar` APPROVE recorded for A-1, A-2, A-5, A-6, A-7 (with remarks), A-8, X-3, B-9 and B-10; **all by D2** ("per D2" for A-8 and X-3). **None by D1** | STATE §2 |

**Plainly:** the repository does **not** show that D1 accepted the collaborator invitation or verified any PR; all commits are under D2's name by the Nov 4 decision (D2 will correct D1's credit later). The planned RACI of §5.1 must **not** be presented as fulfilled.

## 5.4 What is missing (for D1 and D2 to decide, not Claude)

1. ❓ **Which real RACI is published**, with the tasks and hours each declares. This manual cannot invent it.
2. ❓ **Credit for D1** in authorship: D2 said they will correct it; until then Git history does not reflect D1's contribution.
3. ⏳ If D1 reviews PRs before delivery, record who approved each.
4. ⏳ Final RACI matrix per area from items 1–3.
