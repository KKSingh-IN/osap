#!/usr/bin/env python3

# Copyright 2024 Kamlesh Singh
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import os
import subprocess
import sys

def run_command(command, cwd=None):
    """
    Executes a shell command and captures its output.

    Args:
        command (str): The command to execute.
        cwd (str, optional): The current working directory for the command. Defaults to None.

    Returns:
        str: The standard output of the command.

    Raises:
        SystemExit: If the command returns a non-zero exit code (indicating an error).
    """
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, cwd=cwd)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print(f"Error executing command: {command}")
        print(stderr.decode())
        sys.exit(1)
    return stdout.decode()

def get_available_apps():
    """
    Discovers available applications in the 'apps' directory.

    Returns:
        list: A list of directory names, each representing an available application.
    """
    apps_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "apps")
    return [d for d in os.listdir(apps_dir) if os.path.isdir(os.path.join(apps_dir, d))]

def configure_build(app, build_type, options):
    """
    Configures the CMake build for a specified application.

    Args:
        app (str): The name of the application to configure.
        build_type (str): The CMake build type (e.g., "Debug", "Release").
        options (dict): A dictionary of CMake options (e.g., {"USE_VSOMEIP": "ON"}).
    """
    app_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "apps", app)
    build_dir = os.path.join(app_dir, "build")
    os.makedirs(build_dir, exist_ok=True)
    
    cmake_options = [f"-DCMAKE_BUILD_TYPE={build_type}"]
    for option, value in options.items():
        cmake_options.append(f"-D{option}={value}")
    
    cmake_command = f"cmake .. {' '.join(cmake_options)}"
    run_command(cmake_command, cwd=build_dir)

def build_project(app):
    """
    Builds the specified application using CMake.

    Args:
        app (str): The name of the application to build.
    """
    app_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "apps", app)
    build_dir = os.path.join(app_dir, "build")
    
    if not os.path.exists(os.path.join(build_dir, "CMakeCache.txt")):
        print(f"Build directory not configured. Running configuration...")
        configure_build(app, "Debug", {})
    
    run_command("cmake --build .", cwd=build_dir)

def run_tests(app):
    """
    Runs CTest for the specified application.

    Args:
        app (str): The name of the application to test.
    """
    app_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "apps", app)
    build_dir = os.path.join(app_dir, "build")
    run_command("ctest --output-on-failure", cwd=build_dir)

def main():
    """
    Main function to parse command-line arguments and orchestrate the build and test process.
    """
    available_apps = get_available_apps()

    parser = argparse.ArgumentParser(description="Setup and build helper for Automotive Applications")
    parser.add_argument("--app", choices=available_apps, required=True, help="Application to build/test")
    parser.add_argument("--configure", action="store_true", help="Configure the build")
    parser.add_argument("--build", action="store_true", help="Build the project")
    parser.add_argument("--test", action="store_true", help="Run tests")
    parser.add_argument("--build-type", choices=["Debug", "Release", "RelWithDebInfo"], default="Debug", help="Build type")

    args = parser.parse_args()

    options = {
    }

    # Determine if configuration is needed:
    # 1. If --configure flag is explicitly set.
    # 2. If --build is set and the CMakeCache.txt file (indicating a configured build) is missing.
    if args.configure or (args.build and not os.path.exists(os.path.join("apps", args.app, "build", "CMakeCache.txt"))):
        configure_build(args.app, args.build_type, options)

    # Perform the build if requested
    if args.build:
        build_project(args.app)

    # Run tests if requested
    if args.test:
        run_tests(args.app)

if __name__ == "__main__":
    main()