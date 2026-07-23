from latte_roaster.core import RosterPipeline
from latte_roaster.config import app_config


def main():
    pipeline = RosterPipeline(config=app_config)
    pipeline.run()


if __name__ == "__main__":
    main()
