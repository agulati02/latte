from pydantic_settings import BaseSettings, SettingsConfigDict


class AppSettings(BaseSettings):
    model_id: str
    model_version: str = "latest"

    model_config = SettingsConfigDict(frozen=True)

app_config = AppSettings()
