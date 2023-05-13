from ctypes import CDLL, c_int, c_char_p, c_bool

loc = "x64\\Release\\cqltr.dll"

try:
    cqltr = CDLL(loc)
    fi, va = cqltr.figure, cqltr.validate
except FileNotFoundError:
    print(f'{loc} 文件不存在')
    exit()
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

def get_help():
    print("""py ./cqltr.py option [args]
options:
  help  ---     打印说明页
  calc  ---     计算24点        [args] : 四个[1,13]间的整数
  eval  ---     四则运算求值    [args] : 中缀表达式(一定要用引号括起来)
  play  ---     24点游戏""")

if __name__ == '__main__':
    from sys import argv
    from random import randint

    def matched(exp: str, exrcs: list[int]) -> bool:
        res = []
        tmp = 0
        for i in exp:
            if i.isdigit():
                tmp = tmp * 10 + int(i)
                afterDigit = True
            elif afterDigit:
                afterDigit = False
                res.append(tmp)
                tmp = 0
        if tmp != 0:
            res.append(tmp)
        if exrcs == sorted(res):
            return True
        return False

    if len(argv) == 1:
        get_help()
        exit()

    option = argv[1]
    if option == 'calc':
        if len(argv) > 6:
            print('Too Many Arguments')
        elif len(argv) < 6:
            print('Too Few Arguments')
        else:
            print(figure(*map(int, argv[2:])))
    elif option == 'eval':
        print(eval(argv[2]))
    elif option == 'play':
        exrcs = [randint(1, 13) for _ in range(4)]
        while not validate(*exrcs):
            exrcs = [randint(1, 13) for _ in range(4)]
        exrcs.sort()
        print("可用符号: + - * / ( ), 不能输入空格\n输入 exit 退出, ans 获取答案\n\n{} {} {} {}".format(*exrcs))
        while True:
            print('\n>>> ', end='')
            exp = input()
            if exp == 'exit':
                print()
                break
            elif exp == 'ans':
                print(figure(*exrcs))
                continue
            if not all(x in '1234567890+-*/()' for x in exp):
                print('illegal character found')
                continue
            r = eval(exp)
            if 23.99 < r < 24.01:
                if matched(exp, exrcs):
                    print('Correct')
                else:
                    print("Wrong, numbers do not match")
            else:
                print(f'Wrong, result is {r}')
    else:
        get_help()

