class HanoiTowers:
    def __init__(self):
        self._towers = {1: [], 2: [], 3: []}
        self._order = ((1, 3), (1, 2), (2, 3))
        self._current = None

    def __getitem__(self, item: int) -> list:
        return self._towers[item]

    def height(self, i) -> int:
        return len(self._towers[i])

    def reset(self):
        self._towers[1] = [i for i in range(12, 0, -1)]
        self._towers[2] = []
        self._towers[3] = []
        self._current = 0

    def has_next(self) -> bool:
        return len(self._towers[2]) != 12 and\
               len(self._towers[3]) != 12

    def _swap(self, f: int, s: int) -> tuple:
        f_len = len(self._towers[f])
        s_len = len(self._towers[s])
        if f_len == 0:
            self._towers[f].append(self._towers[s].pop())
            return s, f, self._towers[f][-1]
        elif s_len == 0:
            self._towers[s].append(self._towers[f].pop())
            return f, s, self._towers[s][-1]
        elif self._towers[f][-1] < self._towers[s][-1]:
            self._towers[s].append(self._towers[f].pop())
            return f, s, self._towers[s][-1]
        else:
            self._towers[f].append(self._towers[s].pop())
            return s, f, self._towers[f][-1]

    def next(self) -> tuple:
        if self.has_next():
            ret = self._swap(*self._order[self._current])
            self._current = (self._current + 1) % 3
            return ret

    def dumps(self) -> str:
        return "1: {}\n2: {}\n3: {}".format(self._towers[1], self._towers[2], self._towers[3])


class HanoiTowersShuffled:
    def __init__(self):
        from collections import deque
        self._towers = {1: [], 2: [], 3: []}
        self._current = 0
        self.start_position = []
        self.steps = deque()

    def __getitem__(self, item: int) -> list:
        return self._towers[item]

    def height(self, i) -> int:
        return len(self._towers[i])

    def reset(self, rings_order=None):
        self.steps.clear()
        if rings_order is not None:
            self.start_position = rings_order[:]
        self._towers[1] = self.start_position[:]
        self._towers[2] = []
        self._towers[3] = []
        self._move_all(1, 2, 3)

        self._towers[1] = self.start_position[:]
        self._towers[2].clear()
        self._towers[3].clear()
        self._current = 0

    def next(self) -> tuple:
        f, t, r = self.steps[self._current]
        self._towers[t].append(self._towers[f].pop())
        self._current += 1
        return self.steps[self._current - 1]

    def has_next(self) -> bool:
        return len(self.steps) > self._current

    def _top(self, t: int) -> int:
        if len(self._towers[t]) > 0:
            return self._towers[t][-1]
        else:
            return 10000

    def _move_all(self, fr: int, to: int, sp: int):
        while len(self._towers[fr]) > 0:
            self._move_top1(fr, to, sp)
        if len(self._towers[sp]) > 0:
            self._move_top(self._towers[sp][0] + 1, sp, to, fr)

    def _move_top1(self, fr: int, to: int, sp: int):
        if len(self._towers[fr]) == 0:
            return
        if self._top(fr) < self._top(to):
            self._move(fr, to)
        else:
            self._move_top(self._top(fr), to, sp, fr)
            self._move(fr, to)

    def _move_top(self, cut: int, fr: int, to: int, sp: int):
        sz = -1
        for v in reversed(self._towers[fr]):
            if v < cut:
                sz = v
            else:
                break

        if sz == -1:
            return

        self._move_top(sz, fr, sp, to)
        self._move_top1(fr, to, sp)
        self._move_top(sz, sp, to, fr)

    def _move(self, fr: int, to: int):
        self.steps.append((fr, to, self._towers[fr][-1]))
        self._towers[to].append(self._towers[fr].pop())
