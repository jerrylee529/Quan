# coding=gb2312
'''
this unit is to convert a text file to html
'''

__author__ = 'Jerry Lee'


import sys
import csv

# column_name_list: 所有列的名称列表
# column_index_list: 需要显示的列索引
def csv2html(src_filename, dst_filename, title, column_name_list):
    # 获取表格部分
    table = print_table(src_filename, column_name_list)

    # 写入文件
    f = open(dst_filename, 'w')
    message = """
    <html>
    <title>{}</title>
    <head></head>
    <body>
    {}
    </body>
    </html>""".format(title, table)

    # write to file
    f.write(message)

    # close file
    f.close()

def print_table(filename, column_name_list):
    maxwidth = 100

    table = ""

    # start
    table += "<table border='1'>"

    count = 0

    csvfile = file(filename, 'rb')

    reader = csv.reader(csvfile)

    for fields in reader:
        try:
            if count == 0:
                color = "lightgreen"
                table += print_header(column_name_list['name'], color, maxwidth)
                count += 1
                continue
            elif count % 2:
                color = "white"
            else:
                color = "lightyellow"

            table += print_line(fields, column_name_list['index'], color, maxwidth)

            count += 1
        except EOFError:
            break

    csvfile.close()

    # end
    table += "</table>"

    return table

def print_start():
    print("<table border='1'>")

def print_end():
    print("</table>")

def print_header(fields, color, maxwidth):
    line = ""
    #print("<tr bgcolor='{}'>".format(color))
    line += "<tr bgcolor='{}'>".format(color)
    for field in fields:
        #print("<td align='center'>{}</td>".format(field))
        line += "<td align='center'>{}</td>".format(field)

    #print("</tr>")
    line += "</tr>"
    print line

    return line

def print_line(fields, column_index, color, maxwidth):
    line = ""
    #print("<tr bgcolor='{}'>".format(color))
    line += "<tr bgcolor='{}'>".format(color)
    index = 0
    for field in fields:
        #print("<td align='center'>{}</td>".format(field))
        if index in column_index:
            line += "<td align='center'>{}</td>".format(field)
        index += 1

    #print("</tr>")
    line += "</tr>"
    print line

    return line

def extract_fields(line):
    fields = []
    fields = line.split(",")
    return fields

def escape_html(text):
    text = text.replace("&", "&amp;")
    text = text.replace("<", "&lt;")
    text = text.replace(">", "&gt;")
    return text

#column_name_list = {}
#column_name_list['name'] = ['代码', '名称', '每股收益', '净资产收益率', '市盈率', '市净率', 'PEG']
#column_name_list['index'] = [0, 1, 2, 5, 20, 21, 22]

#csv2html("f://thebestroe_report_20161014.csv", "f://demo_1.html", "test", column_name_list)