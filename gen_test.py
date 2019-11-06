#!python

from scipy import sparse


def print_one_sm(file, a, msg):
    for col, row, value in zip(a.col, a.row, a.data):
        file.write("({0}, {1}) {2}\n".format(row, col, value))
    file.write(msg)


def generate_one(p, q,  r, name_pref, den=0.1):
    a = sparse.random(p, q, den)
    b = sparse.random(q, r, den)
    c = sparse.random(p, q, den)

    prod = a * b
    sum = a + c

    with open('{}.txt'.format(name_pref), 'w') as out_file:
        out_file.write( "{}, {}, {}\n".format(p, q, r))
        print_one_sm(out_file, a, "^=a\n",)
        print_one_sm(out_file, b, "^=b\n",)
        print_one_sm(out_file, c, "^=c\n",)
        print_one_sm(out_file, sum.tocoo(), "^=a+c\n",)
        print_one_sm(out_file, prod.tocoo(), "^=a*b\n",)

generate_one(10,10,10,'1',.2)
