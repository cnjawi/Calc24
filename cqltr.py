from ctypes import CDLL, c_int, c_char_p, c_bool

loc = 'build/windows/x64/debug/cqltr.dll'  # 手动更改
cqltr = CDLL(loc)
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

if __name__ == '__main__':
    print(figure(3, 3, 8, 8))