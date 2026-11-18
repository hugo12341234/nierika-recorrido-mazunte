# Individual statement — D2 (321332086) · **DRAFT FOR HUGO'S REVIEW**

> **Not final text.** It was drafted by an assistant from what the repository shows. It cannot know your experience. Rewrite it in your own voice, delete anything that is not true, answer the ❓ items, and remove this box. Suggested final length: 250–400 words.

## 1. My experience in the project

I worked on Nierika Studio, a first-person 3D walkthrough of a Day of the Dead night in Mazunte, Oaxaca, written in C++17 and OpenGL 3.3 Core. According to the repository I was the main author of the code, the Blender models and textures, the audio, and the documentation (about 460 commits under my name). ❓ I also directed an AI assistant (Claude) to generate many of the scripts and drafts, and I reviewed, compiled and tested the result on my own PC. [Confirm or rewrite: say exactly what you did by hand and what you supervised.]

## 2. My participation

- ❓ Planning and scoping: I replanned the schedule on 18/10 and recorded each decision in `docs/ESTADO_PROYECTO.md`.
- Modeling and integration: own models in Blender 5.2, loading with Assimp, scene layout, collisions, animations (10 in total, including KeyFrames with slerp), cultural information cards with Dear ImGui, ambient audio with miniaudio, a guided tour with narration.
- Performance: measured 45 FPS minimum and about 60 FPS average on an integrated-graphics i5 (11th gen) after frustum culling (PR #148).
- Integration of my teammate's work: the lighting base and several Maya models that D1 sent me (PR #16, #17, #39, #111, #115).
- ❓ Hours per week: [your honest estimate].

## 3. Difficulties

- The work was concentrated in one person and in the last two weeks: 300 of the 469 commits are from 11–18 November, and most pull requests were merged without a second person's review (documented in the project state file).
- ❓ [Technical difficulties you actually faced: scale and pivots, transparency, FPS on integrated graphics, vcpkg setup.]

## 4. What I learned

❓ [Pick what is true: Blender with Python (`bpy`), GLSL lighting and wave shaders, quaternion interpolation (slerp), vcpkg, working with an AI assistant and verifying its output, writing documentation with honest evidence.]

## 5. What I would do differently

❓ [e.g., agree on a weekly review with my teammate from the first week and start the manuals earlier.]

---
Date: [dd/mm/2026]  ·  Account: 321332086
