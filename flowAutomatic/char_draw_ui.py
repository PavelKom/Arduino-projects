from tkinter import *
import math

def install(package):
    import os
    os.system('pip install '+package)

try:
    import pyperclip
except:
    install('pyperclip')
    import pyperclip

ROWS = 8
COLUMS = 5

root = Tk()
frame = Frame(root)
frame.pack()
_rows = ROWS - 1
max_len = (math.ceil(math.log10((2 ** ROWS) - 1 ))+1)*COLUMS+1
checks = []
vars = []
text_var = StringVar()

def calc():
    result = []
    for var_line in vars:
        result.append(0)
        for i, var in enumerate(var_line):
            result[-1] += int(var.get()) * (2 ** (_rows-i))
    #print(result)
    buffer = "{"+",".join(map(str, result))+"}"
    pyperclip.copy(buffer)
    text_var.set(buffer)
def clear():
    for var_line in vars:
        for var in var_line:
            var.set(False)
    text_var.set("")
def bind_event(event):
    print(event)

for j in range(COLUMS):
    vars.append([])
    for i in range(ROWS):
        vars[-1].append(BooleanVar(value=False))
        checks.append(Checkbutton(frame, text="    ", indicatoron=False, selectcolor="black", variable=vars[-1][-1]))
        #checks[-1].bind("<Enter>", lambda e, b=checks[-1]: bind_event(b, e))
        checks[-1].grid(row=i, column=j)
Button(root, text="Get num", command=calc).pack() #.grid(row=0, column=5)
Entry(root, textvariable=text_var,width=max_len, justify=CENTER).pack() #.grid(row=1, column=5)
Button(root, text="Clear", command=clear).pack() #.grid(row=2, column=5)


root.mainloop()
