from time import perf_counter
from typing import Callable

def timer(func: Callable) -> Callable:
    def clocked(*args, **kargs):
        st = perf_counter()
        res = func(*args, **kargs)
        ed = perf_counter()
        param = ', '.join(map(str, args))
        for key in kargs:
            param += f', {key}={kargs[key]}'
        print(f'[{ed-st:.5e}] {func.__name__}({param})')
        return res
    clocked.__name__ = func.__name__
    return clocked

if __name__ == '__main__':
    @timer
    def fac(n: int) -> int:
        return 1 if n < 2 else n * fac(n-1)
    
    @timer
    def justatest(*args, **kargs):
        pass
    
    fac(5)
    justatest(1, 2, a='s', b=(3, 4), c=print)
    