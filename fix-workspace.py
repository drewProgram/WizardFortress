#!/usr/bin/env python3
"""Reaplica os ajustes de janela após regenerar o workspace pela Unreal.

Uso: python3 fix-workspace.py
Pode ser executado de qualquer diretório. Não inicia a Unreal.
"""

import json
from pathlib import Path


def main():
    workspace = Path(__file__).resolve().with_name("WizardFortress.code-workspace")
    original = workspace.read_text(encoding="utf-8")
    data = json.loads(original)
    configuration_name = "Launch WizardFortressEditor (Development)"
    configuration = next(
        (item for item in data["launch"]["configurations"]
         if item["name"] == configuration_name),
        None,
    )
    if configuration is None:
        raise SystemExit(f"Configuração não encontrada: {configuration_name}")

    # Preserva os demais argumentos e variáveis de ambiente.
    args = configuration.setdefault("args", [])
    command = "Slate.EnableTooltips 0"
    for index, argument in enumerate(args):
        if argument.startswith("-ExecCmds="):
            commands = args[index][len("-ExecCmds="):].split(",")
            commands = [value for value in commands
                        if not value.strip().lower().startswith("slate.enabletooltips ")]
            commands.append(command)
            args[index] = "-ExecCmds=" + ",".join(commands)
            break
    else:
        args.append(f"-ExecCmds={command}")

    environment = configuration.setdefault("environment", [])
    for variable in environment:
        if variable["name"] == "SDL_VIDEODRIVER":
            variable["value"] = "x11"
            break
    else:
        environment.append({"name": "SDL_VIDEODRIVER", "value": "x11"})

    if data == json.loads(original):
        print("Workspace já configurado; nenhuma alteração necessária.")
        return

    workspace.write_text(
        json.dumps(data, ensure_ascii=False, indent="\t") + "\n",
        encoding="utf-8",
    )
    print(f"Ajustes X11 e tooltips restaurados em {configuration_name}.")


if __name__ == "__main__":
    main()
