Qualcomm QFPROM Fuse Read/Write Tester

Overview - 
This tool is designed to facilitate the reading and writing of QFPROM fuses on Qualcomm devices. It provides a command-line interface for interacting with the QFPROM hardware via IOCTL calls, supporting both single and multiple fuse operations. The utility is intended for engineers and developers working with Qualcomm platforms who need to validate, debug, or automate fuse access in a controlled and traceable manner.

Features -
Fuse Read: Retrieve the value of a specified fuse address and display its status.
Fuse Write: Program one or more fuse addresses with specified values.
Dry Run Mode: Validate command arguments without performing actual hardware operations.
Verbose Logging: Output detailed steps for debugging and traceability.
Quiet Mode: Suppress all output except the exit code, suitable for scripting.
Custom Logging: Write debug logs to a specified file for audit or review.
Version Information: Display tool version and build details for traceability.

Usage -
The tool is invoked from the command line with the following general syntax:
qfprom_test [options] fuseread <addr_type> <fuse_addr>
qfprom_test [options] fusewrite <addr> <lsb> <msb> [<addr> <lsb> <msb> ...]

Options -

--help : Show usage instructions and examples.
--dry-run : Validate arguments without performing IOCTL operations.
--verbose : Display detailed execution steps.
--quiet : Suppress all output except the exit code.
--log <file> : Write debug logs to the specified file.
--version : Show tool version and build information.

Examples -

1. --help
Shows usage instructions and examples.
./qfprom_test --help

2. --dry-run
Validates arguments without performing any actual IOCTL operations (no hardware changes).
./qfprom_test --dry-run fuseread 1 0x1234
./qfprom_test --dry-run fusewrite 0x1234 0xCD 0xAB

3. --verbose
Displays detailed execution steps for debugging and traceability.
./qfprom_test --verbose fuseread 1 0x1234
./qfprom_test --verbose fusewrite 0x1234 0xCD 0xAB

4. --quiet
Suppresses all output except the exit code (useful for scripting).
./qfprom_test --quiet fuseread 1 0x1234
./qfprom_test --quiet fusewrite 0x1234 0xCD 0xAB

5. --log 
Writes debug logs to the specified file.
./qfprom_test --log debug.txt fuseread 1 0x1234
./qfprom_test --log debug.txt fusewrite 0x1234 0xCD 0xAB

6. --version
Shows tool version and build information.
./qfprom_test --version


Combined Options Example:
You can combine multiple options as needed:
./qfprom_test --verbose --log debug.txt --dry-run fusewrite 0x1234 0xCD 0xAB

This command will validate the fuse write operation, show detailed steps, and write logs to debug.txt, but will not perform any actual hardware changes.

Device Requirements -
The tool interacts with the QFPROM device node, typically located at /dev/tmecom.
Requires appropriate permissions to access device nodes and perform IOCTL operations.

Error Handling -
The tool provides clear error messages for invalid input, device access issues, and IOCTL failures.
Exit codes are used to indicate success or specific error conditions, facilitating integration into automated workflows.

Logging and Output -
Output is color-coded for readability (success and error messages).
Logging can be directed to a file for persistent records.
Quiet mode is available for minimal output, ideal for use in scripts or automated systems.

Versioning -
The tool displays its version and build timestamp for traceability and support.