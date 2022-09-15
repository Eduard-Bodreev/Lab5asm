import subprocess as sb
import sys

benchmarks = [
        ("4k.png", 100),
        ("4k.png", 200),
        ("4k.png", 500),
        ("4k.png", 1000),

        ("1080.png", 100),
        ("1080.png", 200),
        ("1080.png", 500),
        ("1080.png", 1000),
        ]

files = [
        "./prog5_o0",
        "./prog5_o1",
        "./prog5_o2",
        "./prog5_o3",
        "./prog5_ofast",
        "./prog5_asm",
        ]

def average(a):
    return sum(a) / len(a)


table = [["Test", *files]]
for img, runs in benchmarks:
    for color in ["r", "g", "b"]:
        res = []

        fcontent = None
        for elfname in files:
            print(' ' * 64, end='\r')
            print(img, runs, color, elfname, end='\r', file=sys.stderr)
            ifname = "in/{}".format(img)
            ofname = "out/{}.{}.{}".format(elfname, color, img)
            out_fmt = sb.check_output([elfname, ifname, ofname, color, str(runs)]).decode()

            assert out_fmt.startswith("Time elapsed:")

            if fcontent:
                with open(ofname, "rb") as f:
                    ofcontent = f.read()
                    assert ofcontent
                    assert ofcontent == fcontent

            t = float(out_fmt.split(":")[1])
            res.append(t)
        table.append(["{}.{}".format(color, img), *res])
    table.append(["{}.{}".format("avg", img), *[average([l[i + 1] for l in table[-3:]]) for i in range(len(files))]])

print()
s_fmt = ' '.join(
        "{{:<{}}}".format(max(len(str(l[i])) for l in table)) for i in range(len(files) + 1)
        )

print(s_fmt)
for l in table:
    print(s_fmt.format(*l))
