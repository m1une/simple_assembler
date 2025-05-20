import re

# 入出力ファイル名
input_file = 'randommif.txt'
output_file = 'randomdata.c'

# 入力ファイルを読み込む
with open(input_file, 'r', encoding='utf-8') as f:
    data = f.read()

# 数値部分だけ抽出（: の後の数値）
numbers = re.findall(r':\s*(-?\d+);', data)
numbers = [int(num) for num in numbers]

# C配列の文字列を生成
c_array = "int data[] = {\n"
for i, num in enumerate(numbers):
    end = ',' if i < len(numbers) - 1 else ''
    c_array += f"    {num}{end}\n"
c_array += "};\n"

# 出力ファイルに書き込む
with open(output_file, 'w', encoding='utf-8') as f:
    f.write(c_array)

print(f"C配列を {output_file} に出力しました。")