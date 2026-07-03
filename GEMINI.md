# Aetherics Agent Memory - Aethera Labs

## ⚠️ Agent Initialization Mandate
**CRITICAL:** Every agent MUST read this `GEMINI.md` file immediately upon initialization. This is the single source of truth for the project state and objectives.

## 📝 Project Overview
- **Project Name:** Aetherics
- **Owner:** Aethera Labs
- **Status:** Phase 3: Sensor Fusion & Predictive Logic (Phase 1 & 2 Complete)

## 🎯 Current Objectives
- [x] **Phase 1: Infrastructure & API Foundation** (AWS Provisioning, FastAPI Core, Telemetry Endpoints, Cloudflare Security).
- [x] **Phase 2: Edge Node Baseline & Toggle System** (Hardware Bring-up, Toggle Registry, Heartbeat, Remote Config).
- [x] **Phase 3: Sensor Fusion & Predictive Logic**
    - [x] Radar (TI IWR6843) Driver Implementation.
    - [x] Thermal (Melexis MLX90640) Driver Implementation.
    - [x] Sensor Fusion Pipeline & Predictive Algorithms.
- [x] **Phase 4: Real-time Streaming & UX**
    - [x] WebSocket Server Implementation.
    - [x] Resilience Layer.
    - [x] Admin & User Dashboards.
- [/] **Phase 5: Validation & MVP Hardening** (Stress Testing, False Positive Audit, Graceful Degradation, API Validation).


## 📋 Implementation Roadmap
Refer to `/doc/Implementation_Plan.md` for the detailed technical breakdown of each phase.


## 🧠 Architectural & Philosophical Decisions
- **Privacy Paradox (Zero Raw Data):** The fundamental philosophy is to process raw sensor data at the edge and never transmit raw point-cloud or thermal images to the cloud to ensure GDPR/HIPAA compliance.
- **API-First Development:** The OpenAPI contract is the single source of truth for all communication between edge and cloud.
- **Modular Firmware Architecture:** Sensor integration uses a unified `Sensor` abstract class to provide a consistent interface for diverse hardware components.
- **State-Machine Based Sensor Fusion:** Adopted a state-machine based approach for sensor fusion (`ROOM_EMPTY`, `OCCUPIED_BED`, `DANGLING_EDGE`, `FALL_DETECTED`) to prioritize robustness, contextual awareness, and reliability over simple thresholding.
- **Observability:** Heartbeat-based monitoring and remote toggle management for edge-node health.
- **Frontend Architecture:**
    - **Stack:** React (TypeScript) for high-performance, modular UI.
    - **Styling:** Vanilla CSS (CSS Variables) for custom, theme-aware aesthetics without framework overhead.
    - **Design Philosophy:** "Aethera Dark Mode"—data-dense, high-contrast, tech-forward, with uncompromising accessibility for the "Silver Economy" user.
    - **State Management:** React hooks interfacing with the `AethericsClient` resilience wrapper.

