import serial.tools.list_ports as list_ports
from pathlib import Path
import threading
import argparse
import serial
import time
import os

running = True
write_done = False
verify_done = False
verify_errors = False


def read_thread():
    global write_done
    global verify_done
    global verify_errors
    while running:
        if ser.inWaiting() > 0:
            data_str = ser.read(ser.inWaiting()).decode('ascii')
            print(f"[R]{data_str}", end='')

            # Check if programming is complete
            if 'Wrote program' in data_str:
                write_done = True
            # Check if there were verification errors
            if 'program error:' in data_str:
                verify_errors = True
            # Check if verify is complete
            if 'program verified: correct' in data_str:
                verify_done = True
        else:
            if verify_errors:
                verify_done = True
        time.sleep(0.01)


def serial_write(data):
    print(f"[W]{data.decode()}")
    ser.write(data)


def check_file(firmware_file):
    # Check if firmware file exists
    if not firmware_file.exists():
        print("The file doesn't exist")
        raise SystemExit(1)

    # Check if firmware file is a file
    if not os.path.isfile(firmware_file):
        print("The file is not a file")
        raise SystemExit(1)

    # Check if firmware file is a hex file
    if not firmware_file.suffix == '.hex':
        print("The file is not a .hex file")
        raise SystemExit(1)


def print_logo():
    print("""
        ___  _________________   ____  __          
       /   |/_  __/_  __/  _/ | / /\ \/ /          
      / /| | / /   / /  / //  |/ /  \  /           
     / ___ |/ /   / / _/ // /|  /   / /            
    /_/  |_/_/   /_/ /___/_/_|_/___/_/___  ______  
                        / __ \/ __ \/ __ \/ ____/  
                       / /_/ / /_/ / / / / / __    
                      / ____/ _, _/ /_/ / /_/ /    
                     /_/   /_/ |_|\____/\____/ V0.1
                          By Felix Reee            
                                                   
""")


def print_wiring():
    print('Connect arduino pro micro to attiny10 as follows:')
    print(""" 
       Arduino                 ATtiny10               
      ----------+          +----------------          
      (SS#)  10 |--[R]-----| 6 (RESET#/PB3)          
                |          |                              
      (MOSI) 16 |--[R]--+--| 1 (TPIDATA/PB0)         
                |       |  |                         
      (MISO) 14 |--[R]--+  |                         
                |          |                              
      (SCK)  15 |--[R]-----| 3 (TPICLK/PB1)          
      ----------+          +----------------       

       -[R]-  =  a few kOhm resistor                 
    """)
    print('Press ENTER to continue...')
    input()


def print_com_ports():
    # List available com ports
    ports = list(list_ports.comports())
    print('')
    print('Available COM ports:')
    for i, port in enumerate(ports):
        print(f' [{i}] {port}')

    # If there are multiple com ports let the user chose one
    if len(ports) > 1:
        print('')
        print(f'Enter number from 0 to {len(ports) - 1}')
        in_str = input()
        com_port = str(ports[int(in_str)])[:4]
    else:
        com_port = str(ports[0])[:4]

    return com_port


def open_serial(com_port):
    try:
        ser = serial.Serial(com_port, baudrate=115200)
        print(f"Opened {ser.portstr}")
    except Exception as e:
        print(e)
        if 'PermissionError' in str(e):
            print(f'{com_port} might be used by another program')
        raise SystemExit(1)

    return ser


def read_firmware(firmware_file):
    with open(firmware_file, 'rb') as f:
        hex_data = f.read()

    # Split data into individual lines (sending it all at once freezes serial connection)
    hex_data = hex_data.split(b'\r\n')
    hex_data.pop()

    # Calculate firmware size
    firmware_size = int((sum([len(x) for x in hex_data]) - len(hex_data) * 9 - (len(hex_data) - 1) * 2 - 2) / 2)
    print(f"Firmware size: {firmware_size}bytes")

    return hex_data


def send_firmware(hex_data):
    serial_write(b'R')
    time.sleep(0.05)
    for line in hex_data:
        serial_write(line)


def program_and_verify():
    global write_done
    global verify_done
    global verify_errors

    # Program
    write_done = False
    serial_write(b'P')
    while not write_done:
        time.sleep(0.01)

    # Verify
    verify_done = False
    verify_errors = False
    serial_write(b'V')
    while not verify_done:
        time.sleep(0.01)
    if verify_errors:
        print('VERIFY FOUND ERRORS!')


if __name__ == '__main__':
    # Initialize argument parser
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help='.hex firmware file')

    # Parse arguments
    args = parser.parse_args()
    firmware_file = Path(args.file)

    # Check if input file is valid
    check_file(firmware_file)

    # Print logo and wiring
    print_logo()
    print_wiring()

    # Display available com ports
    com_port = print_com_ports()

    # Open serial connection
    ser = open_serial(com_port)

    # Start serial reader thread
    reader = threading.Thread(target=read_thread)
    reader.daemon = True
    reader.start()

    # Read firmware file
    hex_data = read_firmware(firmware_file)

    # Send firmware data to arduino
    send_firmware(hex_data)

    time.sleep(0.1)
    print('')
    print('Press ENTER to program attiny / Q to quit')

    while True:
        # Read input
        in_str = input()

        # Q pressed (quit)
        if in_str.lower() == 'q':
            print('Stopping application...')
            break

        # Enter pressed (program device)
        if len(in_str) == 0:
            program_and_verify()

    # Stop reader thread
    running = False
    reader.join()

    # Close serial connection
    ser.close()
