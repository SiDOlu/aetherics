from fastapi import FastAPI
from fastapi.openapi.utils import get_openapi
from api.endpoints import health, telemetry, config, streaming

app = FastAPI(
    title="Aetherics API",
    description="Ambient Intelligence platform for elderly care monitoring.",
    version="0.1.0",
)

# Include routers
app.include_router(health.router, tags=["System"])
app.include_router(telemetry.router, tags=["Telemetry"])
app.include_router(config.router, tags=["Configuration"])
app.include_router(streaming.router, tags=["Streaming"])

@app.get("/", tags=["System"])
async def root():
    return {"message": "Welcome to the Aetherics API. Visit /docs for documentation."}

# Custom OpenAPI configuration to ensure /docs is clean
def custom_openapi():
    if app.openapi_schema:
        return app.openapi_schema
    openapi_schema = get_openapi(
        title=app.title,
        version=app.version,
        description=app.description,
        routes=app.routes,
    )
    app.openapi_schema = openapi_schema
    return app.openapi_schema

app.openapi = custom_openapi
