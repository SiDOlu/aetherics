from pydantic_settings import BaseSettings
from dotenv import load_dotenv

load_dotenv()

class Settings(BaseSettings):
    aws_access_key_id: str = ""
    aws_secret_access_key: str = ""
    aws_region: str = "eu-west-2"
    cloudflare_api_token: str = ""
    debug: bool = True

    class Config:
        env_file = ".env"

settings = Settings()
