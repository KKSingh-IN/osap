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

def configure_build(project_name, project_path, build_type, options):
    """
    Configures the CMake build for a specified application.

    Args:
        app (str): The name of the application to configure.
        build_type (str): The CMake build type (e.g., "Debug", "Release").
        options (dict): A dictionary of CMake options (e.g., {"USE_VSOMEIP": "ON"}).
    """
    build_dir = os.path.join(project_path, "build")
    os.makedirs(build_dir, exist_ok=True)

    cmake_options = [f"-DCMAKE_BUILD_TYPE={build_type}"]
    for option, value in options.items():
        cmake_options.append(f"-D{option}={value}")

    cmake_command = f"cmake .. {' '.join(cmake_options)}"
    print(f"Configuring {project_name} in {build_dir}...")
    run_command(cmake_command, cwd=build_dir)

def build_project(project_name, project_path):
    """
    Builds the specified project using CMake.

    Args:
        project_name (str): The name of the project to build.
        project_path (str): The absolute path to the project's root directory.
    """
    build_dir = os.path.join(project_path, "build")

    # If the build directory isn't configured, configure it first
    if not os.path.exists(os.path.join(build_dir, "CMakeCache.txt")):
        print(f"Build directory for {project_name} not configured. Running configuration (defaulting to Debug with no options)...")
        configure_build(project_name, project_path, "Debug", {}) # Default to Debug for unconfigured common projects

    print(f"Building {project_name} in {build_dir}...")
    run_command("cmake --build .", cwd=build_dir)

def run_tests(project_name, project_path):
    """
    Runs CTest for the specified project.

    Args:
        project_name (str): The name of the project to test.
        project_path (str): The absolute path to the project's root directory.
    """
    build_dir = os.path.join(project_path, "build")
    print(f"Running tests for {project_name} in {build_dir}...")
    run_command("ctest --output-on-failure", cwd=build_dir)
    
def build_common_libs_and_apps(build_type):
    """
    Discovers and builds common libraries and applications in 'srv/'.

    Args:
        build_type (str): The CMake build type (e.g., "Debug", "Release").
    """
    script_dir = os.path.dirname(os.path.abspath(__file__))
    srv_com_dir = os.path.join(script_dir, "srv", "com")

    if not os.path.isdir(srv_com_dir):
        print(f"Warning: '{srv_com_dir}' not found. Skipping common libraries and applications build.")
        return

    print(f"\n--- Building common libraries and applications in '{srv_com_dir}' ---")

    for item in os.listdir(srv_com_dir):
        item_path = os.path.join(srv_com_dir, item)
        if os.path.isdir(item_path):
            # Check if it's a CMake project by looking for CMakeLists.txt
            if os.path.exists(os.path.join(item_path, "CMakeLists.txt")):
                print(f"\nProcessing common project: {item}")
                configure_build(item, item_path, build_type, {})
                build_project(item, item_path)
            else:
                print(f"Skipping '{item}' in 'srv/': No CMakeLists.txt found.")

def main():
    """
    Main function to parse command-line arguments and orchestrate the build and test process.
    """
    available_apps = get_available_apps()

    # If no apps are found, and the user hasn't specified one, we can still build common
    if not available_apps:
        print("Warning: No applications found in the 'apps' directory.")

    parser = argparse.ArgumentParser(description="Setup and build helper for Automotive Applications")
    # Make --app argument not strictly required if no apps are found, to allow building common only
    parser.add_argument("--app", choices=available_apps,
                        required=bool(available_apps), # required only if apps are available
                        help="Application to build/test")
    parser.add_argument("--configure", action="store_true", help="Configure the build")
    parser.add_argument("--build", action="store_true", help="Build the project")
    parser.add_argument("--test", action="store_true", help="Run tests")
    parser.add_argument("--build-type", choices=["Debug", "Release", "RelWithDebInfo"], default="Debug", help="Build type")

    args = parser.parse_args()

    options = {
    }

    # Step 1: Build common libraries and applications automatically
    print("\nStarting automatic build of common libraries and applications...")
    build_common_libs_and_apps(args.build_type)
    print("Finished building common libraries and applications.")

    # Step 2: Handle the explicitly selected application (if any)
    if args.app:
        print(f"\nProcessing selected application: {args.app}")
        app_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "apps", args.app)

        # Determine if configuration is needed for the selected app:
        # If --configure flag is explicitly set.
        # If --build is set and the CMakeCache.txt file (indicating a configured build) is missing.
        if args.configure or (args.build and not os.path.exists(os.path.join(app_path, "build", "CMakeCache.txt"))):
            configure_build(args.app, app_path, args.build_type, options)

        # Perform the build if requested
        if args.build:
            build_project(args.app, app_path)

        # Run tests if requested
        if args.test:
            run_tests(args.app, app_path)
    elif not available_apps:
        print("\nNo application was specified, and no 'apps' directory found to choose from. Only common libraries/apps were processed.")
    else:
        print("\nNo application was specified. Only common libraries/apps were processed. Use --app to select an application.")

if __name__ == "__main__":
    main()