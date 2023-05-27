from cqltr import figure
from timer import timer
import sqlite3

@timer
def CallByDll():
    conn = sqlite3.connect('results.db')
    cur = conn.cursor()
    try:
        cur.execute("""CREATE TABLE results (输入 TEXT, T1 TEXT, tota1 TEXT, T2 TEXT, tota2 TEXT)""")
    except sqlite3.OperationalError:
        print("results.db 中 results 已存在，请先删除文件")
        exit()
    for h in range(1, 14):
        for j in range(h, 14):
            for k in range(j, 14):
                for l in range(k, 14):
                    s = figure(h, j, k, l)
                    cur.execute("""INSERT INTO results VALUES (?, ?, ?, ?, ?)""",
                                (f'{h} {j} {k} {l}', s[:2], s[3:5], s[6:8], s[9:11]))
    conn.commit()

if __name__ == '__main__':
    CallByDll()
