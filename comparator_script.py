import os

# File paths
file_bin_path = 'files/alice_in_wonderland.txt'
file_groza_bin_path = 'files/alice_in_wonderland.groza.txt'



# Check if files exist
if not os.path.exists(file_bin_path) or not os.path.exists(file_groza_bin_path):
    print("One or both files do not exist.")
else:
    # Read the binary content of both files
    with open(file_bin_path, 'rb') as f_bin, open(file_groza_bin_path, 'rb') as f_groza_bin:
        file_bin_content = f_bin.read()
        file_groza_bin_content = f_groza_bin.read()

    # Compare the content of the two files
    files_are_identical = file_bin_content == file_groza_bin_content

    # Identify differences if the files are not identical
    if not files_are_identical:
        min_length = min(len(file_bin_content), len(file_groza_bin_content))
        differences = [(i, file_bin_content[i], file_groza_bin_content[i])
                       for i in range(min_length)
                       if file_bin_content[i] != file_groza_bin_content[i]]
    else:
        differences = []

    # Result summary
    print("Files are identical:", files_are_identical)
    print("Number of differences:", len(differences))
    if differences:
        print("First 10 differences (if any):", differences[:10])
