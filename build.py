#!/usr/bin/python
import os
import sys

global_flags = [
        "-c",
        "-g3",
        "-O0",
        "-Wall", 
        "-Wextra",
        "-Wpedantic"
]

libs = {
    "./graphics.c": ["sdl2", "SDL2_image"],
    "./chess.c":    ["glib-2.0"],
    "./new_main.c":   ["sdl2", "SDL2_image", "glib-2.0"],
}

target = [
    "./graphics.c",
    "./chess.c",
    "./new_main.c"
]

object_folder = "./objects"

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
        else:
            print("-n --new-target")
            print("-s --show_only")
            print("t=...[.c] t=...[.c] ...")
            exit(1)

    if new_target:
        target.clear()

    for v in arg_target: target.append(v)

    if os.path.relpath(__file__) != os.path.basename(__file__):
        print("{} != {}".format(os.path.relpath(__file__), os.path.basename(__file__)))
        exit(1)

    for file in target:
        command = "gcc " + file + " -o " + os.path.join(object_folder, file + ".o")

        if file in libs.keys():
            for v in libs[file]:
                command += " " + os.popen("pkg-config --libs --cflags " + v).read().strip()

        for flag in global_flags:
            command += " " + flag

        print(command)

        if not show_only:
            os.system(command)

if __name__ == "__main__":
    main()
