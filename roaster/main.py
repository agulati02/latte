import json
import time


def emit(event):
    print(json.dumps(event), flush=True)


emit({"type": "started"})

for i in [25, 50, 75, 100]:
    time.sleep(1)
    emit({
        "type": "progress",
        "value": i
    })

emit({
    "type": "completed",
    "artifact": "qwen.latte"
})