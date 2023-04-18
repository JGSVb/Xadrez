#!/usr/bin/python
import os
import sys

CONFIG = dict()

CONFIG["global_flags"] = [
        "-g3",
        "-O0",
        "-Wall", 
        "-Wextra",
        "-Wpedantic"
]

CONFIG["libs"] = {
    "./graphics.c": ["sdl2", "SDL2_image", "glib-2.0"],
    "./chess.c":    ["glib-2.0"],
    "./new_main.c": ["sdl2", "SDL2_image", "glib-2.0"],
    "./test.c":     ["sdl2", "SDL2_image", "glib-2.0"],
}

CONFIG["target"] = [
    "./graphics.c",
    "./chess.c",
    "./new_main.c",
    # "./test.c",
]

CONFIG["object_folder"] = "./objects"

CONFIG["output_filename"] = "main"

def get_lib_flags(name):
    return os.popen("pkg-config --libs --cflags " + name).read().strip()

def main():

    filedir = os.path.normpath(os.path.split(__file__)[0])
    workdir = os.getcwd()
    if filedir != workdir:
        print("Mude o diretório atual para o diretório deste script")
        print(f"(CWD:{workdir}) != (FILEDIR:{filedir})")
        exit(1)

    show_only = False
    new_target = False 
    arg_target = []
    recompile = True

    for arg in sys.argv[1:]:
        
        if arg == "-n" or arg == "--new-target":
            new_target = True
        elif arg == "-s" or arg == "--show_only":
            show_only = True
        elif arg == "-r" or arg == "--no-recompile":
            recompile = False
        elif arg.startswith("t="):
            arg_target.append(arg[2:])
        elif arg.startswith("o="):
            CONFIG["output_filename"] = arg[2:]
        else:
            print("-r --no-recompile")
            print("-n --new-target")
            print("-s --show_only")
            print("t=...[.c] t=...[.c] ...")
            print("o=...")
            exit(1)

    if new_target:
        CONFIG["target"].clear()

    for v in arg_target: CONFIG["target"].append(v)

    all_libs_flags = []

    for file in CONFIG["target"]:
        dest = os.path.join(CONFIG["object_folder"], file + ".o")
        command = "gcc " + file + " -o " + dest

        if file in CONFIG["libs"].keys():
            if file not in CONFIG["target"]:
                continue
            for v in CONFIG["libs"][file]:
                lib_flags = get_lib_flags(v)
                all_libs_flags.append(lib_flags)
                command += " " + lib_flags
                command += " -c"

        for flag in CONFIG["global_flags"]:
            command += " " + flag

        print(command)

        if show_only: continue

        if not recompile and os.path.isfile(dest):
            print("\033[35mFicheiro '{}' já existe, logo não foi recompilado\033[0m\n".format(dest))
        else:
            os.system(command)

    assemble = "gcc {} {} {}".format(
            " ".join(map(lambda x: os.path.join(CONFIG["object_folder"], x), os.listdir(CONFIG["object_folder"]))),
            " -o " + CONFIG["output_filename"],
            " ".join(all_libs_flags),
            )
    print(assemble)

    if not show_only:
        os.system(assemble)

if __name__ == "__main__":
    main()
