import tkinter as tk

class Window(tk.Tk):
    def __init__(self, width: int, height: int, parent=None):
        super(Window, self).__init__(parent)
        self.geometry('{}x{}'.format(width, height))
        self.width = width
        self.height = height
        self.build()

    def build(self):
        self.canvas = tk.Canvas(self, width=640, height=480, highlightbackground='black')
        self.canvas.grid(row=0, column=0, rowspan=1, columnspan=4)

        button_simple = tk.Button(self, text='Simple', command=self.line_simple_general)
        button_brez8 = tk.Button(self, text='Brezenhem 8', command=self.line_brez8_general)
        button_brez4 = tk.Button(self, text='Brezenhem 4', command=self.line_brez4_general)
        button_clear_all = tk.Button(self, text='Clear all', command=lambda: self.canvas.delete('all'))
        color_button = tk.Button(self, text='Color', command=self.choose_color)
        
        button_simple.grid(row=1, column=0)
        button_brez8.grid(row=1, column=1)
        button_brez4.grid(row=1, column=2)
        button_clear_all.grid(row=1, column=3)
        color_button.grid(row=4, column=0)
        
        self.x1_var = tk.IntVar()
        self.x2_var = tk.IntVar()
        self.y1_var = tk.IntVar()
        self.y2_var = tk.IntVar()
        self.m_var = tk.DoubleVar()
        self.color_var = tk.StringVar(); self.color_var.set('#000000')
        self.star_mode_var = tk.IntVar()
        
        label_x1 = tk.Label(self, text='X1:')
        label_x2 = tk.Label(self, text='X2:')
        label_y1 = tk.Label(self, text='Y1:')
        label_y2 = tk.Label(self, text='Y2:')
        label_m = tk.Label(self, text='m:')
        star_mode_switcher = tk.Checkbutton(self, text='Star mode', variable=self.star_mode_var)
        
        label_x1.grid(row=2, column=0)
        label_x2.grid(row=2, column=2)
        label_y1.grid(row=3, column=0)
        label_y2.grid(row=3, column=2)
        #label_m.grid(row=4, column=0)
        star_mode_switcher.grid(row=5, column=0)
        
        entry_x1 = tk.Entry(self, textvar=self.x1_var)
        entry_x2 = tk.Entry(self, textvar=self.x2_var)
        entry_y1 = tk.Entry(self, textvar=self.y1_var)
        entry_y2 = tk.Entry(self, textvar=self.y2_var)
        entry_m = tk.Entry(self, textvar=self.m_var)
        
        entry_x1.grid(row=2, column=1)
        entry_x2.grid(row=2, column=3)
        entry_y1.grid(row=3, column=1)
        entry_y2.grid(row=3, column=3)
        #entry_m.grid(row=4, column=1)

    def draw_pixel(self, x, y):
        self.canvas.create_rectangle(x+1, y+1, x+2, y+2, fill=self.color_var.get(), width=0)
        
    def draw_line_simple(self, pos1, pos2):
        x1, y1 = pos1
        x2, y2 = pos2
        if x1 != x2:
            m = (y2 - y1) / (x2 - x1)
            if x1 < x2:
                y = y1
                for x in range(x1, x2 + 1):
                    self.draw_pixel(x, round(y))
                    y += m
            elif x1 > x2:
                y = y2
                for x in range(x2, x1 + 1):
                    self.draw_pixel(x, round(y))
                    y += m
        elif y1 == y2:
            self.draw_pixel(x1, y1)
        else:
            for y in range(min(y1, y2), max(y1, y2) + 1):
                self.draw_pixel(x1, y)
            
    def draw_line_brez8(self, pos1, pos2):
        sign = lambda a: 1 if a >= 0 else -1
        x1, y1 = pos1; x2, y2 = pos2
        x, y = x1, y1; dx, dy = abs(x2 - x1), abs(y2 - y1)
        s1, s2 = sign(x2 - x1), sign(y2 - y1)
        
        if dy > dx:
            dx, dy = dy, dx
            f = True
        else:
            f = False
            
        e = 2 * dy - dx
        for _ in range(1, dx + 1):
            self.draw_pixel(x, y)
            
            while e >= 0:
                if f:
                    x += s1
                else:
                    y += s2
                e -= 2 * dx
            
            if f:
                y += s2
            else:
                x += s1
            
            e += 2 * dy
        
        self.draw_pixel(x, y)
        
    def draw_line_brez4(self, pos1, pos2):
        sign = lambda a: 1 if a >= 0 else -1
        x1, y1 = pos1; x2, y2 = pos2
        x, y = x1, y1; dx, dy = abs(x2 - x1), abs(y2 - y1)
        s1, s2 = sign(x2 - x1), sign(y2 - y1)
        
        if dy >= dx:
            dx, dy = dy, dx
            f = True
        else:
            f = False
            
        e = 2 * dy - dx
        for _ in range(1, dx + dy):
            self.draw_pixel(x, y)
            
            if e < 0:
                if f:
                    y += s2
                else:
                    x += s1
                e += 2 * dy
            else:
                if f:
                    x += s1
                else:
                    y += s2
                e -= 2 * dx
        
        self.draw_pixel(x, y)
    
    def line_simple_general(self):
        if self.star_mode_var.get() == 0:
            x1, y1, x2, y2 = self.x1_var.get(), self.y1_var.get(), self.x2_var.get(), self.y2_var.get()
            self.draw_line_simple((x1, y1), (x2, y2))
        else:
            self.draw_star(120, (320, 240), self.draw_line_simple)
        
    def line_brez8_general(self):
        if self.star_mode_var.get() == 0:
            x1, y1, x2, y2 = self.x1_var.get(), self.y1_var.get(), self.x2_var.get(), self.y2_var.get()
            self.draw_line_brez8((x1, y1), (x2, y2))
        else:
            self.draw_star(120, (320, 240), self.draw_line_brez8)
        
    def line_brez4_general(self):
        if self.star_mode_var.get() == 0:
            x1, y1, x2, y2 = self.x1_var.get(), self.y1_var.get(), self.x2_var.get(), self.y2_var.get()
            self.draw_line_brez4((x1, y1), (x2, y2))
        else:
            self.draw_star(120, (320, 240), self.draw_line_brez4)
        
    def draw_star(self, length, center, algo):
        from math import sin, cos, pi
        step = pi / 10
        base_x, base_y = center
        for i in range(20):
            angle = i * step
            x, y = cos(angle) * length + base_x, sin(angle) * length + base_y
            end = round(x), round(y)
            algo(center, end)

    def choose_color(self):
        from tkinter.colorchooser import askcolor
        color = askcolor()
        if color[1] is not None:
            self.color_var.set(color[1])


def main():
    window = Window(645, 640)
    window.mainloop()

if __name__ == '__main__':
    main()
