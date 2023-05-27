#include <string>
#include <sstream>

#include "cqltr.h"
#include "expr.h"

/*
  表达式  一元     二元     三元     四元
  变量    unary  binary  ternary   quad
  大小     4       36      432    648+2592
*/
expr unary[4], binary[36], ternary[432], quad1[648], quad2[2592];

//两个表达式有6种组合方法
void combine(expr* dst, int count, expr* src1, expr* src2) {
    dst[count + 0].set('+', src1, src2);
    dst[count + 1].set('*', src1, src2);
    dst[count + 2].set('-', src1, src2);
    dst[count + 3].set('/', src1, src2);
    dst[count + 4].set('-', src2, src1);
    dst[count + 5].set('/', src2, src1);
}

//单个源数组
void fill(expr dst[], expr src[], int size) {
    int i, j, cnt = 0;
    for (i = 0; i < size; i++) {
        for (j = i + 1; j < size; j++) {
            if (!(src[i].pos & src[j].pos)) {
                combine(dst, cnt, src + i, src + j);
                cnt += 6;
            }
        }
    }
}

//两个源数组
void fill(expr dst[], expr src1[], expr src2[], int size1, int size2) {
    int i, j, cnt = 0;
    for (i = 0; i < size1; i++) {
        for (j = 0; j < size2; j++) {
            if (!(src1[i].pos & src2[j].pos)) {
                combine(dst, cnt, src1 + i, src2 + j);
                cnt += 6;
            }
        }
    }
}

//过滤完全相同的结果，并返回
void output(expr src[], int size, int* cnt, int* t_cnt) {
    std::string tmp[78], s;
    int idx = 0;
    for (int i = 0; i < size; i++) {
        if (src[i].val > 23.99 && src[i].val < 24.01) {
            ++(*t_cnt);
            s = src[i].str();
            bool is_different = true;
            for (int j = 0; j < idx; j++) {
                if (s == tmp[j]) {
                    is_different = false;
                    break;
                }
            }
            if (is_different) {
                tmp[idx] = s;
                ++(*cnt);
                ++idx;
            }
        }
    }
}

const char* figure(int exrcs[4]) {
    for (int i = 0; i < 4; i++) {
        unary[i].val = (float)exrcs[i];
        unary[i].pos = 1 << i;
    }
    fill(binary, unary, 4);
    fill(ternary, unary, binary, 4, 36);
    fill(quad1, binary, 36);
    fill(quad2, unary, ternary, 4, 432);

    std::stringstream ss;
    int total1 = 0, total2 = 0, T1 = 0, T2 = 0;
    output(quad1, 648, &total1, &T1);
    output(quad2, 2592, &total2, &T2);

    ss << (T1/10==0 ? " " : "") << T1 << '/';
    ss << (total1/10==0 ? " ": "") << total1 << '\n';
    ss << (T2/10==0 ? " " : "") << T2 << '/';
    ss << (total2/10==0 ? " ": "") << total2 << '\n';

    const std::string* s = new std::string(ss.str());
    return s->c_str();
}

bool validate(int exrcs[4]) {
    for (int i = 0; i < 4; i++) {
        unary[i].val = (float)exrcs[i];
        unary[i].pos = 1 << i;
    }
    fill(binary, unary, 4);
    fill(ternary, unary, binary, 4, 36);
    fill(quad1, binary, 36);
    fill(quad2, unary, ternary, 4, 432);

    for (int i = 0; i < 648; i++) {
        if (quad1[i].val > 23.99 && quad1[i].val < 24.01) {
            return true;
        }
    }
    for (int i = 0; i < 2592; i++) {
        if (quad2[i].val > 23.99 && quad2[i].val < 24.01) {
            return true;
        }
    }
    return false;
}