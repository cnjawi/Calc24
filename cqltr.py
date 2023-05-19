from ctypes import CDLL, c_int, c_char_p, c_bool

locs: tuple[str] = ("build\\Release\\cqltr.dll", "C:\\CodeBase\\Calc24\\build\\libcqltr.dll")
found = False
for loc in locs:
    try:
        cqltr = CDLL(loc, winmode=0)
    except FileNotFoundError:
        continue
    found = True
    break
if not found:
    print(f'文件不存在')
    exit()
try:
    fi, va = cqltr.figure, cqltr.validate
except AttributeError:
    print(f'{loc} 不是所需的动态链接库')
    exit()

FourInt = c_int * 4
fi.argtypes, fi.restype = (FourInt, ), c_char_p
va.argtypes, va.restype = (FourInt, ), c_bool

def figure(a: int, b: int, c: int, d: int) -> str:
    return bytes(fi(FourInt(a, b, c, d))).decode()

def validate(a: int, b: int, c: int, d: int) -> bool:
    return bool(va(FourInt(a, b, c, d)))
