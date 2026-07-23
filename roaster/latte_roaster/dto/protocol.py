from dataclasses import dataclass


@dataclass
class GrindRequest:

    model: str

    output: str

    revision: str = "main"

@dataclass
class GrindEvent:

    request: GrindRequest

    status: str

    message: str = ""
