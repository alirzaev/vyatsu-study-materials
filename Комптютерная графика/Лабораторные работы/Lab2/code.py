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

        button_simple = tk.Button(self, text='Simple', command=self.circle_simple_general)
        button_brez8 = tk.Button(self, text='Brezenhem', command=self.circle_brezenhem_general)
        button_clear_all = tk.Button(self, text='Clear all', command=lambda: self.canvas.delete('all'))
        color_button = tk.Button(self, text='Color', command=self.choose_color)
        
        button_simple.grid(row=1, column=0)
        button_brez8.grid(row=1, column=1)
        button_clear_all.grid(row=1, column=2)
        color_button.grid(row=3, column=2)
        
        self.x0_var = tk.IntVar()
        self.y0_var = tk.IntVar()
        self.radius_var = tk.IntVar()
        self.color_var = tk.StringVar(); self.color_var.set('#000000')

        label_x0 = tk.Label(self, text='X1:')
        label_y0 = tk.Label(self, text='Y1:')
        label_radius = tk.Label(self, text='Radius:')

        label_x0.grid(row=2, column=0)
        label_y0.grid(row=3, column=0)
        label_radius.grid(row=2, column=2)

        entry_x0 = tk.Entry(self, textvar=self.x0_var)
        entry_y0 = tk.Entry(self, textvar=self.y0_var)
        entry_radius = tk.Entry(self, textvar=self.radius_var)

        entry_x0.grid(row=2, column=1)
        entry_y0.grid(row=3, column=1)
        entry_radius.grid(row=2, column=3)

    def draw_pixel(self, x, y):
        self.canvas.create_rectangle(x+1, y+1, x+2, y+2, fill=self.color_var.get(), width=0)

    def circle_simple_general(self):
        x0, y0 = self.x0_var.get(), self.y0_var.get()
        r = self.radius_var.get()
        for x in range(r + 1):
            y = round((r * r - x * x) ** 0.5)

            self.draw_pixel(x0 - x, y0 - y)
            self.draw_pixel(x0 - x, y0 + y)
            self.draw_pixel(x0 + x, y0 + y)
            self.draw_pixel(x0 + x, y0 - y)

    def circle_brezenhem_general(self):
        x0, y0 = self.x0_var.get(), self.y0_var.get()
        r = self.radius_var.get()

        x, y = 0, r
        e = 3 - 2 * r

        while x <= y:
            self.draw_pixel(x0 + x, y0 + y)
            self.draw_pixel(x0 + y, y0 + x)
            self.draw_pixel(x0 + y, y0 - x)
            self.draw_pixel(x0 + x, y0 - y)
            self.draw_pixel(x0 - x, y0 - y)
            self.draw_pixel(x0 - y, y0 - x)
            self.draw_pixel(x0 - y, y0 + x)
            self.draw_pixel(x0 - x, y0 + y)

            if e >= 0:
                e += 4 * (x - y) + 10
                y -= 1
            else:
                e += 4 * x + 6

            x += 1

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
