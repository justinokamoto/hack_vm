import argparse

seg_map = {
    "local": "LCL",
    "argument": "ARG",
    "this": "THIS",
    "that": "THAT"
}

def initialize_mem_segs(out_stream):
    '''
    Initializes memory segment points. This is unnecessary when using provided
    test scripts
    '''
    out_stream.write("// initialize memory segment pointers\n")
    out_stream.write("@256\n")
    out_stream.write("D=A\n")
    out_stream.write("@SP\n")
    out_stream.write("M=D\n")

# BUG: Can't push negative constants
def gen(in_stream, out_stream):
    token=0 # This token is a hack to get unique branch names
    for line in in_stream.readlines():
        token += 1
        stripped = line.strip()
        words = stripped.split(' ')
        if len(words) == 0 or words[0] == '//' or words[0] == '':
            continue
        out_stream.write("// %s\n" % ' '.join(words))
        if words[0] == 'push':
            seg = words[1]
            val = words[2]
            if seg == "constant":
                out_stream.write("@%s\n" % val)
                out_stream.write("D=A\n")
                out_stream.write("@SP\n")
                out_stream.write("A=M\n")
                out_stream.write("M=D\n")
                out_stream.write("@SP\n")
                out_stream.write("M=M+1\n")
                continue
        elif words[0] == 'pop':
            pass
        elif words[0] == 'add':
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("D=M\n")
            out_stream.write("A=A-1\n")
            out_stream.write("M=M+D\n")
            out_stream.write("@SP\n")
            out_stream.write("M=M-1")
            continue
        elif words[0] == 'sub':
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("D=M\n")
            out_stream.write("A=A-1\n")
            out_stream.write("M=M-D\n")
            out_stream.write("@SP\n")
            out_stream.write("M=M-1")
            continue
        elif words[0] == 'lt':
            # Decrement stack pointer
            out_stream.write("@SP\n")
            out_stream.write("M=M-1\n")
            out_stream.write("A=M\n")
            out_stream.write("D=M\n") # set D to top of stack
            out_stream.write("A=A-1\n")
            out_stream.write("D=M-D\n")
            out_stream.write("@BRANCH_TRUE_%s\n" % token)
            out_stream.write("D;JLT\n")
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=0\n")
            out_stream.write("@END_BRANCH_%s\n" % token)
            out_stream.write("0;JMP\n")
            out_stream.write("(BRANCH_TRUE_%s)\n" % token)
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=-1\n")
            out_stream.write("(END_BRANCH_%s)\n" % token)
            continue
        elif words[0] == 'gt':
            # Decrement stack pointer
            out_stream.write("@SP\n")
            out_stream.write("M=M-1\n")
            out_stream.write("A=M\n")
            out_stream.write("D=M\n") # set D to top of stack
            out_stream.write("A=A-1\n")
            out_stream.write("D=M-D\n")
            out_stream.write("@BRANCH_TRUE_%s\n" % token)
            out_stream.write("D;JGT\n")
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=0\n")
            out_stream.write("@END_BRANCH_%s\n" % token)
            out_stream.write("0;JMP\n")
            out_stream.write("(BRANCH_TRUE_%s)\n" % token)
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=-1\n")
            out_stream.write("(END_BRANCH_%s)\n" % token)
            continue
        elif words[0] == 'eq':
            # Decrement stack pointer
            out_stream.write("@SP\n")
            out_stream.write("M=M-1\n")
            out_stream.write("A=M\n")
            out_stream.write("D=M\n") # set D to top of stack
            out_stream.write("A=A-1\n")
            out_stream.write("D=D-M\n")
            out_stream.write("@BRANCH_TRUE_%s\n" % token)
            out_stream.write("D;JEQ\n")
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=0\n")
            out_stream.write("@END_BRANCH_%s\n" % token)
            out_stream.write("0;JMP\n")
            out_stream.write("(BRANCH_TRUE_%s)\n" % token)
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=-1\n")
            out_stream.write("(END_BRANCH_%s)\n" % token)
            continue
        elif words[0] == 'and':
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("D=M\n")
            out_stream.write("A=A-1\n")
            out_stream.write("M=D&M\n")
            out_stream.write("@SP\n")
            out_stream.write("M=M-1\n")
            continue
        elif words[0] == 'or':
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("D=M\n")
            out_stream.write("A=A-1\n")
            out_stream.write("M=D|M\n")
            out_stream.write("@SP\n")
            out_stream.write("M=M-1\n")
            continue
        elif words[0] == 'neg':
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=-M\n")
            continue
        elif words[0] == 'not':
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=!M\n")
            continue

def main():
    parser = argparse.ArgumentParser(description="Generate HACK assembly")
    parser.add_argument('file', metavar='f', type=str)
    args = parser.parse_args()

    in_filename = args.file
    out_filename = '.'.join(args.file.split('.')[:-1]) + '.asm'
    in_file = open(args.file, 'r')
    out_file = open(out_filename, 'w')
    initialize_mem_segs(out_file)
    gen(in_file, out_file)

if __name__ == "__main__":
    main()
