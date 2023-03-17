#!/usr/bin/python
import os
import sys

CONFIG = dict()

CONFIG["global_flags"] = [
        "-c",
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
}

CONFIG["target"] = [
    "./graphics.c",
    "./chess.c",
    "./new_main.c"
]

CONFIG["object_folder"] = "./objects"

CONFIG["output_filename"] = "main"

def get_lib_flags(name):
    return os.popen("pkg-config --libs --cflags " + name).read().strip()


def main():

    show_only = False
    new_target = False 
    arg_target = []

    for arg in sys.argv[1:]:
        
        if arg == "-n" or arg == "--new-target":
            new_target = True
        elif arg == "-s" or arg == "--show_only":
            show_only = True
        elif arg.startswith("t="):
            arg_target.append(arg[2:])
        elif arg.startswith("o="):
            CONFIG["output_filename"] = arg[2:]
        else:
            print("-n --new-target")
            print("-s --show_only")
            print("t=...[.c] t=...[.c] ...")
            print("o=...")
            exit(1)

    if new_target:
        CONFIG["target"].clear()

    for v in arg_target: CONFIG["target"].append(v)

    if os.path.relpath(__file__) != os.path.basename(__file__):
        print("{} != {}".format(os.path.relpath(__file__), os.path.basename(__file__)))
        exit(1)

    all_libs_flags = []

    for file in CONFIG["target"]:
        command = "gcc " + file + " -o " + os.path.join(CONFIG["object_folder"], file + ".o")

        if file in CONFIG["libs"].keys():
            for v in CONFIG["libs"][file]:
                lib_flags = get_lib_flags(v)
                all_libs_flags.append(lib_flags)
                command += " " + lib_flags

        for flag in CONFIG["global_flags"]:
            command += " " + flag

        print(command)

        if not show_only:
            os.system(command)

    assemble = "gcc {} {} {}".format(
            " ".join(map(lambda x: os.path.join(CONFIG["object_folder"], x), os.listdir(CONFIG["object_folder"]))),
            " -o " + CONFIG["output_filename"],
            " ".join(all_libs_flags)
            )
    print(assemble)

    if not show_only:
        os.system(assemble)

if __name__ == "__main__":
    main()
