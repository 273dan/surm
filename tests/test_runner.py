import os
import sys
from pathlib import Path
import shutil
import subprocess
from typing import Tuple
import difflib

global verbose
verbose = False
def printv(message: str) -> None:
    if verbose:
        print(message)

build_dir = os.path.abspath("./build")
run_env = os.environ.copy()
run_env["PATH"] = f"{build_dir}{os.pathsep}{run_env["PATH"]}"
    
def main():
    global verbose
    all_passed = True
    if len(sys.argv) > 1 and sys.argv[1] == "-v":
        verbose = True
    curr_path = Path(os.getcwd())
    tests_dir = curr_path / "tests"
    tests_source_dir = tests_dir / "tests_source"
    if not(tests_dir.exists() and tests_source_dir.exists()):
        printv("invalid directory structure")
        exit(1)
    stage_dir = tests_dir / "stage"
    if(stage_dir.exists()):
        printv(f"cleaning stage {stage_dir}")
        shutil.rmtree(stage_dir)

    printv(f"creating stage folder {stage_dir}")
    stage_dir.mkdir()

    printv("")
    printv(f"-- directory structure initialised --")
    printv(f"tests dir: {tests_dir}")
    printv(f"test source dir: {tests_source_dir}")
    printv(f"stage dir: {stage_dir}")

    printv("")
    printv("discovering tests...")
    tests = list(tests_source_dir.iterdir())
    printv(f"-- discovered {len(tests)} tests --")
    for test in tests:
        (has_commands, has_output) = validate_test_source(test)
        if(not(has_commands and has_output)):
            print(f"test {test} missing .test_output or .test_command")
            exit(1)
        printv(f"- {test}")

    printv("running tests")
    for test in tests:
        result = run_test(test, stage_dir)
        if not result:
            all_passed = False
    printv("all tests finished, cleaning stage dir")
    shutil.rmtree(stage_dir)
    if all_passed:
        exit(0)
    exit(1)

def validate_test_source(source_dir: Path) -> Tuple[bool, bool]:
    has_commands = (source_dir / ".test_command").exists()
    has_output = (source_dir / ".test_output").exists()
    return (has_commands, has_output)

def get_test_name(test: Path) -> str:
    return test.parts[-1].replace("_", " ")

def run_test(test: Path, stage_dir: Path) -> bool:
    test_name = get_test_name(test)
    stage_test_dir = stage_dir / test.parts[-1]
    printv(f"copy {test} -> {stage_test_dir}")
    shutil.copytree(test, stage_test_dir)

    test_command_path = stage_test_dir / ".test_command"
    with open(test_command_path) as test_command:
        test_command = test_command.readline().strip()
    printv(f"test command: {test_command}")

    test_output_path = stage_test_dir / ".test_output"
    with open(test_output_path) as test_output:
        expected_output_full = test_output.read()
    expected_output_lines = list(l.strip('\n') for l in expected_output_full.splitlines())
    printv("expected test output:")
    printv(expected_output_full)

    printv("starting test")
    result = subprocess.run(test_command, capture_output=True, text=True, cwd=stage_test_dir, env=run_env, shell=True)
    actual_output_full = result.stdout
    actual_output_lines = list(l.strip('\n') for l in actual_output_full.splitlines())
    printv("test finished, cleaning stage")
    shutil.rmtree(stage_test_dir)

    if actual_output_full != expected_output_full:
        print(f"{test_name}: \033[31mfail\033[0m")
   
        diff = difflib.unified_diff(
            expected_output_full.splitlines(keepends=True),
            actual_output_full.splitlines(keepends=True),
            fromfile='expected',
            tofile='actual'
        )
   
        for line in diff:
            if line.startswith('+'):
                print(f"\033[32m{line.rstrip()}\033[0m") # Green
            elif line.startswith('-'):
                print(f"\033[31m{line.rstrip()}\033[0m") # Red
            else:
                print(line.rstrip())
        if result.stderr:
            print("stderr")
            print(result.stderr)
        return False
    print(f"{test_name}: \033[32mpass\033[0m")
    return True


if __name__ == "__main__":
    main()

