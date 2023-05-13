from cqltr import figure
from timer import timer
import sqlite3


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
