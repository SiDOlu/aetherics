from fastapi import APIRouter

router = APIRouter()

@router.get("/health", response_model=dict)
async def get_health():
    return {
        "status": "healthy",
        "version": "0.1.0",
        "services": {
            "database": "connected",
            "iot_core": "connected",
            "cache": "connected"
        }
    }
