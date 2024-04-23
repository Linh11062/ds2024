def longest_path_in_file(file_path):
    longest_path = ""
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()
            if len(line) > len(longest_path):
                longest_path = line
    return longest_path

if __name__ == '__main__':
    file_path = 'test1.txt'  
    longest_path = longest_path_in_file(file_path)
    print(f"The longest path in {file_path} is: {longest_path}")
