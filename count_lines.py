# script to count the number of lines of code in a C project.

import os

def is_comment(line):
    line = line.strip()
    if len(line) > 1:
        if line[0] == '/' and line[1] in "/*":
            return True
    if len(line) > 0:
        if line[0] == '*':
            return True
    return False

def is_blank(line):
    if line.strip():
        return False
    return True

def count_file(file):
    code = 0
    comments = 0
    blanks = 0
    file = os.path.abspath(file)
    lines = open(file, "r")
    for line in lines:
        if is_blank(line):
            blanks += 1
        elif is_comment(line):
            comments += 1
        else:
            code +=1
    print(file)
    print("code:     " + str(code))
    print("comments: " + str(comments))
    print("blanks:   " + str(blanks) + '\n')
    return code, comments, blanks

def count_dir(dir):
    files = os.listdir(dir)
    code = 0
    comments = 0
    blanks = 0
    for file in files:
        cd, cm, bl = count_file(dir + '/' + file)
        code += cd
        comments += cm
        blanks += bl
    print(dir)
    print("code:     " + str(code))
    print("comments: " + str(comments))
    print("blanks:   " + str(blanks) + '\n')
    return code, comments, blanks

# list of directories (relative to root) to check
directories = ["stoff2d/src", "stoff2d/include", "examples/shooter/src"]

code = 0
comments = 0
blanks = 0
for dir in directories:
    cd, cm, bl = count_dir(dir)
    code += cd
    comments += cm
    blanks += bl
print("TOTAL")
print("code:     " + str(code))
print("comments: " + str(comments))
print("blanks:   " + str(blanks))

