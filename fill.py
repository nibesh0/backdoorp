def resize_exe_to_300mb(file_path):
    target_size = 500 * 1024 * 1024
    with open(file_path, 'rb') as exe_file:
        content = exe_file.read()

    current_size = len(content)
    if current_size >= target_size:
        print(f" {current_size / (1024 * 1024):.2f} larger.")
        return
    extra_bytes_needed = target_size - current_size
    extra_bytes = bytes([0x00]) * extra_bytes_needed 
    modified_content = content + extra_bytes
    with open('ren' + file_path, 'wb') as modified_exe_file:
        modified_exe_file.write(modified_content)

    print(f" 'ren_{file_path}'")


exe_file_path = 'server.exe'  
resize_exe_to_300mb(exe_file_path)
