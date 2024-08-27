#!/usr/bin/env python3

import os
import urllib.parse

# Function to generate ASCII art from text

text = "42"

def generate_ascii_art(text):
    ascii_art = {
        'A': ['  A  ', ' A A ', 'AAAAA', 'A   A', 'A   A'],
        'B': ['BBBB ', 'B   B', 'BBBB ', 'B   B', 'BBBB '],
        'C': [' CCC ', 'C    ', 'C    ', 'C    ', ' CCC '],
        'D': ['DDD  ', 'D  D ', 'D   D', 'D  D ', 'DDD  '],
        'E': ['EEEEE', 'E    ', 'EEE  ', 'E    ', 'EEEEE'],
        'F': ['FFFFF', 'F    ', 'FFF  ', 'F    ', 'F    '],
        'G': [' GGG ', 'G    ', 'G  GG', 'G   G', ' GGG '],
        'H': ['H   H', 'H   H', 'HHHHH', 'H   H', 'H   H'],
        'I': [' III ', '  I  ', '  I  ', '  I  ', ' III '],
        'J': ['JJJJJ', '    J', '    J', 'J   J', ' JJJ '],
        'K': ['K   K', 'K  K ', 'KKK  ', 'K  K ', 'K   K'],
        'L': ['L    ', 'L    ', 'L    ', 'L    ', 'LLLLL'],
        'M': ['M   M', 'MM MM', 'M M M', 'M   M', 'M   M'],
        'N': ['N   N', 'NN  N', 'N N N', 'N  NN', 'N   N'],
        'O': [' OOO ', 'O   O', 'O   O', 'O   O', ' OOO '],
        'P': ['PPPP ', 'P   P', 'PPPP ', 'P    ', 'P    '],
        'Q': [' QQQ ', 'Q   Q', 'Q   Q', 'Q  QQ', ' QQQQ'],
        'R': ['RRRR ', 'R   R', 'RRRR ', 'R  R ', 'R   R'],
        'S': [' SSS ', 'S    ', ' SSS ', '    S', 'SSSS '],
        'T': ['TTTTT', '  T  ', '  T  ', '  T  ', '  T  '],
        'U': ['U   U', 'U   U', 'U   U', 'U   U', ' UUU '],
        'V': ['V   V', 'V   V', 'V   V', ' V V ', '  V  '],
        'W': ['W   W', 'W   W', 'W W W', 'W W W', ' W W '],
        'X': ['X   X', ' X X ', '  X  ', ' X X ', 'X   X'],
        'Y': ['Y   Y', ' Y Y ', '  Y  ', '  Y  ', '  Y  '],
        'Z': ['ZZZZZ', '   Z ', '  Z  ', ' Z   ', 'ZZZZZ'],
        '0': [' 000 ', '0   0', '0   0', '0   0', ' 000 '],
        '1': ['  1  ', ' 11  ', '  1  ', '  1  ', '11111'],
        '2': [' 222 ', '2   2', '   2 ', '  2  ', '22222'],
        '3': [' 333 ', '3   3', '   33', '3   3', ' 333 '],
        '4': ['   4 ', '  44 ', ' 4 4 ', '44444', '   4 '],
        '5': ['55555', '5    ', '5555 ', '    5', '5555 '],
        '6': [' 666 ', '6    ', '6666 ', '6   6', ' 666 '],
        '7': ['77777', '    7', '   7 ', '  7  ', ' 7   '],
        '8': [' 888 ', '8   8', ' 888 ', '8   8', ' 888 '],
        '9': [' 999 ', '9   9', ' 9999', '    9', ' 999 '],
        ' ': ['     ', '     ', '     ', '     ', '     ']
    }

    lines = [''] * 5
    for char in text.upper():
        if char in ascii_art:
            for i in range(5):
                lines[i] += ascii_art[char][i] + '  '
    
    return "\n".join(lines)

# Read the QUERY_STRING environment variable
query_string = os.environ.get('QUERY_STRING', '')

# Manually parse the query string
params = urllib.parse.parse_qs(query_string)
input_text = params.get('text', [''])[0]

# Generate ASCII art from the input text
ascii_art = generate_ascii_art(input_text)

# Generate the HTTP response
print("Content-Type: text/html\n")
print("<html>")
print("<head><title>ASCII Art Generator</title></head>")
print("<body>")
print("<h2>ASCII Art</h2>")
print(f"<pre>{ascii_art}</pre>")
print("</body>")
print("</html>")
