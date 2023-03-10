#!/usr/bin/python
import subprocess
import argparse
import pathlib

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release'])
    parser.add_argument("profile", help="Conan Profile")
    return parser.parse_args()

def get_profile_path(args):
    conan_directory = pathlib.Path(__file__).resolve().parent.parent
    profile_path = pathlib.PurePath(conan_directory, "profiles", f'{args.profile}')
    return profile_path

def conan_install(conanfile_directory, profile_path, args):
    command = [
        'conan', 'install', f'{conanfile_directory}',
        '--install-folder', f'build/modules', 
        '--settings', f'build_type={args.build_type}',
        '--build', 'missing'
    ]

    if args.profile != "default":
        command.extend(['--profile:build', f'{profile_path}'])
        command.extend(['--profile:host', f'{profile_path}'])

    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

args = parse()
profile_path = get_profile_path(args)
conanfile_directories = [ 'tests', 'examples' ]

for conanfile_directory in conanfile_directories:
    conan_install(conanfile_directory, profile_path, args)