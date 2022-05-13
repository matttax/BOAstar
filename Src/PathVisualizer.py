import xml.etree.ElementTree as ET
import pygame
import tkinter
from tkinter import *

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)


class Button:
    def __init__(self, x, y, width, height, color, text=None, text_color=BLACK):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.color = color
        self.text = text
        self.text_color = text_color

    def draw(self, win):
        pygame.draw.rect(
            win, self.color, (self.x, self.y, self.width, self.height))
        pygame.draw.rect(
            win, BLACK, (self.x, self.y, self.width, self.height), 2)
        if self.text:
            button_font = get_font(22)
            text_surface = button_font.render(self.text, 1, self.text_color)
            win.blit(text_surface, (self.x + self.width /
                                    2 - text_surface.get_width() / 2,
                                    self.y + self.height / 2 - text_surface.get_height() / 2))

    def clicked(self, pos):
        x, y = pos

        if not (self.x <= x <= self.x + self.width):
            return False
        if not (self.y <= y <= self.y + self.height):
            return False

        return True


def get_font(size):
    return pygame.font.SysFont("comicsans", size)


def init_grid(rows, cols, color):
    grid = []
    for i in range(rows_indent, rows - rows_indent):
        grid.append([])
        for j in range(cols_indent, cols - cols_indent):
            if 0 <= i < len(path_map) and 0 <= j < len(path_map[0]):
                if float(path_map[i][j]) < 0:
                    if float(path_map[i][j]) * 40 == -200:
                        grid[i - rows_indent].append((0, 0, 200 + float(path_map[i][j]) * 40))
                    else:
                        grid[i - rows_indent].append((0, 200 + float(path_map[i][j]) * 40, 0))
                elif float(path_map[i][j]) == 100:
                    grid[i - rows_indent].append((250, 0, 0))
                elif float(path_map[i][j]) == 101:
                    grid[i - rows_indent].append((200, 0, 0))
                else:
                    clr = 255 - float(path_map[i][j]) * 255 / 1.2 if float(path_map[i][j]) != 1 else 0
                    grid[i - rows_indent].append((clr, clr, clr))
            else:
                grid[i - rows_indent].append(WHITE)
    return grid


def draw_grid(win, grid):
    for i, row in enumerate(grid):
        for j, pixel in enumerate(row):
            pygame.draw.rect(win, pixel, (j * PIXEL_SIZE, i *
                                          PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

    for i in range(ROWS + 1):
        pygame.draw.line(win, BLACK, (0, i * PIXEL_SIZE), (WIDTH, i * PIXEL_SIZE))

    for i in range(COLS + 1):
        pygame.draw.line(win, BLACK, (i * PIXEL_SIZE, 0), (i * PIXEL_SIZE, HEIGHT - TOOLBAR_HEIGHT))


def draw(win, grid, buttons):
    win.fill(BG_COLOR)
    draw_grid(win, grid)

    for button in buttons:
        button.draw(win)

    pygame.display.update()


def get_row_col_from_pos(pos):
    x, y = pos
    row = y // PIXEL_SIZE
    col = x // PIXEL_SIZE
    if row >= ROWS:
        raise IndexError
    return row, col


def to_map():
    map = []
    for i in range(len(root[0][2])):
        map.append(root[0][2][i].text.split())
    return map


def init_map():
    global path_map
    path_map = to_map()
    current = -1
    for p in path:
        for i in range(len(root[1][p][2])):
            row = root[1][p][2][i].text.split()
            for j in range(len(row)):
                if row[j] == '*':
                    path_map[i][j] = str(current)
        current -= 1
    path_map[int(root[0][3].text)][int(root[0][4].text)] = 100
    path_map[int(root[0][5].text)][int(root[0][6].text)] = 101


pygame.init()
pygame.font.init()
programIcon = pygame.image.load("df.png")
pygame.display.set_icon(programIcon)

WIDTH, HEIGHT = 600, 700
TOOLBAR_HEIGHT = HEIGHT - WIDTH
BG_COLOR = WHITE
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("BOA* PathPlanning Visualizer")
filename = sys.argv[1]
tree = ET.parse(filename)
root = tree.getroot()
path = [0]
path_map = to_map()
path_map[int(root[0][3].text)][int(root[0][4].text)] = 100
path_map[int(root[0][5].text)][int(root[0][6].text)] = 101

ROWS = COLS = max(int(root[0][0].text), int(root[0][1].text)) + 5
PIXEL_SIZE = WIDTH // COLS
rows_indent = cols_indent = -3
grid = init_grid(ROWS, COLS, BG_COLOR)

button_y = HEIGHT - TOOLBAR_HEIGHT / 2 - 25
buttons = [
    Button(130, button_y, 100, 50, WHITE, "Config", BLACK),
    Button(70, button_y, 50, 50, WHITE, "+", BLACK),
    Button(10, button_y, 50, 50, WHITE, "-", BLACK)
]
prev_pos = pos = (0, 0)
rows_delta = cols_delta = 0
run = True
clicked = False

while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

        if pygame.mouse.get_pressed()[0]:
            pos = pygame.mouse.get_pos()

            for button in buttons:
                if not button.clicked(pos):
                    if event.type == pygame.MOUSEBUTTONDOWN:
                        prev_pos = pos
                        clicked = True
                    continue

                if button.text == "+" and ROWS > 10 and COLS > 10:
                    ROWS -= 10
                    COLS -= 10
                    PIXEL_SIZE = WIDTH // COLS
                    grid = init_grid(ROWS, COLS, BG_COLOR)
                    drawing_color = BLACK
                if button.text == "-" and ROWS < 60 and COLS < 60:
                    ROWS += 10
                    COLS += 10
                    PIXEL_SIZE = WIDTH // COLS
                    grid = init_grid(ROWS, COLS, BG_COLOR)
                    drawing_color = BLACK
                if button.text == "Config":
                    window = Tk()
                    window.title("Choose paths")
                    window.geometry("300x300")
                    window.resizable(False, False)
                    scrollbar = tkinter.Scrollbar(window, orient="vertical")
                    lb = tkinter.Listbox(window, width=300, height=15, yscrollcommand=scrollbar.set,
                                         selectmode=MULTIPLE)
                    for i in range(len(root[1])):
                        lb.insert(i, "Length = " + "{:.2f}".format(
                            float(root[1][i][0].text)) + "    Danger = " + "{:.2f}".format(
                            float(root[1][i][1].text)))
                    lb.pack()


                    def selected_item():
                        global path
                        path = []
                        allowed = 3
                        for i in lb.curselection():
                            if allowed > 0:
                                path.append(i)
                                allowed -= 1
                            else:
                                break
                        window.destroy()


                    btn = tkinter.Button(window, text='Show Selected', command=selected_item)
                    btn.pack(side='bottom')
                    window.mainloop()
                    init_map()
                    grid = init_grid(ROWS, COLS, BG_COLOR)

        if event.type == pygame.MOUSEBUTTONUP:
            clicked = False

        if event.type == pygame.MOUSEMOTION:
            if clicked:
                if abs((pos[1] - prev_pos[1]) // PIXEL_SIZE) > abs(rows_delta):
                    rows_indent -= 1 if (pos[1] - prev_pos[1]) // PIXEL_SIZE > 0 else -1
                if abs((pos[0] - prev_pos[0]) // PIXEL_SIZE) > abs(cols_delta):
                    cols_indent -= 1 if (pos[0] - prev_pos[0]) // PIXEL_SIZE > 0 else -1
                rows_delta = (pos[1] - prev_pos[1]) // PIXEL_SIZE
                cols_delta = (pos[0] - prev_pos[0]) // PIXEL_SIZE
                grid = init_grid(ROWS, COLS, BG_COLOR)

    draw(WIN, grid, buttons)

pygame.quit()
