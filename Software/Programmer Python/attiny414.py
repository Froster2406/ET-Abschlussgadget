from pymcuprog.toolconnection import ToolSerialConnection
from pymcuprog.backend import SessionConfig, Backend
import serial.tools.list_ports as list_ports
from pathlib import Path
import argparse
import logging
import sys
import os


class ArgParser(argparse.ArgumentParser):
    def error(self, message):
        self.print_help()
        print_com_ports()
        sys.exit(1)


def print_com_ports():
    # List available com ports
    ports = list(list_ports.comports())
    print('')
    print('Available COM ports:')
    for i, port in enumerate(ports):
        print(f'{port}')
    print('')


def check_file(file):
    # Check if firmware file exists
    if not file.exists():
        print("The file doesn't exist")
        raise SystemExit(1)

    # Check if firmware file is a file
    if not os.path.isfile(file):
        print("The file is not a file")
        raise SystemExit(1)

    # Check if firmware file is a hex file
    if not file.suffix == '.hex':
        print("The file is not a .hex file")
        raise SystemExit(1)


class LogHandler(logging.Handler):
    # Parse logger output and display status
    def emit(self, record):
        if 'Reading device ID...' in record.msg:
            print('Reading device id...')
        if 'Erase...' in record.msg:
            print('Erasing...')
        if 'Write...' in record.msg:
            print('Writing...')
        if ' bytes from flash...' in record.msg:
            print('Verifying...')


if __name__ == '__main__':
    # Initialize logger with callback
    logging.basicConfig(format="%(levelname)s: %(message)s", level=logging.INFO)
    logger = logging.getLogger()
    logger.propagate = False
    logger.handlers.clear()
    logger.addHandler(LogHandler())

    # Initialize argument parser
    parser = ArgParser()
    parser.add_argument("port", help='the com port of the programmer')
    parser.add_argument("file", help='.hex firmware file')
    parser.add_argument("--baud", help='programmer baudrate', type=int, default=115200)

    # Parse arguments
    args = parser.parse_args()
    com_port = args.port
    firmware_file = Path(args.file)
    baudrate = args.baud

    # Check if input file is valid
    check_file(firmware_file)

    # Set up programmer backend
    sessionconfig = SessionConfig("attiny414")
    transport = ToolSerialConnection(serialport=com_port, baudrate=baudrate, timeout=0.05)
    backend = Backend()
    backend.connect_to_tool(transport)
    
    
    while True:
        print("Press ENTER to program device...")
        input()
    
        try:
            # Connect to mcu
            backend.start_session(sessionconfig)
        except Exception as e:
            print(e)
            if 'FileNotFoundError' in str(e):
                sys.stderr.write(f"Serial port {com_port} does not exist")
                print_com_ports()
                sys.exit(1)
            if 'PermissionError' in str(e):
                sys.stderr.write(f"Serial port {com_port} might be used by another application")
                sys.exit(1)
        else:
            # Print device id
            device_id = backend.read_device_id()
            print("Device ID is {0:06X}".format(int.from_bytes(device_id, byteorder="little")))

            # Erase memory
            backend.erase()

            # Write firmware
            backend.write_hex_to_target(str(firmware_file))

            # Verify firmware
            if backend.verify_hex(str(firmware_file)):
                print("Programming was successful!")
            else:
                sys.stderr.write("Verification found errors!")
