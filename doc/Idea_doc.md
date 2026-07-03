# **Project Specification: Aetherics**

## **Product & Technical Implementation Document**

Aetherics is an Ambient Intelligence platform designed for the global "Silver Economy" to address the elderly care crisis through camera-free, frictionless, passive health and safety monitoring. By combining advanced Edge-AI with non-invasive sensors (mmWave radar, thermal arrays, and MEMS microphones), Aetherics eliminates the "Friction Gap" of wearable devices (charging, skin irritation, cognitive non-compliance) and the "Privacy Paradox" of traditional optical cameras.

While historical legacy platforms focused purely on reactive fall detection, **Aetherics shifts the paradigm entirely to predictive analytics**, analyzing micro-behaviors and vital patterns to mitigate risks before an injury occurs.

## **0\. Commercial Delivery: Direct-to-Consumer (B2C) Subscription Model**

While scalable to institutional healthcare models (such as NHS Virtual Wards), the immediate focus of this specification is a consumer-facing package:

* **The B2C Model:** Families or private caregivers purchase a physical hardware kit ("Aetherics Node") for a home environment.  
* **Subscription Access:** Access to real-time status tracking, historical behavioral insights, configuration dashboards, and priority alert routing is unlocked via a monthly/annual recurring SaaS subscription.

## **1\. Core Problem & Primary Users**

### **1.1 The Core Problem Solved**

* **The Burden of the "Long-Lie":** Undetected falls and delayed intervention cause compounding trauma and cost healthcare states billions (e.g., £4.4 Billion annually in the UK).  
* **The Failure of Wearables:** Pendants and smartwatches are often forgotten, uncharged, or actively rejected by individuals suffering from dementia or cognitive decline.  
* **The Privacy Paradox:** Placing optical surveillance cameras in sensitive spaces like bedrooms or bathrooms causes immense surveillance stigma and presents severe data liabilities under GDPR, HIPAA, and the EU AI Act.

### **1.2 Primary Users**

1. **Families & Private Caregivers:** Relatives looking for ambient peace of mind that a vulnerable loved one is safe at home without infringing upon their dignity or privacy.  
2. **Approved Third-Party Systems:** External care providers or emergency routing platforms that ingest Aetherics alerts into localized workflows.

## **2\. Technical Stack & Infrastructure Preferences**

* **Backend Language:** Python (Asynchronous FastAPI/Flask for rapid async request/response handling and high performance).  
* **Cloud Platform:** Amazon Web Services (AWS) — Utilizing AWS IoT Core for secure ingestion, Lambda for event routing, DynamoDB/TimeStream for high-throughput time-series telemetry, and SNS for push notifications.  
* **Edge Security & CDN:** Cloudflare — Configured for DDoS protection, endpoint caching, and strict API-rate limiting.  
* **Architecture Style:** **API-First Design.** The platform is built entirely decoupled. All clients—whether the official internal app or an approved ecosystem partner—interact via identical core API structures.

## **3\. Hardware Architecture (Prototype Round 1\)**

To prove the sensor-fusion pipeline, this round will utilize a raw breadboard or perfboard mount setup (no enclosure is required for this phase; final antimicrobial ABS or 3D-printed chassis will be reserved for production rounds).

* **Radar Transceiver:** **TI IWR6843ISK** (Evaluation Board, not AOP) — Used for early algorithm development and RF debugging to extract dense, unstructured point-cloud data. (Future rounds will switch to the compact Antenna-on-Package form factor).  
* **Thermal Imaging:** **Melexis MLX90640** (32×24 thermal grid) — For spatial heat mapping and environmental boundary definitions.  
* **Audio Capture:** **STMicroelectronics MP34DT05 MEMS Digital Microphones (Minimum ×2 units)** — Used for directional/structural impact acoustics.  
* **Compute Node:** **Espressif ESP32-S3-DevKitC (×2 units: 1 Active, 1 Backup)** — For local edge calculations, data fusion, and secure Wi-Fi connectivity.  
* **Power:** USB-C breakout boards combined with 5V power adapters.

## **4\. Modular Feature & Hardware Toggles (Admin Fail-Safe Flags)**

To ensure software development, hardware bring-up, and field-testing are never stalled by component delays, sensor failure, high-latency processing bottlenecks, or localized regulatory blockers, **every single architectural feature, hardware peripheral, and predictive software pipeline must be completely isolated behind runtime Feature Flags (toggles)** controllable dynamically from the Admin Dashboard or remote config payload.

\+-----------------------------------------------------------------------------------------+  
|                                    AETHERICS ENGINE                                     |  
\+-----------------------------------------------------------------------------------------+  
       |                                          |                               |  
       v                                          v                               v  
\[HARDWARE LAYER CONFIG\]                 \[ALGORITHMIC TELEMETRY\]         \[PREDICTIVE ANALYTICS\]  
  \- HW\_USE\_MICROPHONE (T/F)               \- COMPUTE\_HEART\_RATE (T/F)      \- RUN\_FALL\_PREDICTION (T/F)  
  \- HW\_USE\_THERMAL    (T/F)               \- COMPUTE\_RESPIRATION(T/F)      \- RUN\_BED\_EXIT\_DETECT (T/F)  
  \- HW\_USE\_RADAR      (T/F)               \- COMPUTE\_COUGH\_DETECT(T/F)     \- RUN\_GAIT\_ANALYSIS   (T/F)

### **4.1 Peripheral HW Toggles**

* **TOGGLE\_MICROPHONE (Boolean):** Disables/Enables MP34DT05 microphone sampling. When False, acoustic classification threads are cleanly terminated, freeing DMA channels and memory.  
* **TOGGLE\_THERMAL (Boolean):** Disables/Enables MLX90640 thermal array mapping. When False, bed-boundary monitoring falls back exclusively to radar point-cloud coordinates.  
* **TOGGLE\_RADAR (Boolean):** Disables/Enables IWR6843ISK parsing.

### **4.2 Software Algorithmic Toggles**

* **PROCESS\_VITALS\_HEART\_RATE / PROCESS\_VITALS\_RESPIRATION (Boolean):** If mathematical micro-displacement calculations cause heap exhaustion or lag on the ESP32-S3 under multi-threaded conditions, developers can toggle vital sign processing off instantly. The backend will adjust its telemetry expectations to handle null vital fields gracefully.  
* **PROCESS\_ACOUSTIC\_COUGH / PROCESS\_ACOUSTIC\_DISTRESS (Boolean):** Disables machine-learning audio classification on the edge, avoiding edge-inference CPU overhead.  
* **PROCESS\_PREDICTIVE\_FALL (Boolean):** Toggles postural sway and gait anomaly processing. If false, alerts default to a purely reactive, classical spatial boundary fall detection mode.

*Any disabled feature must fail gracefully—generating no telemetry packets, emitting default log statuses, and notifying the cloud connection layer without interrupting running threads.*

## **5\. Patient Calibration & Ambient Health Vitals**

To maximize predictive accuracy and prevent false positives, Aetherics implements personalized biometric calibration and passive multi-modal vital sign monitoring.

### **5.1 BMI-Based Edge AI Calibration**

Using static inputs (Age, Height, Weight) provided during the B2C registration, the cloud calculates the user's Body Mass Index (![][image1]):

![][image2]This calibration coefficient is delivered downstream to the Edge Node to tune algorithm thresholds:

* **Point-Cloud Density Filtering:** Adjusts the expected skeletal point-cloud distribution density of the TI IWR6843ISK. Individuals with higher BMIs present a larger radar cross-section (RCS), requiring adaptive clutter-rejection bounds.  
* **Velocity & Momentum Profiles:** Calibrates the expected descent velocity profile during fall forecasting, distinguishing an accelerated slip from a controlled, rapid downward transition (e.g., plopping into a soft couch).  
* **Postural Sway Tolerances:** Adjusts the center-of-gravity (CoG) displacement models used in predicting fall probabilities based on height-to-weight structural ratios.

### **5.2 Multi-Sensor Health Vitals Monitored (MVP Scope)**

By orchestrating the three-tiered sensor array, the Aetherics Node passively aggregates clinical-grade biomarkers without any physical contact:

#### **A. Cardiorespiratory Telemetry (TI IWR6843ISK mmWave Radar)**

The high-frequency ![][image3] FMCW signals capture sub-millimeter chest displacement mapping, isolating physiological frequencies:

* **Heart Rate (HR):** Extracts micro-vibrations from myocardial contractions (ballistocardiography).  
* **Heart Rate Variability (HRV):** Measures Root Mean Square of Successive Differences (RMSSD) to track autonomic nervous system stress or cardiovascular fatigue.  
* **Respiration Rate (RR):** Tracks macro-displacements from the respiratory cycle (breathing rate), flagging tachypnea or sudden bradypnea.  
* **Sleep Apnea Biomarkers:** Monitors prolonged chest immobility segments (apnea intervals) coupled with rapid gasping patterns.

#### **B. Thermoregulatory Trends (Melexis MLX90640 Thermal Grid)**

The ![][image4] far-infrared thermal array acts as a continuous non-contact core/surface temperature scanner:

* **Fever Screening:** Regularly evaluates facial/temporal heat signatures during sleep or stationary sitting against ambient room temperature differentials.  
* **Circadian Body Temperature Tracking:** Tracks the natural daily dips and rises in skin temperature, identifying sleep cycle disruptions or thermal dysregulation (hypothermia/hyperthermia indicators).

#### **C. Acoustic Biomarkers (MP34DT05 MEMS Microphones)**

The dual digital microphone array monitors localized respiratory soundscapes:

* **Cough/Wheeze Monitoring:** Machine learning classifiers on the edge detect and count coughing fits or respiratory wheezes, helping predict COPD exacerbations or heart failure fluid build-up.  
* **Snoring & Sleep Disturbances:** Records decibel levels and structural frequencies of sleep-onset sounds to index deep vs. restless sleep cycles.  
* **Pain & Distress Acoustics:** Identifies involuntary vocal cues, such as groaning or verbal distress signals, to register pain event telemetry.

## **6\. Telemetry Schema & Predictive Endpoints**

Aetherics transitions past basic detection by monitoring micro-movements, gait velocity deviations, and spatial positioning against a learned baseline.

### **6.1 alert.fall.predictive**

Fires when the Edge-AI identifies critical gait degradation or structural instability, signaling an elevated probability of an imminent fall.

{  
  "event\_type": "alert.fall.predictive",  
  "timestamp": "2026-07-02T11:42:00Z",  
  "device\_id": "aeth-node-0912X",  
  "location": "living\_room",  
  "calibration\_phase": "active\_monitoring",   
  "confidence\_status": "high",  
  "telemetry\_data": {  
    "gait\_velocity\_deviation": \-0.35,  
    "postural\_sway\_index": 7.8,  
    "historical\_baseline\_variance": "high\_risk\_threshold\_exceeded",  
    "bmi\_calibration\_multiplier": 1.12  
  }  
}

* **calibration\_phase** Enums: learning\_baseline, calibrated, active\_monitoring, recalibrating  
* **confidence\_status** Enums: low, medium, high

### **6.2 alert.bed\_exit.dangling**

Fires when an individual is sitting on the edge of the bed with legs dangling, signaling an unassisted, high-risk mobility transfer attempt before they ever stand up.

{  
  "event\_type": "alert.bed\_exit.dangling",  
  "timestamp": "2026-07-02T03:15:22Z",  
  "device\_id": "aeth-node-0912X",  
  "location": "bedroom",  
  "calibration\_phase": "calibrated",  
  "confidence\_status": "medium",  
  "telemetry\_data": {  
    "thermal\_boundary\_breach": true,  
    "duration\_seconds\_dangling": 45,  
    "micro\_movement\_frequency\_hz": 1.4,  
    "current\_respiration\_rate\_bpm": 22.4  
  }  
}

### **6.3 telemetry.vitals.daily**

Aggregates daily non-contact biometric telemetry to create longitudinal health profiles.

{  
  "event\_type": "telemetry.vitals.daily",  
  "timestamp": "2026-07-02T23:59:59Z",  
  "device\_id": "aeth-node-0912X",  
  "metrics": {  
    "mean\_heart\_rate\_bpm": 72.1,  
    "resting\_heart\_rate\_bpm": 64.5,  
    "mean\_respiration\_rate\_bpm": 14.8,  
    "mean\_hrv\_rmssd\_ms": 32.4,  
    "average\_thermal\_baseline\_celsius": 36.6,  
    "respiratory\_anomalies\_detected": {  
      "cough\_count": 14,  
      "wheezing\_episodes": 2,  
      "apnea\_events": 1  
    }  
  }  
}

## **7\. Software Deliverables & Streaming Protocols**

### **7.1 Client Interfaces**

1. **Mobile Application:** Consumer interface for families to view live safety status indexes, review historical sleep/movement trends, and receive push alerts.  
2. **Web Version:** Responsive user application containing accounting configurations, billing tools, and deep historical telemetry plots.  
3. **Admin Dashboard:** Central multi-tenant dashboard for Aetherics operations to provision devices, monitor network load, push firmware revisions, and manage client subscriptions and feature toggles.

### **7.2 Streaming Architecture: Solving the "One-Way Street"**

Traditional HTTP request/response paradigms prevent real-time, low-latency alert injection down to client interfaces. Aetherics employs a unified dual-channel connection pattern:

1. **Persistent WebSockets:** The application natively utilizes a persistent, bi-directional WebSocket connection (ws:// / wss://) for near-zero latency streaming of state-changes and predictive alerts from the Python backend.  
2. **HTTP Polling Fallback with Exponential Backoff:** If firewall proxy issues, cellular network drops, or carrier restrictions interrupt the WebSocket pipeline, the client library will immediately fall back to standard HTTP GET polling.  
3. **Reconnection Loop:** To prevent overloading cloud services when reconnecting, a dynamic exponential backoff interval with added random jitter governs WebSocket retry loops. The reconnection backoff delay (![][image5]) is modeled as:![][image6]  
   Where ![][image7], ![][image8], and ![][image9] is a small randomized variable to prevent thundering herd problems on our servers.

## **8\. Developer Interfaces, Testing, & Diagnostics**

### **8.1 Automated Documentation & Swagger UI**

* **Auto-Generation:** The Python application (via FastAPI or standard OpenAPI generators) must dynamically generate documentation directly from code data model schemas.  
* **Active Interactive Interface:** The active Swagger UI must be served live directly at the **/docs** path (e.g., http://localhost:8000/docs or https://api.aetherics.io/docs). Developers, engineers, and authorized partners must be able to securely execute mock payloads, inspect endpoints, and test live backend integrations directly from any standard web browser.

### **8.2 Service Diagnostics**

* **Health Checkpoint:** A dedicated, lightweight **/health** or /status endpoint must be public-facing. It will return health arrays evaluating cloud connectivity, primary database status, active hardware thread states, and cache memory usage.

## **9\. Future Roadmap & Out-of-Scope (To Be Added Post-MVP)**

To maintain a laser-focused MVP timeline, the following advanced predictive indicators and hardware-enhanced features are categorized as **Phase 2 Development** and are out of scope for the Round 1 physical prototype:

### **9.1 Advanced Biometric & Behavioral Markers (Phase 2 Software)**

* **UTI/Infection Predictive Flagging:** Combining continuous nighttime tossing/turning frequency (via radar activity indexing) with highly elevated nocturnal bathroom frequency tracking to detect UTIs or systemic infections early.  
* **Goniometry / Range of Motion (ROM) Profiling:** Tracking limb extension angles and speed of movement during planned sit-to-stand movements to monitor musculoskeletal decay.  
* **Spatial Gait Speed Deceleration mapping:** Long-term charting of spatial movement velocity across high-traffic indoor transit vectors (e.g., hallways) to identify neurological or frailty onset.

### **9.2 Contextual Environmental Acoustics (Phase 2 Audio classifiers)**

* **Environmental Emergency Soundscapes:** Machine learning classifiers targeting non-physiological sounds: running tap-water alerts (flooding detection), smoke alarms, carbon monoxide alarms, and breaking window glass.  
* **Directional Sound Ingestion (Acoustic Localization):** Using the MEMS dual microphone arrays to construct acoustic directional vectors, calculating exactly *where* inside a monitored zone an alarm is radiating from.

### **9.3 Complex Spatial Mapping (Phase 2 Cloud Fusion)**

* **Multi-Person Clutter Segregation:** Distinguishing visitors, caregivers, or pets from the calibrated primary user. Dynamic separation of dual point-clouds to prevent state-alert cross-contamination.

## **10\. Definition of a "Successful First Version" (MVP)**

The baseline milestone for an acceptable first prototype iteration is reached when:

1. **Validated Predictive Pipeline:** The sensor-fusion models successfully trigger distinct alert.bed\_exit.dangling payloads using input from the mmWave radar and thermal grids without manual human prompts.  
2. **Resilient Protocol Switching:** Simulating network failures gracefully demotes client connections from WebSockets to HTTP polling and handles exponential backoff reconnection smoothly.  
3. **Clean Decoupling:** Hardware and software modular toggles safely deactivate delayed components (e.g., TOGGLE\_MICROPHONE=False or PROCESS\_VITALS\_HEART\_RATE=False) while maintaining completely stable monitoring functionality across active components.  
4. **Interactive Validation:** The complete API ecosystem can be monitored, tested, and validated in real time by an outside developer via the interactive Swagger interface exposed at /docs.
