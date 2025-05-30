#!/usr/bin/env python3
import subprocess

# Path to the executable
EXEC = "./pclock"

# Define test cases: (description, args_list, expected_stderr, expected_exit_code)
tests = [
    # Usage tests (bad input format)
    ("no args", [], "Usage: ./pclock <number to guess>\n", 1),
    ("too many args", ["123", "456"], "Usage: ./pclock <number to guess>\n", 1),
    ("non-numeric", ["foo"], "Usage: ./pclock <number to guess>\n", 1),
    ("mixed alpha-numeric", ["42abc"], "Usage: ./pclock <number to guess>\n", 1),
    ("decimal", ["5.5"], "Usage: ./pclock <number to guess>\n", 1),
    ("scientific notation", ["1e3"], "Usage: ./pclock <number to guess>\n", 1),
    ("hex-like", ["0x10"], "Usage: ./pclock <number to guess>\n", 1),
    ("empty string", [""], "Usage: ./pclock <number to guess>\n", 1),
    ("whitespace only", ["   "], "Usage: ./pclock <number to guess>\n", 1),
    ("leading zeros with letters", ["007a"], "Usage: ./pclock <number to guess>\n", 1),
    ("plus minus only", ["+-"], "Usage: ./pclock <number to guess>\n", 1),
    ("dash dash", ["--"], "Usage: ./pclock <number to guess>\n", 1),

    # Range-error tests
    ("zero", ["0"], "Error: number must be between 1 and 1000\n", 1),
    ("negative", ["-5"], "Error: number must be between 1 and 1000\n", 1),
    ("plus zero", ["+0"], "Error: number must be between 1 and 1000\n", 1),
    ("above max", ["1001"], "Error: number must be between 1 and 1000\n", 1),
    ("very large", ["99999"], "Error: number must be between 1 and 1000\n", 1),
    ("all zeros", ["0000"], "Error: number must be between 1 and 1000\n", 1),

    # Valid runs
    ("valid: 1", ["1"], "", 0),
    ("valid: 1000", ["1000"], "", 0),
    ("valid: 500", ["500"], "", 0),
    ("valid: +42", ["+42"], "", 0),
    ("valid: 42", ["42"], "", 0),
    ("valid: 999", ["999"], "", 0),
]

def run_test(idx, desc, args, exp_err, exp_code):
    try:
        result = subprocess.run([EXEC] + args, capture_output=True, text=True)
        actual_err = result.stderr
        actual_code = result.returncode
        passed = (actual_err == exp_err and actual_code == exp_code)
        status = "PASS" if passed else "FAIL"
        print(f"{idx+1:02d}. {desc:25} | args={args} | exit={actual_code} | "
              f"stderr={actual_err.strip()!r} | expected_exit={exp_code} => {status}")
    except FileNotFoundError:
        print(f"{idx+1:02d}. {desc:25} => ERROR: Executable '{EXEC}' not found!")

if __name__ == "__main__":
    print("Running pclock tests...\n")
    for i, (desc, args, exp_err, exp_code) in enumerate(tests):
        run_test(i, desc, args, exp_err, exp_code)
