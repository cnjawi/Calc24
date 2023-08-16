from cqltr import figure
import sqlite3

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


conn = sqlite3.connect('results.db')
cur = conn.cursor()
try:
    cur.execute("""CREATE TABLE results (一 INTEGER, 二 INTEGER, 三 INTEGER, 四 INTEGER, 数量 INTEGER, 结果 TEXT)""")
except sqlite3.OperationalError:
    print("results.db 中 results 已存在，请先删除文件")
    exit()

@timer
def CallByDll():
    for h in range(1, 14):
        for j in range(h, 14):
            for k in range(j, 14):
                for l in range(k, 14):
                    s = figure(h, j, k, l)
                    cur.execute("""INSERT INTO results VALUES (?, ?, ?, ?, ?, ?)""",
                                (h, j, k, l, int(s[-3:-1]), s[:-10]))
    conn.commit()

if __name__ == '__main__':
    CallByDll()
