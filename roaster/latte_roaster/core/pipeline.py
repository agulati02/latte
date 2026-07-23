from pydantic_settings import BaseSettings


class RosterPipeline:
    def __init__(self, config: BaseSettings):
        self.config = config

    def run(self):
        # TODO Implement the pipeline logic here
        pass
