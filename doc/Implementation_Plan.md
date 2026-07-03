# Implementation Plan: Project Aetherics

**Author:** Gemini CLI (Orchestrator)
**Date:** 2026-07-02
**Status:** Approved
**Related Doc:** `Idea_doc.md`

## 1. Background & Motivation
Aetherics is a high-moat Ambient Intelligence platform for the "Silver Economy." The goal is to resolve the "Privacy Paradox" (security vs. dignity) by using camera-free, passive sensors (mmWave, Thermal, Acoustic) to provide predictive health and safety monitoring for the elderly. 

The primary technical challenge is the orchestration of an Edge-AI pipeline on resource-constrained hardware (ESP32-S3) that must maintain zero-latency alerts while ensuring "Zero Raw Data" cloud upload for regulatory compliance (GDPR/HIPAA).

## 2. Scope & Impact
### In-Scope (MVP)
- **Edge Node:** Basic sensor fusion using TI IWR6843ISK, Melexis MLX90640, and MEMS Mics.
- **Predictive Pipeline:** Implementation of `alert.bed_exit.dangling` and `alert.fall.predictive`.
- **Backend:** Asynchronous Python API (FastAPI) on AWS.
- **Telemetry:** BMI-based calibration for radar point-cloud density.
- **Communication:** Dual-channel WebSocket + HTTP Polling with exponential backoff.
- **Controls:** Dynamic Feature Toggles for all sensors and algorithms.
- **Interfaces:** Admin Dashboard, Mobile/Web status view, and Swagger UI.

### Out-of-Scope (Phase 2)
- Multi-person clutter segregation.
- UTI/Infection predictive flagging.
- Advanced Environmental Acoustics (Smoke/CO alarms).
- Final antimicrobial ABS enclosure.

## 3. Proposed Solution (Architecture)

### 3.1 System Architecture
The system follows a decoupled, API-First architecture:
`[Sensors] -> [ESP32-S3 (Edge AI)] -> [Cloudflare (Security)] -> [AWS FastAPI Backend] -> [WebSocket/HTTP] -> [Client Apps]`

### 3.2 Component Breakdown
- **The Edge Node (Firmware):** 
    - **Language:** C++/Arduino (ESP-IDF).
    - **Logic:** Local point-cloud processing $ightarrow$ Feature Extraction $ightarrow$ Alert Generation $ightarrow$ Cloud Upload.
    - **State Management:** Implements a "Toggle Registry" that checks remote config before initiating sensor threads.
- **The Cloud Backend (API):**
    - **Language:** Python 3.11+ (FastAPI).
    - **Database:** DynamoDB for device state; TimeStream for longitudinal health vitals.
    - **Messaging:** AWS IoT Core for device ingestion; SNS for push notifications.
    - **Streaming:** FastAPI WebSockets for real-time state updates to clients.
- **The Client Interfaces:**
    - **Admin:** React-based dashboard for provisioning and toggle management.
    - **End-User:** Mobile/Web app for status monitoring.

## 4. Implementation Phases

### Phase 1: Infrastructure & API Foundation (The "Skeleton")
*Goal: Establish the cloud-to-client pipeline.*
- [ ] **AWS Provisioning:** Set up IoT Core, DynamoDB, and TimeStream.
- [ ] **Backend Core:** Implement FastAPI base with `/health` and `/docs` (Swagger).
- [ ] **Telemetry Endpoints:** Create ingestion endpoints for `alert.*` and `telemetry.*` payloads.
- [ ] **Auth & Security:** Configure Cloudflare API rate limiting and AWS IAM roles.

### Phase 2: Edge Node Baseline & Toggle System (The "Heartbeat")
*Goal: Secure connectivity and remote control.*
- [ ] **Hardware Bring-up:** Initialize ESP32-S3 with secure Wi-Fi and AWS IoT Core connectivity.
- [ ] **The Toggle Registry:** Implement the runtime feature flag system (`TOGGLE_RADAR`, etc.) in firmware.
- [ ] **Heartbeat Mechanism:** Implement a periodic status packet reporting active sensors and CPU/RAM health.
- [ ] **Remote Config:** Enable the backend to push toggle updates to the node in real-time.

### Phase 3: Sensor Fusion & Predictive Logic (The "Intelligence")
*Goal: Turning raw waves into actionable alerts.*
- [ ] **Radar Pipeline:** Integrate TI IWR6843ISK $ightarrow$ implement point-cloud filtering.
- [ ] **Thermal Integration:** Integrate Melexis MLX90640 $ightarrow$ implement boundary detection.
- [ ] **BMI Calibration:** Implement the cloud-to-edge calibration coefficient for RCS (Radar Cross Section).
- [ ] **Predictive Logic:** Develop the `bed_exit.dangling` and `fall.predictive` algorithms on the edge.
- [ ] **Acoustic Baseline:** Basic MEMS mic integration for structural impact detection.

### Phase 4: Real-time Streaming & UX (The "Interface")
*Goal: Zero-latency delivery to the caregiver.*
- [ ] **WebSocket Server:** Implement the bi-directional stream in FastAPI.
- [ ] **Resilience Layer:** Build the HTTP Polling fallback and Exponential Backoff logic in the client library.
- [ ] **Admin Dashboard:** Build the UI for device provisioning and feature toggle switching.
- [ ] **End-User App:** Basic "Safe/Alert" status view and historical vital trends.

### Phase 5: Validation & MVP Hardening (The "Seal") - Work Started 2026-07-03
*Goal: Verify against the Definition of Success.*
- [ ] **Protocol Stress Test:** Simulate network drops to verify WebSocket $ightarrow$ HTTP transition.
- [ ] **False Positive Audit:** Test BMI calibration against different user profiles.
- [ ] **Graceful Degradation Test:** Toggle off the Microphone and verify the rest of the system remains stable.
- [ ] **API Validation:** Full end-to-end test via Swagger UI.

## 5. Verification & Testing
- **Unit Testing:** Pytest for backend logic; Unity/Ceedling for ESP32 firmware.
- **Integration Testing:** Mock sensor payloads injected via Python scripts to verify cloud routing.
- **Field Testing:** "Dry run" with a physical test subject for `bed_exit.dangling` validation.
- **Compliance Audit:** Verification that no raw point-cloud data is stored in AWS.

## 6. Rollback & Risk Mitigation
- **Firmware Safety:** Implement a dual-boot partition (A/B) on the ESP32 for safe OTA (Over-the-Air) updates.
- **Circuit Protection:** Use USB-C breakout boards with over-current protection to prevent hardware fry during prototype rounds.
- **Data Fallback:** If TimeStream exceeds budget, implement a DynamoDB TTL (Time-to-Live) archive for old telemetry.
