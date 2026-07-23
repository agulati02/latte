import json

from roaster.latte_roaster.dto.protocol import GrindEvent


class EventEmitter:

    def emit(self, event: GrindEvent):

        print(
            json.dumps(event),
            flush=True
        )
