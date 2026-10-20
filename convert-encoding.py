from pathlib import Path

for root in (Path("Source"), Path("Plugins")):
    if not root.exists():
        continue

    for path in root.rglob("*"):
        if not path.is_file() or path.suffix not in (".h", ".cpp"):
            continue

        data = path.read_bytes()

        if data.startswith((b"\xff\xfe", b"\xfe\xff")):
            content = data.decode("utf-16")
            path.write_bytes(content.encode("utf-8"))
            print(f"Convertido: {path}")