

# Read line numbers to include from num.txt
with open("C:/Users/rushi/Downloads/SSA-2_updated/re_formatIP.txt", "r") as num_file:
    included_lines = set(map(int, num_file.readlines()))

# Read lines from main.txt, including specified line numbers
with open("C:/Users/rushi/Downloads/SSA-2_updated/acl.rl", "r") as main_file, open("C:/Users/rushi/Downloads/SSA-2_updated/re_formatOP", "w") as new_file:
    for line_number, line in enumerate(main_file, start=1):
        if line_number in included_lines:
            new_file.write(line)
print('done')