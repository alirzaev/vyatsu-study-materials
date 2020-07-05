# !/usr/bin/env python3

from re import match
from re import search

LABEL = r'([A-Za-z][A-Za-z0-9_]+):'
SPACE = r'([\s\t]*)'
TMP_VAR = 'r7'
MACRO_NOT = '{space}nand {0} {0} {1}'

MACRO_AND = '{space}nand {0} {1} {2}\n' +\
            '{space}nand {2} {2} {2}'

MACRO_OR = ('{space}nand {0} {0} TMP\n' +\
            '{space}nand {1} {1} {2}\n' +\
            '{space}nand TMP {2} {2}').replace('TMP', TMP_VAR)

MACRO_XOR = ('{space}nand {0} {0} TMP\n' +\
             '{space}nand TMP {1} TMP\n' +\
             '{space}nand {1} {1} {2}\n' +\
             '{space}nand {0} {2} {2}\n' +\
             '{space}nand TMP {2} {2}').replace('TMP', TMP_VAR)

MACRO_ROL = ('{space}sub 8 {1} TMP\n' +\
             '{space}ror {0} TMP {2}').replace('TMP', TMP_VAR)

MACRO_JO = ('{space}nand {0} {0} TMP\n' +\
            '{space}jz TMP {1}').replace('TMP', TMP_VAR)

MACRO_ADD = ('{space}nand {1} {1} TMP\n' +\
             '{space}sub TMP 0xFF TMP\n' +\
             '{space}sub {0} TMP {2}').replace('TMP', TMP_VAR)

MACRO_IADD = ('{space}nand {1} {1} r7\n' +\
              '{space}sub r7 0xFF r7\n' +\
              '{space}sub {0} r7 {3}\n' +\
              '{space}nand {2} {2} r7\n' +\
              '{space}sub r7 0xFF r7\n' +\
              '{space}sub {3} r7 {3}\n' +\
              '{space}nand {3} {3} TMP\n' +\
              '{space}nand {0} {0} r7\n' +\
              '{space}nand {1} {1} {4}\n' +\
              '{space}nand r7 {4} {4}\n' +\
              '{space}nand {4} TMP TMP\n' +\
              '{space}nand TMP TMP TMP\n' +\
              '{space}nand {0} {1} {4}\n' +\
              '{space}nand {4} {4} {4}\n' +\
              '{space}nand {4} {4} r7\n' +\
              '{space}nand TMP TMP {4}\n' +\
              '{space}nand r7 {4} {4}\n' +\
              '{space}nand {4} 0x80 {4}\n' +\
              '{space}nand {4} {4} {4}\n' +\
              '{space}ror {4} 7 {4}').replace('TMP', '[255]')

macroses = { 'not': MACRO_NOT, 
             'and': MACRO_AND,
             'or': MACRO_OR,
             'rol': MACRO_ROL,
             'jo': MACRO_JO,
             'add': MACRO_ADD,
             'iadd': MACRO_IADD }

def process_line(line):
    s = line
    m0 = match(SPACE, s)  # найти лишние пробелы в начале строки
    s = line[len(m0.group(1)):]  # убрать пробелы в начале строки
    tabs = line[0:len(m0.group(1))]  # сохранить пробелы
    
    comment = ''
    t = s
    if s.find(';') != -1:
        comment = s[s.index(';'):]  # сохранить комментарий
        t = s[:s.index(';')]  # убрать комментарий
    
    tokens = t.split()
    if len(tokens) > 0 and tokens[0] in macroses:
        expanded = macroses[tokens[0]].format(*tokens[1:], space=tabs)
        return '{space};{0}\n{1}'.format(s, expanded, space=tabs)
    else:
        return line


while True:
    try:
        print(process_line(input()))
    except EOFError:
        break
