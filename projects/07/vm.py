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

def gen(in_stream, out_stream):
    for line in in_stream.readlines():
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
            out_stream.write("M=D+M\n")
            out_stream.write("@SP\n")
            out_stream.write("M=M-1")
            continue
        elif words[0] == 'sub':
            # @SP
            # A=M-1
            # D=M
            # A=A-1
            # M=D-M
            # @SP
            # M=M=1
            pass
        elif words[0] == 'gt':
            pass
        elif words[0] == 'lt':
            pass
            # @SP
            # A=M-1
            # D=M
            # A=A-1
            # if D and M
        elif words[0] == 'eq':
            # Decrement stack pointer
            out_stream.write("@SP\n")
            out_stream.write("M=M-1\n")
            out_stream.write("A=M\n")
            out_stream.write("D=M\n") # set value top of stack
            out_stream.write("A=A-1\n")
            out_stream.write("D=D-M\n")
            out_stream.write("@BRANCH_TRUE\n")
            out_stream.write("D;JEQ\n")
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=0\n")
            out_stream.write("@END_BRANCH\n")
            out_stream.write("0;JMP\n")
            out_stream.write("(BRANCH_TRUE)\n")
            out_stream.write("@SP\n")
            out_stream.write("A=M-1\n")
            out_stream.write("M=-1\n")
            out_stream.write("(END_BRANCH)\n")
            continue
        elif words[0] == 'neg':
            # @SP
            # A=M-1
            # M=-M
            pass
        elif words[0] == 'and':
            # @SP
            # A=M-1
            # D=M
            # A=A-1
            # M=D&M
            # @SP
            # M=M=1
            pass
        elif words[0] == 'or':
            # @SP
            # A=M-1
            # D=M
            # A=A-1
            # M=D|M
            # @SP
            # M=M=1
            pass
        elif words[0] == 'not':
            # @SP
            # A=M-1
            # M=!M
            pass
# add x + y integer
# sub x - y integer
# neg -y integer
# eq x==0 boolean
# gt x > y boolean
# lt x < y boolean
# and x and y boolean (bitwise)
# or x or y boolean (bitwise)
# not not x boolean (bitwise)


def main():
    parser = argparse.ArgumentParser(description="Generate HACK assembly")
    parser.add_argument('file', metavar='f', type=str)
    args = parser.parse_args()

    in_filename = args.file
    out_filename = '.'.join(args.file.split('.')[:-1]) + '.asm'
    in_file = open(args.file, 'r')
    out_file = open(out_filename, 'w')

    gen(in_file, out_file)

if __name__ == "__main__":
    main()
