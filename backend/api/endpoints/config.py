from fastapi import APIRouter, HTTPException
from pydantic import BaseModel
from typing import Optional

router = APIRouter()

class DeviceConfig(BaseModel):
    device_id: str
    hw_use_microphone: Optional[bool] = None
    hw_use_thermal: Optional[bool] = None
    hw_use_radar: Optional[bool] = None
    process_vitals_heart_rate: Optional[bool] = None
    process_vitals_respiration: Optional[bool] = None
    process_predictive_fall: Optional[bool] = None

# Local simulation store
device_configs_db = {}

@router.get("/config/{device_id}", response_model=DeviceConfig)
async def get_config(device_id: str):
    if device_id not in device_configs_db:
        # Return default config if not initialized
        return DeviceConfig(
            device_id=device_id,
            hw_use_microphone=True,
            hw_use_thermal=True,
            hw_use_radar=True,
            process_vitals_heart_rate=True,
            process_vitals_respiration=True,
            process_predictive_fall=True
        )
    return device_configs_db[device_id]

@router.post("/config/update", response_model=dict)
async def update_config(config: DeviceConfig):
    # Store in memory DB for simulation
    device_configs_db[config.device_id] = config
    
    # In production, this would trigger an AWS IoT Core shadow or MQTT publish
    print(f"Config pushed to AWS IoT Core Shadow for device {config.device_id}")
    
    return {"status": "success", "pushed_to_device": True}
