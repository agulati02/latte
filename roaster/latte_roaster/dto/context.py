from dataclasses import dataclass

from roaster.latte_roaster.dto.protocol import GrindRequest


@dataclass
class ImportContext:

    request: GrindRequest

    model = None

    tokenizer = None

    exported_program = None

    ir = None